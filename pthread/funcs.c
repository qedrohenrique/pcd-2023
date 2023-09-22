/*
  Pedro Henrique de Almeida  - 148526
  Matheus Ferreira Santos - 148410
  Rafaela Cristine dos Santos Uchôas - 140351
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <wchar.h>
#include <locale.h>
#include <sys/time.h>

#include <GL/gl.h>
#include <GL/freeglut.h>

#include "funcs.h"
#include "config.h"


int getNeighbors(float** grid, int i, int j){
  int i_low = (i + 1) % GRID_SIZE;
  int i_high = (i - 1 + GRID_SIZE) % GRID_SIZE;
  int j_low = (j - 1 + GRID_SIZE) % GRID_SIZE;
  int j_high = (j + 1) % GRID_SIZE;

  int pos[8][2] = {
    {i_low, j_low},
    {i_low, j},
    {i_low, j_high},
    {i, j_low},
    {i, j_high},
    {i_high, j_low},
    {i_high, j},
    {i_high, j_high}
  };
  int count = 0;

  for(int c = 0; c < 8; c++)
    if(grid[pos[c][0]][pos[c][1]] != 0)
      count++;

	return count;
}

float getNeighborsAvg(float** grid, int i, int j){
  int i_low = (i + 1) % GRID_SIZE;
  int i_high = (i - 1 + GRID_SIZE) % GRID_SIZE;
  int j_low = (j - 1 + GRID_SIZE) % GRID_SIZE;
  int j_high = (j + 1) % GRID_SIZE;

  int pos[8][2] = {
    {i_low, j_low},
    {i_low, j},
    {i_low, j_high},
    {i, j_low},
    {i, j_high},
    {i_high, j_low},
    {i_high, j},
    {i_high, j_high}
  };
  int sum = 0;

  for(int c = 0; c < 8; c++)
    if(grid[pos[c][0]][pos[c][1]] != 0)
      sum += grid[pos[c][0]][pos[c][1]];

  return sum / 8.0;
}

void print_grid(float** grid_ptr){
  for(int i = 0; i < GRID_SIZE; i++){
    for(int j = 0; j < GRID_SIZE; j++){
      if(grid_ptr[i][j] != 0)
        wprintf(L"%lc ", 0x25A0);
      else
        wprintf(L"%lc ", 0x25A1);
    }
    wprintf(L"\n");
  }
  wprintf(L"\n");
}

void print_grid_float(float** grid_ptr){
  for(int i = 0; i < GRID_SIZE; i++){
    for(int j = 0; j < GRID_SIZE; j++){
      if(grid_ptr[i][j] != 0)
        wprintf(L"%f ", grid_ptr[i][j]);
      else
        wprintf(L"%f ", grid_ptr[i][j]);
    }
    wprintf(L"\n");
  }
  wprintf(L"\n");
}

void setupGrid(float** grid){
  //GLIDER
  int lin = 1, col = 1;
  grid[lin  ][col+1] = 1;
  grid[lin+1][col+2] = 1;
  grid[lin+2][col  ] = 1;
  grid[lin+2][col+1] = 1;
  grid[lin+2][col+2] = 1;
  //R-pentomino
  lin =10, col = 30;
  grid[lin  ][col+1] = 1;
  grid[lin  ][col+2] = 1;
  grid[lin+1][col  ] = 1;
  grid[lin+1][col+1] = 1;
  grid[lin+2][col+1] = 1;
}

void fillGrid(float** grid){
  for(int i = 0;  i < GRID_SIZE; i++)
    for(int j = 0; j < GRID_SIZE; j++)
      grid[i][j] = 0;
}

void swapGrids(float** new_grid, float** old_grid){
  float** aux = new_grid;
  new_grid = old_grid;
  old_grid = aux;
}

int countAliveCells(float** grid){
  int c = 0;
  for(int i = 0; i < GRID_SIZE; i++)
    for(int j = 0; j < GRID_SIZE; j++)
      if(grid[i][j] != 0) c++;

  return c;
}

float** grid = {0};
int redraw_flag = 0;
int exit_flag = 0;

void* parallel_generation(void* arg) {
  thread_args* args = (thread_args*) arg;
  float** ptr1 = args->grid_ptr;
  float** ptr2 = args->newgrid_ptr;

  for(int i = 0 ; i < NUM_GEN; i++){
    *args->count_alive = 0;

    for (int j = args->start; j <= args->end; j++) {
      int idx_1 = j / GRID_SIZE;
      int idx_2 = j - idx_1 * GRID_SIZE;

      int nn = getNeighbors(ptr1, idx_1, idx_2);
      if(ptr1[idx_1][idx_2] != 0) {
        *args->count_alive += 1;

        if(nn == 2 || nn == 3){
          float avg = getNeighborsAvg(ptr1, idx_1, idx_2);
          if(avg > 0) ptr2[idx_1][idx_2] = 1;
        }
        else
          ptr2[idx_1][idx_2] = 0;

      } else {
        if(nn == 3){
          float avg = getNeighborsAvg(ptr1, idx_1, idx_2);
          if(avg > 0) ptr2[idx_1][idx_2] = 1;
        }
        else
          ptr2[idx_1][idx_2] = 0;
      }
    }

    float** aux = ptr1;
    ptr1 = ptr2;
    ptr2 = aux;

    pthread_barrier_wait(args->barrier);
    pthread_barrier_wait(args->barrier);
  }

  pthread_exit(NULL);
}

void runGeneration(float** grid_1, float** grid_2){
  int cells_per_thread = (GRID_SIZE * GRID_SIZE) / NUM_WORKERS;
  int counter = GRID_SIZE * GRID_SIZE;
  int actual = 0;

  pthread_t threads[NUM_WORKERS];
  thread_args args[NUM_WORKERS];
  int count_alive[NUM_WORKERS] = {0};

  pthread_barrier_t barrier;
  int err = pthread_barrier_init(&barrier, NULL, NUM_WORKERS + 1);
  if(err) {
    perror("Error on creating barrier\n");
    exit(1);
  }

  for(int i = 0; i < NUM_WORKERS; i++) {
    args[i].grid_ptr = grid_1;
    args[i].newgrid_ptr = grid_2;
    args[i].count_alive = &count_alive[i];
    args[i].start = actual;
    args[i].end = actual + cells_per_thread - 1;
    args[i].barrier = &barrier;
    if(i == NUM_WORKERS - 1)
      args[i].end = GRID_SIZE * GRID_SIZE - 1;

    pthread_create(&threads[i], NULL, parallel_generation, &args[i]);
    actual += cells_per_thread;
  }

  float** ptr1 = grid_1;
  float** ptr2 = grid_2;

  for(int i = 0; i < NUM_GEN; i++) {
    pthread_barrier_wait(&barrier);

    int count = 0;
    for(int j = 0; j < NUM_WORKERS; j++)
      count += count_alive[j];
    wprintf(L"Gen %d: %d\n", i, count);

    grid = ptr1;
    ptr1 = ptr2;
    ptr2 = grid;

    #ifdef OPENGL
    redraw_flag = 1;
    #endif

    #ifdef PRINT_CONSOLE
    print_grid(grid);
    #endif

    usleep(SLEEP_BETWEEN_FRAMES);

    pthread_barrier_wait(&barrier);
  }

  exit_flag = 1;

  for(int i = 0; i < NUM_WORKERS; i++)
    pthread_join(threads[i], NULL);

}

void* config_opengl(void* arg) {
  struct args* func_args = (struct args*)arg;

  glutInit(&func_args->argc, func_args->argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowPosition(250, 100);
  glutInitWindowSize(700, 700);
  glutCreateWindow("Rainbow Game of Life");
  glutDisplayFunc(draw);
  glutIdleFunc(idle);
  glutMainLoop();

  return NULL;
}

void draw() {
  float step = 2.0 / GRID_SIZE;

  if(!grid) {
    glFlush();
    return;
  }

  for (int i = 0; i < GRID_SIZE; i++) {
    for (int j = 0; j < GRID_SIZE; j++) {
      glColor3f(1, 1, 1);
      if(grid[i][j])
        glColor3f(0, 0, 0);
      glBegin(GL_POLYGON);
      glVertex2f(-1 + step * j, 1 - step * i);
      glVertex2f(-1 + step * j + step, 1 - step * i);
      glVertex2f(-1 + step * j + step, 1 - step * i - step);
      glVertex2f(-1 + step * j, 1 - step * i - step);
      glEnd();
    }
  }
  glFlush();
}

void idle() {
  if(!redraw_flag)
    return;

  if(exit_flag)
    exit(0);

  redraw_flag = 0;
  glutPostRedisplay();
}


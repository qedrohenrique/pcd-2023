#include <pthread.h>

#define NUM_GEN 100
#define GRID_SIZE 2048
#define NUM_WORKERS 4


typedef struct {
  int start;
  int end;
  pthread_barrier_t* barrier;
  float** grid_ptr;
  float** newgrid_ptr;
  int* count_alive;
} thread_args;

int getNeighbors(float** grid, int i, int j);
float getNeighborsAvg(float** grid, int i, int j);
int getAlive(float** grid);
void print_grid(float** grid_ptr);
void print_grid_float(float** grid_ptr);
void setupGrid(float** grid);
void fillGrid(float** grid);
void swapGrids(float** new_grid, float** old_grid);
int countAliveCells(float** grid);
void runGeneration(float** grid_1, float** grid_2);


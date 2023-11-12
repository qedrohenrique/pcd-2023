/*
  Pedro Henrique de Almeida  - 148526
  Matheus Ferreira Santos - 148410
  Rafaela Cristine dos Santos Uchôas - 140351
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>
#include <wchar.h>
#include <locale.h>
#include <sys/time.h>
#include <mpi.h>
#include "funcs.h"


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
    if(grid[pos[c][0]][pos[c][1]] == 1)
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

  // wprintf(L"AVG(%d, %d):%d\n ", i, j, sum/8.0);
  return sum / 8.0;
}

void print_grid(float** grid_ptr){
  setlocale(LC_CTYPE, "");
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

int countAliveCellsSection(float** grid, int start, int end){
	int q = 0;

	for(int i = start; i <= end; i++)
    for(int j = 0; j < GRID_SIZE; j++)
      if(grid[i][j]!=0)q++;

  return q;
}

void getBounds(int *start, int *end, int rank, int num_proc){
  if(rank == 0) *start = 0;
  else *start = rank * GRID_SIZE / num_proc + 1;

  if(rank == num_proc - 1) *end = GRID_SIZE - 1;
  else *end = (rank + 1) * GRID_SIZE / num_proc;
}

int runGeneration(float** grid_1, float** grid_2){
  int ierr, num_proc, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Status status[num_proc];

  int send_next[GRID_SIZE], 
      send_prev[GRID_SIZE], 
      recv_next[GRID_SIZE], 
      recv_prev[GRID_SIZE];

  int local_count = 0, global_count = 0;

  int start, end;
  getBounds(&start, &end, rank, num_proc);

  for(int x = 0; x < GRID_SIZE; x++){
    send_next[x] = grid_1[end][x];
    send_prev[x] = grid_1[start][x];
  }

  ierr = MPI_Sendrecv(
                      &send_prev, GRID_SIZE, MPI_INTEGER, (rank-1+num_proc)%num_proc, 20,
                      &recv_next, GRID_SIZE, MPI_INTEGER, (rank+num_proc+1)%num_proc, 20,
                      MPI_COMM_WORLD, status
                      );
  ierr = MPI_Sendrecv(
                      &send_next, GRID_SIZE, MPI_INTEGER, (rank+1)%num_proc, 10,
                      &recv_prev, GRID_SIZE, MPI_INTEGER, ((rank-1)+num_proc)%num_proc, 10,
                      MPI_COMM_WORLD, status
                      );

  ierr = MPI_Barrier(MPI_COMM_WORLD);

  for(int i = 0; i < NUM_GEN; i++){

    for(int x = 0; x < GRID_SIZE; x++){
      grid_1[(end+1)%GRID_SIZE][x] = recv_next[x];
      grid_1[(start-1+GRID_SIZE)%GRID_SIZE][x] = recv_prev[x];
    }

    for(int j = start; j <= end; j++){
      for(int k = 0; k < GRID_SIZE; k++){
        int nn = getNeighbors(grid_1, j, k);
        if((grid_1)[j][k] != 0){
          if(nn == 2 || nn == 3){
            float aliveCell = 0;
            if (getNeighborsAvg(grid_1, j, k) > 0) aliveCell = 1.0;
            grid_2[j][k] = aliveCell;
          }
          else
            grid_2[j][k] = 0;
        }
        else{
          if(nn == 3){
            float aliveCell = 0;
            if (getNeighborsAvg(grid_1, j, k) > 0) aliveCell = 1.0;
            grid_2[j][k] = aliveCell;
          }
          else
            grid_2[j][k] = 0;
        }
      }
    }

    float** aux = grid_1;
    grid_1 = grid_2;
    grid_2 = aux;

    local_count = countAliveCellsSection(grid_1, start, end);
    ierr = MPI_Barrier(MPI_COMM_WORLD);
    ierr = MPI_Reduce(&local_count, &global_count, 1, MPI_INTEGER, MPI_SUM, 0, MPI_COMM_WORLD);


  for(int x = 0; x < GRID_SIZE; x++){
    send_next[x] = grid_1[end][x];
    send_prev[x] = grid_1[start][x];
  }

  ierr = MPI_Sendrecv(
                      &send_prev, GRID_SIZE, MPI_INTEGER, (rank-1+num_proc)%num_proc, 20,
                      &recv_next, GRID_SIZE, MPI_INTEGER, (rank+num_proc+1)%num_proc, 20,
                      MPI_COMM_WORLD, status
                      );
  ierr = MPI_Sendrecv(
                      &send_next, GRID_SIZE, MPI_INTEGER, (rank+1)%num_proc, 10,
                      &recv_prev, GRID_SIZE, MPI_INTEGER, ((rank-1)+num_proc)%num_proc, 10,
                      MPI_COMM_WORLD, status
                      );

  ierr = MPI_Barrier(MPI_COMM_WORLD);
  if(rank == 0) wprintf(L"[%d] Alive: %d\n", i, global_count);
  ierr = MPI_Barrier(MPI_COMM_WORLD);

  }


  // for(i = 0; i < NUM_GEN; i++){
    

   
        
  //   float** aux = ptr1;
  //   ptr1 = ptr2;
  //   ptr2 = aux;
  //   for(int x = 0; x < GRID_SIZE; x++){
  //     send_next[x] = ptr1[end][x];
  //     send_prev[x] = ptr1[start][x];
  //   }

 

  //   ierr = MPI_Barrier(MPI_COMM_WORLD);

  // }

  // return alive_count;
}

 // Gambiarra pra printar a grid c/ 2 processos.

    // if(rank ==0) {
    //   for(int m = start; m <= end; m++){
    //     for(int n = 0; n < GRID_SIZE; n++){
    //       if(grid_1[m][n] != 0)
    //         wprintf(L"%lc ", 0x25A0);
    //       else
    //         wprintf(L"%lc ", 0x25A1);
    //     }
    //     wprintf(L"\n");
    //   }
    // }
    // ierr = MPI_Barrier(MPI_COMM_WORLD);
    // if(rank==1) {
    //   for(int m = start; m <= end; m++){
    //     for(int n = 0; n < GRID_SIZE; n++){
    //       if(grid_1[m][n] != 0)
    //         wprintf(L"%lc ", 0x25A0);
    //       else
    //         wprintf(L"%lc ", 0x25A1);
    //     }
    //     wprintf(L"\n");
    //   }
    //     wprintf(L"\n");
    // }
    // ierr = MPI_Barrier(MPI_COMM_WORLD);

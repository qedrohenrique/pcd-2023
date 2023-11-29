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

int main(int argc, char** argv){
  setlocale(LC_CTYPE, "");

  struct timeval inicio, final;
  struct timeval inicio_concorrente, final_concorrente;
  int tmili, tmili_concorrente;

  gettimeofday(&inicio, NULL);

  int i, j;

  float** grid = (float**)malloc(GRID_SIZE * sizeof(float*));
  float** newgrid = (float**)malloc(GRID_SIZE * sizeof(float*));
  for(i = 0; i < GRID_SIZE; i++){
    grid[i] = (float*)calloc(GRID_SIZE , sizeof(float));
    newgrid[i] = (float*)calloc(GRID_SIZE , sizeof(float));
  }

  fillGrid(grid);
  fillGrid(newgrid);
  setupGrid(grid);

  MPI_Init(&argc, &argv);

  gettimeofday(&inicio_concorrente, NULL);
  int alive = runGeneration(grid, newgrid);
  gettimeofday(&final_concorrente, NULL);

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if(rank == 0){
    gettimeofday(&final, NULL);

    tmili = (int)(1000 * (final.tv_sec - inicio.tv_sec)
      + (final.tv_usec - inicio.tv_usec) / 1000);

    tmili_concorrente = (int)
      (1000 * (final_concorrente.tv_sec - inicio_concorrente.tv_sec)
      + (final_concorrente.tv_usec - inicio_concorrente.tv_usec) / 1000);

    wprintf(L"tempo decorrido: %d milisegundos\n", tmili);
    wprintf(L"tempo trecho concorrente: %d milisegundos\n",
    tmili_concorrente);
  }

  int ierr = MPI_Finalize();

  return 0;
}


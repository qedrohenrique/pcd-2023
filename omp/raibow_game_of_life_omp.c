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
#include "funcs.h"


int main(int argc, char** argv){
	setlocale(LC_CTYPE, "");
  wprintf(L"Threads: %d\n", NUM_WORKERS);
  wprintf(L"Gens: %d\n", NUM_GEN);

  struct timeval inicio, final;
  struct timeval inicio_concorrente, final_concorrente;
  int tmili, tmili_concorrente;

  gettimeofday(&inicio, NULL);

  int i, j;

  /*
  *
  *   GRID MEMORY ALLOCATION
  *
  */

  float** grid = (float**)malloc(GRID_SIZE * sizeof(float*));
  float** newgrid = (float**)malloc(GRID_SIZE * sizeof(float*));
  for(i = 0; i < GRID_SIZE; i++){
    grid[i] = (float*)calloc(GRID_SIZE , sizeof(float));
    newgrid[i] = (float*)calloc(GRID_SIZE , sizeof(float));
  }

  /*
  *
  *   GRID SETUP
  *
  */
  fillGrid(grid);
  fillGrid(newgrid);
  setupGrid(grid);

  gettimeofday(&inicio_concorrente, NULL);
  int alive = runGeneration(grid, newgrid);
  gettimeofday(&final_concorrente, NULL);

  // wprintf(L"Vivos ao final: %d\n", alive);
  gettimeofday(&final, NULL);

  tmili = (int)(1000 * (final.tv_sec - inicio.tv_sec)
    + (final.tv_usec - inicio.tv_usec) / 1000);

  tmili_concorrente = (int)
    (1000 * (final_concorrente.tv_sec - inicio_concorrente.tv_sec)
    + (final_concorrente.tv_usec - inicio_concorrente.tv_usec) / 1000);

  wprintf(L"tempo decorrido: %d milisegundos\n", tmili);
  wprintf(L"tempo trecho concorrente: %d milisegundos\n",
    tmili_concorrente);

  return 0;
}


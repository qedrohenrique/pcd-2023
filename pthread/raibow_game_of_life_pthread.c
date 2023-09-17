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
#include "funcs.h"

pthread_barrier_t barrier;


int main(int argc, char** argv){
  wprintf(L"Threads: %d\n", NUM_WORKERS);
  wprintf(L"Gens: %d\n", NUM_GEN);
	setlocale(LC_CTYPE, "");

  struct timeval inicio, final;
  struct timeval inicio_concorrente, final_concorrente;
  int tmili, tmili_concorrente;

  gettimeofday(&inicio, NULL);

  int i, j, shift = 0;

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

  pthread_t tid[NUM_WORKERS+1];
	pthread_barrier_init (&barrier, NULL, NUM_WORKERS);

  /*
  *
  *   GRID SETUP
  *
  */
  fillGrid(grid);
  fillGrid(newgrid);
  setupGrid(grid);

  gettimeofday(&inicio_concorrente, NULL);
  for(j=0;j<NUM_WORKERS;j++){
		thread_args* arg;
		arg = (thread_args*)malloc(sizeof(thread_args));
		setupArgs(arg, grid, newgrid, shift);
		pthread_create(&(tid[j]), NULL, runGeneration, (void*) arg);
		shift++;
  }

	int* thread_count[NUM_WORKERS];
	int soma_total=0;
    for(j=0; j<NUM_WORKERS; j++){
    	pthread_join(tid[j], (void**) &(thread_count[j]));
    }
    gettimeofday(&final_concorrente, NULL);
    for(int i=0;i<NUM_WORKERS;i++){
    	soma_total+=*(thread_count[i]);
    }

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


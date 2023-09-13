#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>
#include <wchar.h>
#include <locale.h>
#include <sys/time.h>
#include "funcs.h"

#define NUM_GEN 5
#define GRID_SIZE 50
#define NUM_WORKERS 2

typedef struct {
    float** grid_ptr;
    float** newgrid_ptr;
} thread_args;

int getNeighbors(float** grid, int i, int j){
	int i_low = (i+1)%GRID_SIZE, i_high = (i-1+GRID_SIZE)%GRID_SIZE, j_low = (j-1+GRID_SIZE)%GRID_SIZE, j_high = (j+1)%GRID_SIZE;
	int pos[8][2] = {{i_low, j_low}, {i_low, j}, {i_low, j_high},
	{i, j_low}, {i, j_high},
	{i_high, j_low}, {i_high, j}, {i_high, j_high}};
	int count=0;

	for(int c=0;c<8;c++){
		if(grid[pos[c][0]][pos[c][1]] != 0){
			count++;
		}
	}
	return count;
}

float getNeighborsAvg(float** grid, int i, int j){
	int i_low = (i+1)%GRID_SIZE, i_high = (i-1+GRID_SIZE)%GRID_SIZE, j_low = (j-1+GRID_SIZE)%GRID_SIZE, j_high = (j+1)%GRID_SIZE;
	int pos[8][2] = {{i_low, j_low}, {i_low, j}, {i_low, j_high},
	{i, j_low}, {i, j_high},
	{i_high, j_low}, {i_high, j}, {i_high, j_high}};
	int sum=0;

	for(int c=0;c<8;c++){
		if(grid[pos[c][0]][pos[c][1]] != 0){
			sum += grid[pos[c][0]][pos[c][1]];
		}
	}
	// wprintf(L"AVG(%d, %d):%d\n ", i, j, sum/8.0);
	return sum/8.0;
}

int getAlive(float** grid){
	int q = 0, i, j;

	#pragma omp for private(i, j)	
		for(i=0; i<GRID_SIZE; i++){
    	    for(j=0; j<GRID_SIZE; j++){
    	    	if(grid[i][j]==1){q++;}
    	    }
    	}
    return q;
}

void print_grid(float** grid_ptr){
	for(int i=0;i<GRID_SIZE;i++){
		for(int j=0;j<GRID_SIZE;j++){
			if(grid_ptr[i][j]!=0){wprintf(L"%lc ", 0x25A0);}
			else{wprintf(L"%lc ", 0x25A1);}
		}
		wprintf(L"\n");
	}
	wprintf(L"\n");
}

void print_grid_float(float** grid_ptr){
	for(int i=0;i<GRID_SIZE;i++){
		for(int j=0;j<GRID_SIZE;j++){
			if(grid_ptr[i][j]!=0){wprintf(L"%f ", grid_ptr[i][j]);}
			else{wprintf(L"%f ", grid_ptr[i][j]);}
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
	for(int i = 0;  i < GRID_SIZE; i++){
		for(int j = 0; j < GRID_SIZE; j++){
			grid[i][j] = 0;
		}
	}
}

void swapGrids(float** new, float** old){
	float** aux = new;
	new = old;
	old = aux;
}

int countAliveCells(float** grid){
	int c = 0;
	for(int i = 0; i < GRID_SIZE; i++){
		for(int j = 0; j < GRID_SIZE; j++){
			if(grid[i][j] != 0) c++;
		}
	}
	return c;	
}

int runGeneration(void* arg1){
	thread_args arg = *((thread_args*) arg1);
	int i, j, k, alive_count=0;
	#pragma omp parallel num_threads(NUM_WORKERS) private(i, j, k) reduction(+: alive_count)
	{
		for(i=0;i<NUM_GEN;i++){
			#pragma omp single
			{
				wprintf(L"Gen %d: %d\n", i, countAliveCells(arg.grid_ptr));
			}
			#pragma omp for
				for(j=0;j<GRID_SIZE; j++){
					for(k=0;k<GRID_SIZE;k++){
						int nn = getNeighbors(arg.grid_ptr, j, k);
						if((arg.grid_ptr)[j][k]!=0){
							if(nn==2 || nn==3){
								float neighborAvg = getNeighborsAvg(arg.grid_ptr, j, k);
								(arg.newgrid_ptr)[j][k]=1;
							}
							else{
								(arg.newgrid_ptr)[j][k]=0;
							}
						}
						else{
							if(nn==3){
								float neighborAvg = getNeighborsAvg(arg.grid_ptr, j, k);
								(arg.newgrid_ptr)[j][k]=1;
							}
							else{
								(arg.newgrid_ptr)[j][k]=0;
							}
						}
					}
				}
			#pragma omp single
				{
				print_grid_float(arg.grid_ptr);
				}
			#pragma omp barrier 
			#pragma omp single
				{
					float** aux = arg.grid_ptr;
					arg.grid_ptr = arg.newgrid_ptr;
					arg.newgrid_ptr = aux;
				}
			#pragma omp barrier 
		}
		alive_count += getAlive(arg.grid_ptr);
	}
	return alive_count;
}

void setupArgs(thread_args* arg, float** grid_ptr, float** newgrid_ptr){ 
	arg->grid_ptr = grid_ptr;
	arg->newgrid_ptr = newgrid_ptr;
}

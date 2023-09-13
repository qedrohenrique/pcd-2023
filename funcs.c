#include "funcs.h"
#include <omp.h>

void print_grid_win(float** grid){
	for(int i = 0; i < GRID_SIZE; i++){
		for(int j = 0; j < GRID_SIZE; j++){
			if(grid[i][j]==1) printf("1 ");
			else printf("0 ");
		}
		printf("\n");
	}
}

void print_grid_unix(float** grid){
	for(int i = 0; i < GRID_SIZE; i++){
		for(int j = 0; j < GRID_SIZE; j++){
			if(grid[i][j]==1){wprintf(L"%lc ", 0x25A0);}
			else{wprintf(L"%lc ", 0x25A1);}
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

	return;
}

int getNeighbors(float** grid, int i, int j){ 
	int neighborsAlive=0;

	int right = (i + 1) % GRID_SIZE;
	int left = (i - 1 + GRID_SIZE) % GRID_SIZE;
	int bottom = (j - 1 + GRID_SIZE) % GRID_SIZE;
	int top = (j + 1) % GRID_SIZE;
	
	int neighbors[8][2] = {
		{right, bottom}, 
		{right, j}, 
		{right, top},
		{i, bottom}, 
		{i, top},
		{left, bottom}, 
		{left, j}, 
		{left, top}
	};

	for(int i = 0; i < 8; i++){
		if(grid[neighbors[i][0]][neighbors[i][1]] == 1)
			neighborsAlive++;
	}

	return neighborsAlive;
}

int countAliveCells(float** grid){
	int aliveCells = 0;
	for(int i = 0; i < GRID_SIZE; i++){
		for(int j = 0; j < GRID_SIZE; j++){
			if(grid[i][j]==1) aliveCells++;
		}
	}

	return aliveCells;
}

void swapGrids(float** old, float** new){
	float** aux = old;
	old = new;
	new = aux;
}

void runGenerations(float** grid, float** new_grid){
	int i, j, k, alive;

	for(i = 0; i < 20; i++){
		// alive = 0;
		#pragma omp parallel num_threads(8) private(j, k) reduction(+: alive)
		{
			#pragma omp single
			{
			// print_grid_unix(grid);
			}
			#pragma omp for
				for(j = 0; j < GRID_SIZE; j++){
					for(k = 0; k < GRID_SIZE; k++){
						// printf("(%d, %d - %d): %d\n", j, k, omp_get_thread_num(), getNeighbors(grid, j, k));
						int nn = getNeighbors(grid, j, k);
						if(grid[j][k]==1){
							if(nn==2 || nn==3){
								(new_grid)[j][k]=1;
							}
							else{
								(new_grid)[j][k]=0;
							}
						}
						else{
							if(nn==3){
								wprintf(L"(%d, %d) - %d\n", j, k, nn);
								(new_grid)[j][k]=1;
							}
							else{
								(new_grid)[j][k]=0;
							}
						}
						// printf("j = %d, k = %d\n",j,k);
					}
				}
				#pragma omp single
				{
				grid = new_grid;
				swapGrids(grid, new_grid);
				print_grid_unix(grid);
				}
				#pragma omp barrier 
		}

		// printf("Gen %d: %d alive.", i+1, alive);
	}
}
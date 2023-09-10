#include "funcs.h"

void print_grid_win(int** grid){
	for(int i = 0; i < GRID_SIZE; i++){
		for(int j = 0; j < GRID_SIZE; j++){
			if(grid[i][j]==1) printf("1 ");
			else printf("0 ");
		}
		printf("\n");
	}
}

void print_grid_unix(int** grid){
	for(int i = 0; i < GRID_SIZE; i++){
		for(int j = 0; j < GRID_SIZE; j++){
			if(grid[i][j]==1){wprintf(L"%lc ", 0x25A0);}
			else{wprintf(L"%lc ", 0x25A1);}
		}
		wprintf(L"\n");
	}
}

void setupGrid(int** grid){
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

int getNeighbors(int** grid, int i, int j){ 
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

int countAliveCells(int **grid){
	int aliveCells = 0;
	for(int i = 0; i < GRID_SIZE; i++){
		for(int j = 0; j < GRID_SIZE; j++){
			if(grid[i][j]==1) aliveCells++;
		}
	}

	return aliveCells;
}
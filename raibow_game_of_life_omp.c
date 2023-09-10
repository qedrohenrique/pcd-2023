/*
    Pedro Henrique de Almeida   -   148526
    Rafaela
    Matheus
*/
#include <stdio.h>
#include <stdlib.h>
#include "funcs.c"

int main (int argc, char **argv){

    int ** grid, ** new_grid;

    // ROWS
    grid = malloc(GRID_SIZE * sizeof(float));
    new_grid = malloc(GRID_SIZE * sizeof(float));

    // COLUMNS
    for(int i = 0; i < GRID_SIZE; i++){
		grid[i] = (float*) malloc(GRID_SIZE * sizeof(float));
		new_grid[i] = (float*) malloc(GRID_SIZE * sizeof(float));
	}

	setupGrid(grid);

    print_grid_win(grid);
	printf("\n");
	printf("%d\n", getNeighbors(grid, 2, 2));
	printf("%d\n", countAliveCells(grid));

    return 0;
}
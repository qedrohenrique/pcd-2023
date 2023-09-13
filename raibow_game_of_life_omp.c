/*
    Pedro Henrique de Almeida   -   148526
    Rafaela
    Matheus
*/
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "funcs.c"

int main (int argc, char **argv){
	setlocale(LC_CTYPE, "");
    float ** grid, ** new_grid;

    // ROWS
    grid = malloc(GRID_SIZE * sizeof(float));
    new_grid = malloc(GRID_SIZE * sizeof(float));

    // COLUMNS
    for(int i = 0; i < GRID_SIZE; i++){
		grid[i] = (float*) malloc(GRID_SIZE * sizeof(float));
		new_grid[i] = (float*) malloc(GRID_SIZE * sizeof(float));
	}

	setupGrid(grid);
    print_grid_unix(grid);
    runGenerations(grid, new_grid);

    return 0;
}
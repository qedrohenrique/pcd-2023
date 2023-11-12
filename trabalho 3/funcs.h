/*
  Pedro Henrique de Almeida  - 148526
  Matheus Ferreira Santos - 148410
  Rafaela Cristine dos Santos Uchôas - 140351
*/

#define NUM_GEN 2000
#define GRID_SIZE 2048


int getNeighbors(float** grid, int i, int j);
float getNeighborsAvg(float** grid, int i, int j);
int getAlive(float** grid);
void print_grid(float** grid_ptr);
void print_grid_float(float** grid_ptr);
void setupGrid(float** grid);
void fillGrid(float** grid);
int countAliveCells(float** grid);
int countAliveCellsSection(float** grid, int start, int end);
int runGeneration(float** grid_1, float** grid_2);
void getBounds(int *start, int *end, int rank, int num_proc);


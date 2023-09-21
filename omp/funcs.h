/*
  Pedro Henrique de Almeida  - 148526
  Matheus Ferreira Santos - 148410
  Rafaela Cristine dos Santos Uchôas - 140351
*/

#define NUM_GEN 3
#define GRID_SIZE 50
#define NUM_WORKERS 4


int getNeighbors(float** grid, int i, int j);
float getNeighborsAvg(float** grid, int i, int j);
int getAlive(float** grid);
void print_grid(float** grid_ptr);
void print_grid_float(float** grid_ptr);
void setupGrid(float** grid);
void fillGrid(float** grid);
int countAliveCells(float** grid);
int runGeneration(float** grid_1, float** grid_2);


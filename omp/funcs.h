#define NUM_GEN 10
#define GRID_SIZE 2048
#define NUM_WORKERS 4


int getNeighbors(float** grid, int i, int j);
float getNeighborsAvg(float** grid, int i, int j);
int getAlive(float** grid);
void print_grid(float** grid_ptr);
void print_grid_float(float** grid_ptr);
void setupGrid(float** grid);
void fillGrid(float** grid);
void swapGrids(float** new_grid, float** old_grid);
int countAliveCells(float** grid);
int runGeneration(float** grid_1, float** grid_2);


#define GRID_SIZE 50
#define NUM_THREADS 1

typedef struct {
    float ** grid;
    float ** new_grid;
} thread_args_t;

void print_grid_win(float** grid_ptr);
void print_grid_unix(float** grid_ptr);
void setupGrid(float** grid);
int getNeighbors(float** grid, int i, int j);
int countAliveCells(float **grid);
void swapGrids(float** old, float** new);
void runGenerations(float** grid, float** new_grid);
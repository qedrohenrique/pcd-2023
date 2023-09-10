#define GRID_SIZE 50
#define NUM_THREADS 1

typedef struct {
    int ** grid;
    int ** new_grid;
} thread_args_t;

void print_grid_win(int** grid_ptr);
void print_grid_unix(int** grid_ptr);
void setupGrid(int** grid);
int getNeighbors(int** grid, int i, int j);
int countAliveCells(int **grid);
void swapGrids(int** old, int** new);
void runGenerations(int** grid, int** new_grid);
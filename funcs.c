#define GRID_SIZE 48

typedef struct {
    int ** grid;
    int ** new_grid;
} thread_args_t;

void print_grid(int** grid_ptr){
	for(int i = 0; i < GRID_SIZE; i++){
		for(int j = 0; j < GRID_SIZE; j++){
			if(grid_ptr[i][j]==1){printf("1 ");}
			else{printf("0 ");}
		}
		printf(L"\n");
	}
}

void print_grid_unix(int** grid_ptr){
	for(int i = 0; i < GRID_SIZE; i++){
		for(int j = 0; j < GRID_SIZE; j++){
			if(grid_ptr[i][j]==1){wprintf(L"%lc ", 0x25A0);}
			else{wprintf(L"%lc ", 0x25A1);}
		}
		wprintf(L"\n");
	}
}
CC = gcc
MPI = mpicc
TARGET = raibow_game_of_life_mpi
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
CFLAGS = -fopenmp -pthread

all: $(OBJ)
	$(MPI) -o $(TARGET) $(OBJ)

%.o: %.c
	$(MPI) -o $@ -c $< $(CFLAGS)

clean:
	rm *.o
	rm raibow_game_of_life_mpi


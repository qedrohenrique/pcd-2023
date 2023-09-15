CC = gcc
TARGET = raibow_game_of_life_omp
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

all: $(OBJ)
	$(CC) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) -o $@ -c $<

clean:
	rm *.o
	rm raibow_game_of_life_omp


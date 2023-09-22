CC = gcc
TARGET = raibow_game_of_life_pthread
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
CFLAGS = -fopenmp -pthread

all: $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) $(CFLAGS)

%.o: %.c config.h
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm *.o
	rm raibow_game_of_life_pthread


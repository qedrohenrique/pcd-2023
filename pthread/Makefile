CC = gcc
TARGET = rainbow_game_of_life_pthread
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
CFLAGS = -fopenmp -pthread -lGL -lglut

all: $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) $(CFLAGS)

%.o: %.c config.h
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm *.o
	rm rainbow_game_of_life_pthread


BIN = dungen

CFLAGS = -O0 -g -std=c99 -Wall -Wextra -pedantic -flto -march=native

LDFLAGS = -lm

CC = gcc

SRC = main.c map.c

all:
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC) -o $(BIN)

run:
	./$(BIN)

clean:
	rm -f $(BIN)

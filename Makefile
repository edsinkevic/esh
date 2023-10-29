CC=gcc
SRC=src
BUILD=build
OPS=-Wall -Wextra -pedantic -std=c11

main.o: 
	$(CC) -c $(SRC)/main.c -o $(BUILD)/main.o $(OPS)

esh: main.o 
	$(CC) $(BUILD)/* -o esh

clean:
	rm -rf build/*

CC = gcc

all: calculator.x

calculator.x: calculator.c
	$(CC) -g calculator.c -o calculator.x



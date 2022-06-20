CC = g++

all: calculator.x

calculator.x: calculator.cpp
	$(CC) -g calculator.cpp -o calculator.x



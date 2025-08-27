# Simple makefile

CC := g++ -std=c++11 -O3

CCW := -Wall -Werror -Wextra

.DEFAULT_GOAL := main

main:
	mkdir -p bin
	$(CC) $(CCW) -o ./bin/main ./src/main.cpp
	./bin/main
	
all: clean run

clean:
	rm -rf ./bin/main

run: main.cpp
	./bin/main

CC=gcc
FLAGS=-Wall -g

.PHONY: all
all: task stdinExample

task: codec.h basic_main.c
	$(CC) $(FLAGS) $^ -L. -lCodec -o encoder

stdinExample: stdin_main.c
	$(CC) $(FLAGS) $^ -L. -lCodec -o tester

.PHONY: clean
clean:
	rm -rf encoder tester

CC=gcc
FLAGS=-Wall -g

.PHONY: all
all: coder

coder: coder.c codec.h LinkedList.c LinkedList.h ThreadPool.c ThreadPool.h
	$(CC) $(FLAGS) $^ -lpthread ./libCodec.so -o $@

.PHONY: clean
clean:
	rm -rf coder *.o




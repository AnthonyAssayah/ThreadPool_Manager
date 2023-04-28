all: coder

coder: coder.c codec.h LinkedList.c LinkedList.h ThreadPool.c ThreadPool.h
	gcc $^ -lpthread ./libCodec.so -o $@

.PHONY: all clean
clean:
	rm -rIf coder *.o *.a




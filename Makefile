all: Coder

Coder: coder.c codec.h LinkedList.c LinkedList.h
	gcc $^ ./libCodec.so -o $@

#queue.a: queue.o
#	ar rcs queue.a queue.o

#thpool.a: thpool.o
#	ar rcs thpool.a thpool.o -lpthread

#%.o: %.c
#	gcc -c $< -o $@

.PHONY: all clean
clean:
	rm -rIf coder *.o *.a




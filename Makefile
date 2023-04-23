all: Coder

Coder: coder.o codec.h queue.a thpool.a
	gcc -o Coder coder.o ./libCodec.so  

queue.a: queue.o
	ar rcs queue.a queue.o

thpool.a: thpool.o
	ar rcs thpool.a thpool.o -lpthread

%.o: %.c
	gcc -c $< -o $@

.PHONY: all clean
clean:
	rm -rIf coder *.o *.a




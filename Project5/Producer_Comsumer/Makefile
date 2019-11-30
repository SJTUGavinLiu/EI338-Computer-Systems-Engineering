CC=gcc
CFLAGS=-Wall
PTHREADS=-lpthread

all: buffer.o model.o
	$(CC) $(CFLAGS) -o test buffer.o model.o $(PTHREADS)

buffer.o: buffer.c buffer.h
	$(CC) $(CFLAGS) -c buffer.c 



clean:
	rm -rf *.o
	rm -rf example


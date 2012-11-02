CC=gcc
CFLAGS= -Wall -g

all: search

search: search.c sfunc.o
	$(CC) $(CFLAGS) search.c sfunc.o -o search

sfunc.o: sfunc.c sfunc.h
	$(CC) $(CFLAGS) -c sfunc.c



clean:
	rm -rf *.o search

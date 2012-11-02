CC=gcc
CFLAGS= -Wall -g

all: indexer

indexer: indexer.c sorted-list.o tokenizer.o
	$(CC) $(CFLAGS) indexer.c sorted-list.o tokenizer.o -o indexer

sorted-list.o: sorted-list.c sorted-list.h
	$(CC) $(CFLAGS) -c sorted-list.c

tokenizer.o: tokenizer.c tokenizer.h
	$(CC) $(CFLAGS) -c tokenizer.c

clean:
	rm -rf *.o indexer

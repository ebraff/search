CC=gcc
CFLAGS= -Wall -g

all: search indexer

search: search.c sfunc.o
	$(CC) $(CFLAGS) search.c sfunc.o -o search

sfunc.o: sfunc.c sfunc.h
	$(CC) $(CFLAGS) -c sfunc.c

indexer: indexer.c sorted-list.o tokenizer.o
	$(CC) $(CFLAGS) indexer.c sorted-list.o tokenizer.o -o indexer

sorted-list.o: sorted-list.c sorted-list.h
	$(CC) $(CFLAGS) -c sorted-list.c

tokenizer.o: tokenizer.c tokenizer.h
	$(CC) $(CFLAGS) -c tokenizer.c

clean:
	rm -rf *.o search indexer

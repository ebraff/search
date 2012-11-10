#ifndef SEARCH_H
#define SEARCH_H

/*
 *	search.h
 */
 
#include <stdlib.h>
#include "sfunc.h"

int compareWords(void* word1, void * word2);
int compareFiles(void* word1, void * word2);
SortedListPtr sand(SortedListPtr* table, long tl, char *searchWords);
SortedListPtr sor(SortedListPtr* table, long tl, char *searchWords);


#endif

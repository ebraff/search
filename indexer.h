#ifndef INDEXER_H
#define INDEXER_H
/*
 * indexer.h
 */

#include <stdlib.h>
#include "sorted-list.h"

int compareWords(void* word1, void * word2);
int compareFiles(void* word1, void * word2);
int compareOcc(void* num1, void* num2);


#endif
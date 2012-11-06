#ifndef SFUNC_H
#define SFUNC_H
/*
 * sfunct.h
 */

#include <stdlib.h>

typedef int (*CompareFuncT)(void *, void *);


/* word type obj holds a nodePtr to file list, 
 * file type node holds an integer word count */
struct Node
{
	struct SortedList* fileList;
	struct Node* next;
	void * object;
	int count;
};
typedef struct Node* NodePtr;

struct SortedList
{
	NodePtr head;
	CompareFuncT funct;
};
typedef struct SortedList* SortedListPtr;

struct SortedListIterator
{
	NodePtr curr;
	NodePtr prev;
};
typedef struct SortedListIterator* SortedListIteratorPtr;


SortedListPtr SLCreate(CompareFuncT cf);

void SLDestroy(SortedListPtr list);

NodePtr SLInsert(SortedListPtr list, void *newObj);

int SLRemove(SortedListPtr list, void *newObj);

SortedListIteratorPtr SLCreateIterator(SortedListPtr list);

void SLDestroyIterator(SortedListIteratorPtr iter);

void *SLNextItem(SortedListIteratorPtr iter);

int FileInsert(SortedListPtr list, void *newObj);

int ReInsert(SortedListPtr list, NodePtr newObj);

int DestroyTable(SortedListPtr* table, long tl);

void loadTable(SortedListPtr* table, FILE* input, long tl);
NodePtr getNode(SortedListPtr* table, char* word, long tl);

#endif

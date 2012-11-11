#ifndef SORTED_LIST_H
#define SORTED_LIST_H
/*
 * sorted-list.h
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

int SLInsert(SortedListPtr list, void *newObj, char* filename);

int SLRemove(SortedListPtr list, void *newObj);

SortedListIteratorPtr SLCreateIterator(SortedListPtr list);

void SLDestroyIterator(SortedListIteratorPtr iter);

void *SLNextItem(SortedListIteratorPtr iter);

int FileInsert(SortedListPtr list, void *newObj);

int ReInsert(SortedListPtr list, NodePtr newObj);

#endif
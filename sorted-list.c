/*
 * sorted-list.c
 */

#include	<string.h>
#include	<stdio.h>
#include	"sorted-list.h"
#include 	"indexer.h"



/*
 * SLCreate creates a new, empty sorted list.  The caller must provide
 * a comparator function that can be used to order objects that will be
 * kept in the list.
 * 
 * If the function succeeds, it returns a (non-NULL) SortedListT object.
 * Else, it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */

SortedListPtr SLCreate(CompareFuncT cf)
{
	SortedListPtr list= (SortedListPtr)malloc(sizeof(struct SortedList));

	if (list == NULL)
		return NULL;
	list->funct = cf;
	list->head = NULL;
	/*returns head of new list*/
	return list;
}

/*
 * SLDestroy destroys a list, freeing all dynamically allocated memory.
 * Objects should NOT be deallocated, however.  That is the responsibility
 * of the user of the list.
 *
 * You need to fill in this function as part of your implementation.
 */
void SLDestroy(SortedListPtr list)
{
	NodePtr temp;
	temp = list->head;
	/*free everything!!!*/
	while(temp != NULL)
	{
		temp = temp->next;
		if(list->head->fileList){
			SLDestroy(list->head->fileList);
		}
		free(list->head->object);
		free(list->head);
		list->head = temp;
	}
	free(list);

}

/*Added on to initialize struct Node given the object and 
 * the pointer to the next node in the list*/
NodePtr NodeCreate(void* object, NodePtr next)
{
	NodePtr newo;
	if (object == NULL)
	{
		return NULL;
	}
	newo = (NodePtr)malloc(sizeof(struct Node));
	if(newo == NULL)
		return NULL;
	newo->object = object;
	newo->next = next;
	return newo;
}

/*
 * SLInsert inserts a given object into a sorted list, maintaining sorted
 * order of all objects in the list.  If the new object is already in the list,
 * return a small error and do not insert it again.
 *
 * If the function succeeds, it returns 1.  Else, it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

int SLInsert(SortedListPtr list, void *newObj, char* filename)
{
	int compare;
	NodePtr newo;
	SortedListIteratorPtr it;
	SortedListPtr flist;

	newo = NULL;
	it = SLCreateIterator(list);
	if (it == NULL){
		SLDestroyIterator(it);
		return 0;
	}

	if (it->prev == NULL || (*list->funct)(newObj, it->curr->object) < 0) /*if the iterator's previous is NULL or if the new object is greater than the iterator*/
	{
		flist = SLCreate(compareFiles);
		newo = NodeCreate(newObj,it->curr);
		newo->fileList=flist;
		FileInsert(flist, filename); /*--------------------insert or increment filenode*/		
		newo->next = it->curr;
		list->head = newo;
		SLDestroyIterator(it);
		return 1;
	}
	while(it->curr != NULL){ /*---------------------loop to move iterator*/
		compare = (*list->funct)(newObj, it->curr->object);	
		if(compare <0) /*---------------------------if the iterator is less than the new object, insert it*/
		{
			break;
		}
		else if(compare > 0) /*---------------------if the iterator is greater than the new object, move the iterator to the next node(s)*/
		{
			it = SLNextItem(it);
		}
		else /*-------------------------------------if the words are the same/equal*/
		{
			/*find FILE and insert it*/
			flist = it->curr->fileList;
			FileInsert(flist,filename);
			

			free(newObj);		/*---- Word in list already, free new word node */
			SLDestroyIterator(it);
			return 1;				
		}
	}
	flist = SLCreate(compareFiles);
	newo=NodeCreate(newObj,it->curr);	
	it->prev->next =  newo;/*-----------------------smallest; put the new node on the end*/
	newo->fileList=flist;
	FileInsert(flist, filename);	
	SLDestroyIterator(it);
	return 1;
}


int FileInsert(SortedListPtr list, void *newObj)
{
	/*go through entire list until you find the file name or the end of 
	 * the list is reached (list is not necessarily in alphabetical order)
	 * if the end of the list is reached, increment the counter and insert at end*/
	int compare;
	NodePtr newo;
	NodePtr tempNode;
	SortedListIteratorPtr it;

	list->funct = compareFiles;

	it = SLCreateIterator(list);
	if (it == NULL){
		SLDestroyIterator(it);
		return 0;
	}

	if (it->prev == NULL)// || (*list->funct)(newObj, it->curr->object) < 0) /*if the iterator's previous is NULL or if the new object is greater than the iterator*/
	{
		newo = NodeCreate(newObj,it->curr);
		newo->count = 1;
		newo->fileList=NULL;
		newo->next = it->curr;
		list->head = newo;
		SLDestroyIterator(it);
		return 1;
	}	
	while(it->curr != NULL){ /*---------------------------------loop to move iterator*/
		compare = (*list->funct)(newObj, it->curr->object);
		if(compare == 0) /*-------------------------------------if the file already exists in the word's list*/
		{
			(it->curr->count)++;
			if(strcmp(it->prev->object,it->curr->object) == 0){
				free(newObj);
				SLDestroyIterator(it);
				return 1;
			}else{
				tempNode = it->curr;
				it->prev->next = it->curr->next;
			}
			free(newObj);
			
			ReInsert(list, tempNode);
			SLDestroyIterator(it);
			return 1;				
		}
		it = SLNextItem(it);
	}
	it->prev->next = NodeCreate(newObj,it->curr); /*------------smallest; put the new node on the end*/
	it->prev->next->count=1;
	it->prev->next->fileList=NULL;
	SLDestroyIterator(it);
	return 1;
}


int ReInsert(SortedListPtr list, NodePtr newObj)
{
	int compare;
	SortedListIteratorPtr it;

	//list->funct = compareOcc;
	it = SLCreateIterator(list);
	if (it == NULL){
		SLDestroyIterator(it);
		return 0;
	}
	if (it->prev == NULL || (newObj->count)-(it->curr->count) >= 0) /*--- (n1-n2) , insert DESENDING*/
	{
		newObj->next = list->head;
		list->head = newObj;
		SLDestroyIterator(it);
		return 1;
	}

	while(it->curr != NULL){ 
		compare = (newObj->count)-(it->curr->count);		
		if(compare >=0)
		{
			break;
		}
		else
		{
			it = SLNextItem(it);
		}		
	}
	it->prev->next = newObj;
	newObj->next = it->curr;
	SLDestroyIterator(it);
	return 1;
}


/*
 * SLRemove removes a given object from a sorted list.  Sorted ordering
 * should be maintained.
 *
 * If the function succeeds, it returns 1.  Else, it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

int SLRemove(SortedListPtr list, void *newObj)
{
	int compare;
	SortedListIteratorPtr it;

	it = SLCreateIterator(list);
	if (it == NULL){
		SLDestroyIterator(it);
		return 0;
	}

	if (it->prev == NULL) /*------------------------if the iterator's previous is NULL, don't do anything*/
	{
		SLDestroyIterator(it);
		return 1;
	}
	compare = (*list->funct)(newObj, it->curr->object);	
	if(compare == 0) /*-----------------------------if the first node in the list is the target*/
	{
		if(it->curr->next == NULL) /*---------------if that node was the only one in the list*/
		{
			free(it->curr->object);
			free(it->curr);
			SLDestroyIterator(it);
			list->head = NULL;
		}
		else /*-------------------------------------if that node was followed by nodes*/
		{
			list->head = it->curr->next;
			free(it->curr->object);
			free(it->curr);
			SLDestroyIterator(it);
		}
		return 1;
	}
	while(it->curr != NULL){ /*---------------------loop to move iterator*/
		compare = (*list->funct)(newObj, it->curr->object);		
		if(compare >0) /*---------------------------if the iterator is less than the new object, insert it*/
		{
			printf("Could not find object in list.\n");
			SLDestroyIterator(it);
			return 1;
		}
		else if(compare < 0) /*---------------------if the iterator is greater than the new object, move the iterator to the next node(s)*/
		{
			it = SLNextItem(it);
		}
		else /*-------------------------------------if they're the same/equal*/
		{
			if (it->curr != NULL)
			{
				it->prev->next = it->curr->next;
				free(it->curr->object);
				free(it->curr);
				it->curr = it->prev->next;
			}
			SLDestroyIterator(it);	
			return 1;				
		}
	}
	if (it->curr != NULL){
		it->prev->next = it->curr->next; /*---------delete the smallest*/
		free(it->curr->object);
		free(it->curr);
	}
	else
	{
		printf("Could not find object in list.\n");
	}
	SLDestroyIterator(it);
	return 1;
}


/*
 * SLCreateIterator creates an iterator object that will allow the caller
 * to "walk" through the list from beginning to the end using SLNextItem.
 *
 * If the function succeeds, it returns a non-NULL SortedListIterT object.
 * Else, it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */

SortedListIteratorPtr SLCreateIterator(SortedListPtr list)
{
	SortedListIteratorPtr it = (SortedListIteratorPtr)malloc(sizeof(struct SortedListIterator));
	if (it == NULL)
		return NULL;
	it->curr = list->head;
	it->prev = list->head;
	return it;
}



/*
 * SLDestroyIterator destroys an iterator object that was created using
 * SLCreateIterator().  Note that this function should destroy the
 * iterator but should NOT affectt the original list used to create
 * the iterator in any way.
 *
 * You need to fill in this function as part of your implementation.
 */

void SLDestroyIterator(SortedListIteratorPtr iter)
{
	free(iter);
}


/*
 * SLNextItem returns the next object in the list encapsulated by the
 * given iterator.  It should return a NULL when the end of the list
 * has been reached.
 *
 * One complication you MUST consider/address is what happens if a
 * sorted list encapsulated within an iterator is modified while that
 * iterator is active.  For example, what if an iterator is "pointing"
 * to some object in the list as the next one to be returned but that
 * object is removed from the list using SLRemove() before SLNextItem()
 * is called.
 *
 * You need to fill in this function as part of your implementation.
 */

void *SLNextItem(SortedListIteratorPtr iter)
{
		iter->prev = iter->curr;
		iter->curr = iter->prev->next;
		return iter;
}

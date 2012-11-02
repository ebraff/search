/*
 * sorted-list.c
 */

#include	<string.h>
#include	<stdio.h>
#include	"sfunc.h"
#include	"search.h"


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
	/*FREE FILENPTR?*/

	NodePtr temp;
	temp = list->head;
	/*free everything!!!*/
	while(temp != NULL)
	{
		temp = temp->next;
		SLDestroy(list->head->fileList);
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

NodePtr SLInsert(SortedListPtr list, void *newObj)
{
	int compare;
	NodePtr newo;
	SortedListIteratorPtr it;
	SortedListPtr flist;

	newo = NULL;

	it = SLCreateIterator(list);
	if (it == NULL){
		SLDestroyIterator(it);
		return NULL;
	}
	if (it->prev == NULL || (*list->funct)(newObj, it->curr->object) < 0) /*if the iterator's previous is NULL or if the new object is greater than the iterator*/
	{
		newo = NodeCreate(newObj,it->curr);
		newo->fileList=SLCreate(compareFiles);
		//FileInsert(flist, filename); /*--------------------insert or increment filenode*/		
		newo->next = it->curr;
		list->head = newo;
		SLDestroyIterator(it);
		return list->head;
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
			free(newObj);
			newo= it->curr;
			SLDestroyIterator(it);
			return newo;
		}
	}
	newo=NodeCreate(newObj,it->curr);
	it->prev->next =  newo;/*-----------------------smallest; put the new node on the end*/
	newo->fileList=SLCreate(compareFiles);
	SLDestroyIterator(it);
	return newo;
}


int FileInsert(SortedListPtr list, void *newObj)
{
	/*go through entire list until you find the file name or the end of 
	 * the list is reached (list is not necessarily in alphabetical order)
	 * if the end of the list is reached, increment the counter and insert at end*/
	SortedListIteratorPtr it;

	it = SLCreateIterator(list);
	if (it == NULL){
		SLDestroyIterator(it);
		return 0;
	}

	list->head= NodeCreate(newObj,it->curr); /*------------insert file at head*/
	SLDestroyIterator(it);
	return 1;
}


int ReInsert(SortedListPtr list, NodePtr newObj)
{
	int compare;
	NodePtr newo;
	SortedListIteratorPtr it;

	//list->funct = compareOcc;
	it = SLCreateIterator(list);
	if (it == NULL){
		SLDestroyIterator(it);
		return 0;
	}
	if (it->prev == NULL || (newObj->count)-(it->curr->count) >= 0) /*--- (n1-n2) , insert DESENDING*/
	{
		//newo = NodeCreate(newObj,it->curr);
		//newo->next = it->curr;
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

unsigned long hash(char* word, int tbSize){
	
	unsigned long hash = 5381;
	int i;
	
	for (i =0; i< strlen(word); i++){
		hash = ((hash << 5)+hash+ word[i]); /*------ hash * 33 + word[i] */
	}
	
	return hash % tbSize;
	
}

void loadTable(SortedListPtr* table, FILE* input, long tl){
	int ret,i;
	unsigned long index;
	size_t len = 0;
	char * end, *word, *file, *line;
	end= word = file= line =NULL;
	SortedListPtr wordList;
	NodePtr currWord;
	
	for(i=0; i<tl;i++){
		table[i]=NULL;
	}
	
	ret = getline(&line, &len, input);
	
	while(ret!=-1){
		word = strndup(line+7,ret-8);  /*---- get word */

		//insert word node
		index = hash(word, tl);			/*----- get hash index */

		if(table[index] == NULL){
			table[index] = SLCreate(compareWords);
		}
		currWord=SLInsert(table[index], (void*)word);
		ret = getline(&line, &len, input);
		
		while(line[0]!='<'){
			end = strrchr(line, ' ');
			file = strndup(line,ret-strlen(end)+1);
			//insert file node
			SLInsert(currWord->fileList, (void*)file);
			ret = getline(&line, &len, input);
		}
		ret = getline(&line, &len, input);
	}
	
	
	
	free(line);
}

int DestroyTable(SortedListPtr* table, long tl){
	int i;
	for (i=0;i<tl;i++){
		if(table[i]){
			SLDestroy(table[i]);
		}
	}
	free(table);

	return 1;
	


}

 

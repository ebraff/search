/*
 * sfunc.c
 */
#include	<string.h>
#include	<stdio.h>
#include	"sfunc.h"
#include	"search.h"

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

/* Added on to initialize struct Node given the object and 
 * the pointer to the next node in the list
 */
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

NodePtr SLInsert(SortedListPtr list, void *newObj)
{
	int compare;
	NodePtr newo;
	SortedListIteratorPtr it;

	newo = NULL;

	it = SLCreateIterator(list);
	if (it == NULL){
		SLDestroyIterator(it);
		return NULL;
	}
	if (it->prev == NULL || (*list->funct)(newObj, it->curr->object) < 0) /*if the iterator's previous is NULL or if the new object is greater than the iterator*/
	{
		newo = NodeCreate(newObj,it->curr);
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
	SLDestroyIterator(it);
	return newo;
}
int SLRemove(SortedListPtr list, void *newObj)
{
	int compare;
	SortedListIteratorPtr it;

	it = SLCreateIterator(list);
	compare = (*list->funct)(newObj, it->curr->object);
	if (it == NULL){
		SLDestroyIterator(it);
		return 0;
	}

	if (it->prev == NULL || compare < 0) /*------------------------if the iterator's previous is NULL, don't do anything*/
	{
		SLDestroyIterator(it);
		return 1;
	}
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
		if(compare <0) /*---------------------------if the new object is less than the iterator, insert it*/
		{
			printf("Could not find object in list.\n");
			SLDestroyIterator(it);
			return 1;
		}
		else if(compare > 0) /*---------------------if the iterator is less than the new object, move the iterator to the next node(s)*/
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
		it->prev->next = it->curr->next; /*---------delete the largest*/
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

SortedListIteratorPtr SLCreateIterator(SortedListPtr list)
{
	SortedListIteratorPtr it = (SortedListIteratorPtr)malloc(sizeof(struct SortedListIterator));
	if (it == NULL)
		return NULL;
	it->curr = list->head;
	it->prev = list->head;
	return it;
}

void SLDestroyIterator(SortedListIteratorPtr iter)
{
	free(iter);
}

void *SLNextItem(SortedListIteratorPtr iter)
{
		iter->prev = iter->curr;
		iter->curr = iter->prev->next;
		return iter;
}

/* hash returns hash index given a word and 
 * the length of the table
 */
unsigned long hash(char* word, long tl){ 
	
	unsigned long hash = 5381;
	int i;
	
	for (i =0; i< strlen(word); i++){
		hash = ((hash << 5)+hash+ word[i]); /*------ hash * 33 + word[i] */
	}
	
	return hash % tl;
	
}

/* loadTable creates a hash table (type SortedListPtr*)
 * Given FILE* created by indexer program and table length
 */
void loadTable(SortedListPtr* table, FILE* input, long tl){
	int ret,i;
	unsigned long index;
	size_t len = 0;
	char * end, *word, *file, *line;
	end= word = file= line =NULL;
	NodePtr currWord,currFile;
	
	for(i=0; i<tl;i++){
		table[i]=NULL;
	}
	
	ret = getline(&line, &len, input);
	
	while(ret!=-1){
		word = strndup(line+7,ret-8); /*-------get word */

		/*insert word node*/
		index = hash(word, tl);/*--------------get hash index */

		if(table[index] == NULL){
			table[index] = SLCreate(compareWords);
		}
		currWord=SLInsert(table[index], (void*)word);
		currWord->fileList=SLCreate(compareWords);
		ret = getline(&line, &len, input);
		
		while(line[0]!='<'){
			end = strrchr(line, ' ');
			file = strndup(line,ret-strlen(end)+1);
			/*insert file node*/
			currFile=SLInsert(currWord->fileList, (void*)file);
			currFile->fileList=NULL;
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


NodePtr getNode(SortedListPtr* table, char* word, long tl){
	unsigned long pos;
	NodePtr found;
	SortedListIteratorPtr it;
	int compare;
	
	it=NULL;		
	pos = hash(word, tl); /*get table number*/
	if(table[pos])
	{
		it = SLCreateIterator(table[pos]);
	}
	if(it == NULL)
	{
		return NULL;
	}
	if(it->prev == NULL)
	{
		SLDestroyIterator(it);
		return NULL;
	}
	
	/*get next node in table[i] (a SortedListPtr)*/	
	while(it->curr != NULL)
	{
		compare = (table[pos]->funct)(word, it->curr->object);
		if (compare == 0)
		{
			found = it->curr;
			SLDestroyIterator(it);
			return found;
		}
		it = SLNextItem(it);
	}
	SLDestroyIterator(it);
	return NULL;
	
 }
 
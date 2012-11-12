/* 
 * search.c
 */
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "sfunc.h"

/*string compare function for sorted list*/
int compareWords(void* word1, void * word2)
{
	int i;	
	char *s1, *s2;
	s1 =(char*)word1;
	s2 = (char*)word2;
	i =strcmp(s1, s2);
	return i;
}

/*sa -- AND function for search*/
SortedListPtr sand(SortedListPtr* table, long tl, char *searchWords)
{
	SortedListPtr final;
	SortedListIteratorPtr it0, it1, it2;
	NodePtr wordNode, fileNode;
	char **wordPtr, *currentWord, *fileName;

	it1 = NULL, it2 = NULL;
	wordPtr = &searchWords;
	wordNode=NULL;
	final=NULL;
	currentWord = strtok_r(searchWords, " \n",wordPtr);	
	while(wordNode==NULL && currentWord!=NULL){
		wordNode = getNode(table, currentWord, tl); /*------------------get the node inside of the hash table*/
		if(!wordNode)
		{
			printf("Cannot find word: %s\n", currentWord);
		}
		currentWord = strtok_r(searchWords, " \n",wordPtr);/*-----------get the next word*/
	}
	
	if (wordNode != NULL)
	{
		final = SLCreate(compareWords);
		it0 = SLCreateIterator(wordNode->fileList);
		while (it0->curr != NULL) /*------------------------------------put all of the file names from the first word into the final list*/
		{
			fileName= strdup((char*)it0->curr->object);
			fileNode=SLInsert(final, (void*)fileName);
			fileNode->fileList=NULL;
			it0 = SLNextItem(it0);
		}
		SLDestroyIterator(it0);
		while (currentWord != NULL)
		{
			wordNode = getNode(table, currentWord, tl);
			if (wordNode != NULL)
			{	
				it1 = SLCreateIterator(final);
				it2 = SLCreateIterator(wordNode->fileList);
				while(it1->curr != NULL || it2->curr !=NULL) /*-while either list is not NULL*/
				{
					if (!final->head || it1->curr == NULL)/*we've reached the end of the final list*/
					{
						break;
					}
					if (it2->curr == NULL)/*----------------we've reached the end of the current word's list*/
					{
						it1 = SLNextItem(it1);
						SLRemove(final, it1->prev->object);
					}
					else if (strcmp(((char*)it1->curr->object),((char*)it2->curr->object)) == 0) /*the two current files are equal*/
					{
						it1 = SLNextItem(it1);
						it2 = SLNextItem(it2);
					}
					else if (strcmp(((char*)it1->curr->object),((char*)it2->curr->object)) < 0) /*the current word's iterator hit a file that is greater than the final list's iterator*/
					{
						it1 = SLNextItem(it1);
						SLRemove(final, it1->prev->object);
					}
					else /*---------------------------------it1 < it2 ; word in final list less than current word*/
					{
						it2 = SLNextItem(it2);
					}
				}
			}		
			else
			{
				printf("Cannot find word: %s\n", currentWord);
			}
			currentWord = strtok_r(searchWords, " \n",wordPtr);/*---get the next word*/
			SLDestroyIterator(it1);
			SLDestroyIterator(it2);	
		}
	
		return final;
	}
	else
	{
		return NULL;
	}
}

/*so -- OR function for search*/
SortedListPtr sor(SortedListPtr* table, long tl, char *searchWords)
{
	SortedListPtr final;
	SortedListIteratorPtr it0, it1, it2;
	NodePtr wordNode, fileNode;
	char **wordPtr, *currentWord, *fileName;

	it1 = NULL, it2 = NULL;
	wordPtr = &searchWords;
	wordNode=NULL;
	final=NULL;
	currentWord = strtok_r(searchWords, " \n",wordPtr);	
	while(wordNode==NULL && currentWord!=NULL){
		wordNode = getNode(table, currentWord, tl); /*------------------get the node inside of the hash table*/
		if(!wordNode){
			printf("Cannot find word: %s\n", currentWord);
		}
		currentWord = strtok_r(searchWords, " \n",wordPtr);/*-----------get the next word*/
	}
	
	if (wordNode != NULL)
	{
		final = SLCreate(compareWords);
		it0 = SLCreateIterator(wordNode->fileList);
		while (it0->curr != NULL) /*------------------------------------put all of the file names from the first word into the final list*/
		{
			fileName= strdup((char*)it0->curr->object);
			fileNode=SLInsert(final, (void*)fileName);
			fileNode->fileList=NULL;
			it0 = SLNextItem(it0);
		}
		SLDestroyIterator(it0);
		while (currentWord != NULL)
		{
			wordNode = getNode(table, currentWord, tl);
			if (wordNode != NULL)
			{
				it1 = SLCreateIterator(final);
				it2 = SLCreateIterator(wordNode->fileList);
				while(it2->curr !=NULL)/*-----------------------while the current word's file list is not NULL*/
				{
					if (!final->head || it1->curr == NULL)/*we've reached the end of the final list*/
					{
						fileName= strdup((char*)it2->curr->object);
						fileNode=SLInsert(final, (void*)fileName);
						fileNode->fileList=NULL;
						it2 = SLNextItem(it2);
					}
					 else if (strcmp(((char*)it1->curr->object),((char*)it2->curr->object)) == 0) /*the two current files are equal*/
					{
						it1 = SLNextItem(it1);
						it2 = SLNextItem(it2);
					}
					else if (strcmp(((char*)it1->curr->object),((char*)it2->curr->object)) > 0) /*the current word's iterator hit a file that is greater than the final list's iterator*/
					{
						fileName= strdup((char*)it2->curr->object);
						fileNode=SLInsert(final, (void*)fileName);
						fileNode->fileList=NULL;
						it2 = SLNextItem(it2);
					}
					else /*---------------------------------it1 < it2 ; word in final list less than current word*/
					{
						it1 = SLNextItem(it1);
					}
				}
			}		
			else
			{
				printf("Cannot find word: %s\n", currentWord);
			}
			SLDestroyIterator(it1);
			SLDestroyIterator(it2);
			currentWord = strtok_r(searchWords, " \n",wordPtr);
		}		
		return final;
	}
	else
	{
		return NULL;
	}	
}

int main(int argc, char **argv)
{
	FILE *input;
	long tableLen;
	int ret;
	size_t len;
	char *searchType, *searchWords;
	SortedListIteratorPtr it;
	SortedListPtr *wordTable, answer;

	if (argc < 2){
		printf("Invalid number of arguments! Please enter a file name to read.");
		return -1;
		}
	len = 0;
	answer = NULL;
	input = fopen(argv[1],"r");
	if(!input){
		printf("Error opening file!!\n");
		return -1;
	}
	searchType = (char*)malloc(10);
	searchWords = NULL;
	tableLen = 797;	
	wordTable = (SortedListPtr*)malloc(sizeof(SortedListPtr)*tableLen);
	
	if(wordTable==NULL){
		fprintf(stderr,"Malloc Fail");
		return -1;
	}

	loadTable(wordTable, input, tableLen);

	/*-----SEARCH MENU-----*/
	
	do
	{	
		printf("search> ");
		scanf("%s", searchType);
		if(strcmp(searchType, "q") == 0) /*-----------told to quit*/
		{
			break;
		}
		if (strcmp(searchType, "sa") != 0 && strcmp(searchType, "so") !=0)
		{
			printf("Error: Not a search command.\n");
			while (getc(stdin) != '\n') {
        		/* do nothing */
   			 }
			continue;
		}			
		if(getc(stdin)=='\n'){ /*---------------------second word is just a new line*/
			printf("No words entered!\n");
			continue;
		}
		ret = getline(&searchWords, &len, stdin);
		searchWords[strlen(searchWords)-1]='\0'; /*---remove \n from input */
		if (ret <2){
			printf("No words entered!\n");
			continue;
		}
		
/*--------Search AND---------*/

		if(strcmp(searchType, "sa") == 0)
		{
			answer = sand(wordTable, tableLen, searchWords);
			/*print the results*/
			if(answer && answer->head)
			{
				it = SLCreateIterator(answer);
				while (it->curr != NULL) 
				{
					printf("%s\n", (char*)it->curr->object);
					it = SLNextItem(it);
				}
				SLDestroyIterator(it);
			}
			else
			{
				printf("Your search returned no files!\n");
			}
			if (answer){
				SLDestroy(answer);				
			}
		}

/*--------Search OR---------*/

		else if (strcmp(searchType, "so") == 0)
		{
			answer = sor(wordTable, tableLen, searchWords);
			/*print the results*/
			if(answer && answer->head)
			{
				it = SLCreateIterator(answer);
				while (it->curr != NULL) 
				{
					printf("%s\n", (char*)it->curr->object);
					it = SLNextItem(it);
				}
				SLDestroyIterator(it);
			}
			else{
				printf("Your search returned no files!\n");
			}
			if (answer){
				SLDestroy(answer);
			}
		}

	} while(1);

	if(searchWords)
	{
		free(searchWords);
	}	
	DestroyTable(wordTable,tableLen);
	free(searchType);
	fclose(input);
	
	return 0;
}

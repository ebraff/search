/* 
 * search.c
 */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "sfunc.h"


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
		wordNode = getNode(table, currentWord, tl);
		currentWord = strtok_r(searchWords, " \n",wordPtr);
	}
	
	if (wordNode != NULL)
	{
		final = SLCreate(compareWords);
		it0 = SLCreateIterator(wordNode->fileList);
		while (it0->curr != NULL)
		{
			fileName= strdup((char*)it0->curr->object);
			fileNode=SLInsert(final, (void*)fileName);
			fileNode->fileList=NULL;
			it0 = SLNextItem(it0);
		}
		SLDestroyIterator(it0);
		while (currentWord != NULL)
		{
			if(final->head == NULL)
			{
				return NULL;
			}
			else
			{
				wordNode = getNode(table, currentWord, tl);
				if (wordNode != NULL)
				{
					it1 = SLCreateIterator(final);
					it2 = SLCreateIterator(wordNode->fileList);
					while(it1->curr != NULL || it2->curr !=NULL)
					{
						if (!final->head || it1->curr == NULL)
						{
							
							return final;
						}
						if (it2->curr == NULL)
						{
							it1 = SLNextItem(it1);
							SLRemove(final, it1->prev->object);
						}
						else if (strcmp(((char*)it1->curr->object),((char*)it2->curr->object)) == 0)
						{
							it1 = SLNextItem(it1);
							it2 = SLNextItem(it2);						
						}
						else if (strcmp(((char*)it1->curr->object),((char*)it2->curr->object)) < 0)
						{
							it1 = SLNextItem(it1);
							SLRemove(final, it1->prev->object);
						}
						else
						{
							it2 = SLNextItem(it2);
						}
					}
				}		
				else
				{
					printf("Cannot find word: %s\n", currentWord);
				}
				currentWord = strtok_r(searchWords, " \n",wordPtr);
			}
		}

		SLDestroyIterator(it1);
		SLDestroyIterator(it2);
	}
	return final;
}

/*so -- OR function for search*/
SortedListPtr sor(SortedListPtr* table, long tl, char *searchWords)
{
	SortedListPtr final;
	char* fullList, *tempList, *currWord;
	fullList=tempList=currWord=NULL;
	
	fullList=searchWords;
	printf("%s\n",fullList);
	tempList=strchr(fullList,' ');
	
	if (tempList){
		currWord= strndup(fullList,strlen(fullList)-strlen(tempList));
		
		fullList=tempList;
		printf("%s",currWord);
		free(currWord);
	}
	
	
	
	
	return NULL;
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
	
	do{	
		printf("search> ");
		scanf("%s", searchType);
		
		if(strcmp(searchType, "q") == 0){
			break;
		}
		if(getc(stdin)=='\n'){
			printf("No words entered!\n");
			continue;
		}
		ret = getline(&searchWords, &len, stdin);

		searchWords[strlen(searchWords)-1]='\0'; /*-- maybe realloc? */
		if (ret <2){
			printf("No words entered!\n");
			continue;
		}
		
		
		if(strcmp(searchType, "sa") == 0)
		{
			answer = sand(wordTable, tableLen, searchWords);
			if(answer!=NULL){
				it = SLCreateIterator(answer);
				while (it->curr != NULL)
				{
					printf("%s\n", (char*)it->curr->object);
					it = SLNextItem(it);
				}
			SLDestroyIterator(it);
			SLDestroy(answer);
			}
			else{
				printf("Your search returned no files!\n");
			}
		}
		else if (strcmp(searchType, "so") == 0)
		{
			sor(wordTable, tableLen, searchWords);
		}
		else
		{
			printf("Error: Not a search command.\n");
		}
	}while(1);

	if(searchWords){
		free(searchWords);
	}
	
	DestroyTable(wordTable,tableLen);
	free(searchType);
	fclose(input);
	
	return 0;
}


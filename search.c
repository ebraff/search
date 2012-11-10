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
	NodePtr wordNode;
	char **wordPtr, *currentWord;
	
	final = SLCreate(compareWords);
	it1 = NULL, it2 = NULL;
	wordPtr = &searchWords;
	currentWord = strtok_r(searchWords, " \n",wordPtr);
	wordNode = getNode(table, currentWord, tl);
	if (wordNode != NULL)
	{
		it0 = SLCreateIterator(wordNode->fileList);
		while (it0->curr != NULL)
		{
			SLInsert(final, it0->curr->object);
			it0 = SLNextItem(it0);
		}
		SLDestroyIterator(it0);
		
		currentWord = strtok_r(searchWords, " \n",wordPtr);	
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
						/*printf("FINAL'S FILE: %s\n", (char*)it1->curr->object);
						printf("WORD'S FILE: %s\n", (char*)it2->curr->object);*/
						if (it1== NULL || it1->curr == NULL)
						{
							break;
						}
						if (it2->curr == NULL)
						{
							/*printf("FINAL'S FILE: %s\n", (char*)it1->curr->object);*/
							SLRemove(final, it1->curr->object);
							break;				
						}
						else if (strcmp(((char*)it1->curr->object),((char*)it2->curr->object)) == 0)
						{
							it1 = SLNextItem(it1);
							it2 = SLNextItem(it2);						
						}
						else if (strcmp(((char*)it1->curr->object),((char*)it2->curr->object)) < 0)
						{
							SLRemove(final, it1->curr->object);				
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
	/*SortedListPtr final;*/
	
	return NULL;
}

int main(int argc, char **argv)
{
	FILE *input;
	long tableLen;
	size_t len;
	char *searchType, *searchWords;
	SortedListIteratorPtr it;
	SortedListPtr *wordTable, answer;
	
	if (argc < 2)
	{
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
	
	if(wordTable==NULL)
	{
		fprintf(stderr,"Malloc Fail");
		return -1;
	}

	loadTable(wordTable, input, tableLen);

	/*-----SEARCH MENU-----*/
	printf("search> ");
	scanf("%s", searchType);
	while(strcmp(searchType, "q") != 0)
	{	
		getline(&searchWords, &len, stdin);
		if(strcmp(searchType, "sa") == 0)
		{
			answer = sand(wordTable, tableLen, searchWords);
			it = SLCreateIterator(answer);
			while (it->curr != NULL)
			{
				printf("%s\n", (char*)it->curr->object);
				it = SLNextItem(it);
			}
			SLDestroyIterator(it);
			SLDestroy(answer);
		}
		else if (strcmp(searchType, "so") == 0)
		{
			sor(wordTable, tableLen, searchWords);
		}
		else
		{
			printf("Error: Not a search command.\n");
		}
		printf("search> ");
		scanf("%s", searchType);

	}
	if(searchWords)
	{
		free(searchWords);
	}
	/*if(answer)
	{
		SLDestroy(answer);
	}*/
	DestroyTable(wordTable,tableLen);
	free(searchType);
	fclose(input);	
	return 0;
}


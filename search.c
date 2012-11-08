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

/*int compareOcc(void* num1, void*  num2)
{
	int n1 = *(int*)num1;
	int n2 = *(int*)num2;
	return n1 - n2;

}*/

int compareFiles(void* word1, void * word2)
{
	int i;

	char *s1, *s2;
	s1 = word1;
	s2 = word2;	
	i =strcmp(s1, s2);
	return i;

}

/*sa -- AND function for search*/

SortedListPtr sand(SortedListPtr* table, long tl, char *searchWords)
{
	/*SortedListPtr final;*/
	char *currentWord;
	NodePtr currentNode;
	currentWord = NULL;
	while (scanf("%s", currentWord) != EOF)
	{
		printf("%s\n", currentWord);
		currentNode = getNode(table, currentWord, tl);
		if (currentNode != NULL)
		{
			printf("%s\n", (char*)(currentNode->object));
		}
		else
		{
			printf("Cannot find word: %s.\n", currentWord);
		}
	}
	return NULL;
}

/*so -- OR function for search*/
SortedListPtr sor(SortedListPtr* table, long tl, char *searchWords)
{
	/*SortedListPtr final;*/
	
	return NULL;
}

int main(int argc, char **argv)
{
	if (argc < 2){
		printf("Invalid number of arguments! Please enter a file name to read.");
		return -1;
		}
	FILE* input = fopen(argv[1],"r");
	long tableLen;
	size_t len =0;
	char * searchType, *searchWords;
	searchType = (char*)malloc(10);
	searchWords = NULL;
	/*NodePtr testNode;*/
	
	tableLen = 797;
	
	SortedListPtr* wordTable = (SortedListPtr*)malloc(sizeof(SortedListPtr)*tableLen);
	if(wordTable==NULL){
		fprintf(stderr,"Malloc Fail");
		return -1;
	}
	
	loadTable(wordTable, input, tableLen);
	

	/*-----SEARCH MENU-----*/
	/*searchType = (char*)malloc(4);*/
	printf("search> ");
	scanf("%s", searchType);
	while(strcmp(searchType, "q") != 0)
	{	
		getline(&searchWords, &len, stdin);
		if(strcmp(searchType, "sa") == 0)
		{
			sand(wordTable, tableLen, searchWords);
		}
		else if (strcmp(searchType, "so") == 0)
		{
			/*sor(wordTable, tableLen);*/
		}
		else
		{
			printf("Error: Not a search command.\n");
		}
		printf("search> ");
		scanf("%s", searchType);

	}

	if(searchWords){
		free(searchWords);
	}
printf("%s\n",searchType);	
	
	DestroyTable(wordTable,tableLen);
	fclose(input);
	
	return 0;
}


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

int compareOcc(void* num1, void*  num2)
{
	int n1 = *(int*)num1;
	int n2 = *(int*)num2;
	return n1 - n2;

}

int compareFiles(void* word1, void * word2)
{
	int i;

	char *s1, *s2;
	s1 = word1;
	s2 = word2;	
	i =strcmp(s1, s2);
	return i;

}

int main(int argc, char **argv)
{
	FILE* input = fopen(argv[1],"r");
	long tableLen;
	
	tableLen = 797;
	
	SortedListPtr* wordTable = (SortedListPtr*)malloc(sizeof(SortedListPtr)*tableLen);
	if(wordTable==NULL){
		fprintf(stderr,"Malloc Fail");
		return -1;
	}
	
	loadTable(wordTable, input, tableLen);
	
	/*---- SEARCH MENU */
	
	DestroyTable(wordTable,tableLen);
	fclose(input);

	
	
	
	
	return 0;
}


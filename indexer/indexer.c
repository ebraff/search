/* Coded by Brian Battell, Lara Martin, and Elana Braff*/

/* indexer.c*/

#include	<string.h>
#include	<stdio.h>
#include	<dirent.h>
#include	<sys/stat.h>
#include	"indexer.h"
#include	"sorted-list.h"
#include	"tokenizer.h"


SortedListPtr globalList;

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

int walkDir(char* name){ /*---------------------------------take in SL also*/
	DIR* dr;
	struct stat statbuf;
	struct dirent *fname;
	char* newPath;
	char* word;
	char* file;
	char* token;
	TokenizerT * tk;

	if(stat(name,&statbuf)==-1){
		printf("Error accessing file or Directory %s\n",name);
		return -1;
	}

	if(S_ISDIR(statbuf.st_mode)){

		dr =opendir(name);
		if(!dr){
			printf("Cannot open Directory %s\n",name);
			return -1;
		}
		printf("Directory Name: %s\n", name);
		while ((fname=readdir(dr))!=NULL){
			if(fname->d_name[0]=='.' || fname->d_name[strlen(fname->d_name)-1]=='~'){ 	/*-------------------Skips hidden files and '.' '..' Directories and those ending with ~*/
				continue;
			}
			else{
				newPath=(char*)malloc(strlen(name)
				+strlen(fname->d_name)+2);   
				sprintf(newPath, "%s/%s", name, fname->d_name);
				walkDir(newPath); /*-------------------------Pass SL also*/
				free(newPath);
			}
		}
		closedir(dr);
	}
	else if(S_ISREG(statbuf.st_mode)){
			printf("FileName: %s\n", name);
			/*Tokenize and add words to list*/
			tk = run(name);
			token = TKGetNextToken(tk);
			
			while(token!= NULL) {
				word = (char*)malloc(strlen(token)+1); /*--- word */
				strcpy(word,token);
				file = (char*)malloc(strlen(name)+1);	/*---- fileName */
				strcpy(file,name);
				SLInsert(globalList, word, file);
				free(token);
				token = TKGetNextToken(tk);
			}
			TKDestroy(tk);
	}

	return 1;
}

int main(int argc, char** argv)
{
	/*dest is argv[1]*/
	/*source files are argv[2]*/
	NodePtr curr, flist, temp;
	char* word;
	char* file;
	char* path;
	FILE *output;

	if (argc != 3)
	{
		printf("Invalid number of arguments.\n");
		return -1;
	}
	
	path = (char*)malloc(strlen(argv[2])+1);
    strcpy(path,argv[2]);
    
    if(path[strlen(path)-1]=='/'){
       path[strlen(path)-1]='\0';
    }

	globalList = SLCreate(compareWords);
	walkDir(path);
	free(path);
	curr = globalList->head;
	output = fopen(argv[1],"a+"); /*append file (add text to a file or create a file if it does not exist.*/

	while(curr != NULL) /*----------prints the list*/
		{
			word=(char*)curr->object;
			fprintf(output, "<list> %s\n",word);

			flist = curr->fileList->head;
			while(flist!=NULL){
				file= (char*)flist->object;
				fprintf(output, "%s %i\n",file,flist->count);
				
				temp=flist;
				flist = flist->next;

				}
			fprintf(output, "</list>\n");
			curr = curr->next;
	
		}
	fclose(output);
	SLDestroy(globalList);
	return 0;
}

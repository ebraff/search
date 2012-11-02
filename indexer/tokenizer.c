/*
 * tokenizer.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tokenizer.h"

#define MAX_HEX_CHARS 2
#define MAX_OCT_CHARS 3

/*
 * TKCreate creates a new TokenizerT object for a given set of separator
 * characters (given as a string) and a token stream (given as a string).
 * 
 * TKCreate should copy the two arguments so that it is not dependent on
 * them staying immutable after returning.  (In the future, this may change
 * to increase efficiency.)
 *
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */

TokenizerT *TKCreate(char *ts) {

	/*
	 * Description: creates a new tokenizer struct from the token stream and delimiters
	 * Parameters: set of delimiters, token stream
	 * Modifies: nothing
	 * Returns: a pointer to a tokenizer struct on success, a null pointer on failure
	 * 
	 */
	FILE * temp;
	TokenizerT* tokenizer;

	if(ts == NULL){
		return NULL;
	}

	tokenizer = (TokenizerT*)malloc(sizeof(TokenizerT));

	if(tokenizer == NULL){
		return NULL;
	}

	temp = fopen(ts,"r");
	if(temp == NULL)
	{
		printf("Cannot read file.\n");
		return NULL;
	} tokenizer->filename = temp; tokenizer->current_position = 0;

	return tokenizer;
}

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
 */

void TKDestroy(TokenizerT *tk) {	

	/*
	 * Description: destroys tokenizer struct and deallocates all memory
	 * Parameters: tokenizer to be destroyed
	 * Modifies: tokenizer struct: deallocates memory
	 * Returns: nothing 
	 */
	fclose(tk->filename);
	free(tk);

	return;
}


/*
 * TKGetNextToken returns the next token from the token stream as a
 * character string.  Space for the returned token should be dynamically
 * allocated.  The caller is responsible for freeing the space once it is
 * no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

char *TKGetNextToken(TokenizerT *tk) {
	/*
	 * Description: returns the next token from the token stream specified within the tokenizer
	 * Parameters: tokenizer from which to extract token
	 * Modifies: tokenizer->current_position: identifies starting point of next token; creates a new string with 
	 * Returns: token extracted as a char* on success, null on failure/end of string;
	 */

	char* token = NULL;
	fpos_t token_pos;
    int length=0;
	char current;
	int i;

	/* Checks each char until it finds the first alnum OR EOF */
	fgetpos(tk->filename,&token_pos); /*gets position in file*/
	while((current = fgetc(tk->filename))!=EOF) {
		if(isalnum(current)) {
			length++;
			break;
		}
		tk->current_position++;
        fgetpos(tk->filename,&token_pos);
	}

	if(current== EOF) {
		return NULL;
	}

	/*finds the first delimeter*/
	while(current != EOF) {
		current = fgetc(tk->filename);
		if(!isalnum(current)) {
			break;
		}else{
            length++;
        }
		tk->current_position++;
	}
	
	/*creates token*/
    token = (char*)malloc(length + 1);
    fsetpos(tk->filename,&token_pos);
	fgets(token, length+1,tk->filename);
	for (i = 0; i < length; i++)
	{
		token[i] = tolower(token[i]);
	}
	return token;
}

/*
 * main will have two string arguments (in argv[1] and argv[2]).
 * The first string contains the separator characters.
 * The second string contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */

TokenizerT * run(char * filename) {

	TokenizerT* tokenizer = TKCreate(filename);

	if(tokenizer == NULL) {
		printf("Error: unable to create tokenizer\n");
		return NULL;
	}
	return tokenizer;
}

#ifndef TOKENIZER_H
#define TOKENIZER_H
/*
 * tokenizer.h
 */

#include <stdlib.h>

struct TokenizerT_ {
	FILE* filename;	
	long current_position;
};
typedef struct TokenizerT_ TokenizerT;


int char_to_hex(char character);
 
int char_to_oct(char character);
 
int is_oct_digit(char oct_digit);

char* unescape_string(char* string);

TokenizerT *TKCreate(char *ts);

void TKDestroy(TokenizerT *tk);

char *TKGetNextToken(TokenizerT *tk);

TokenizerT * run(char * filename);


#endif

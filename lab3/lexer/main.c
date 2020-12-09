#include "token.h"
#include "preprocess.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN_BUFFER 128

/*
Generates lexemes for the given C file after preprocessing it. 
*/
int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage: arg1 -> input c file");
		exit(EXIT_FAILURE);
	}

	char *path;

	// Preprocessing the input c file:
	path = preprocess(argv[1]);

	// Identify tokens and output to stdout
	FILE *fin = fopen(path, "r");
	Token *token;

	//Get just the first lexeme
	token = getNextToken(fin);
	displayToken(token);

	fclose(fin);
	return 0;
}

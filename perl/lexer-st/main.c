#include "token.h"
#include "st.h"
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

	FILE *fin = fopen(argv[1], "r");
	if (fin == NULL)
	{
		fprintf(stderr, "Could not open processed file %s", argv[1]);
		perror(" ");
		exit(EXIT_FAILURE);
	}

	Token *token;

	//Get all lexemes
	while (1)
	{
		token = getNextToken(fin);
		displayToken(token);

		if (token->type == END_OF_FILE)
			break;
	}

	displayAllLocalSymbolTables();
	fclose(fin);
	return 0;
}

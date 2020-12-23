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
	
	FILE *fin = fopen("test.txt", "r");
	if (fin == NULL)
	{
		fprintf(stderr, "Could not open processed file %s", "test.txt");
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

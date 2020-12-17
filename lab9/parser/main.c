#include "token.h"
#include "st.h"
#include "preprocess.h"
#include "rdp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN_BUFFER 128

/*
Runs a recursive descent parser for the subset of the C grammar from Q7.1. Uses lexer-st from lab4. 
*/

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage: arg1 -> input c file");
		exit(EXIT_FAILURE);
	}

	const char *path;

	// Preprocessing the input c file:
	path = preprocess(argv[1]);

	fin = fopen(path, "r");
	if (fin == NULL)
	{
		fprintf(stderr, "Could not open processed file %s", path);
		perror(" ");
		exit(EXIT_FAILURE);
	}

  parse(fin);
  displayAllLocalSymbolTables();
	fclose(fin);
	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_LENGTH 120
/*
	Takes a c source file as input, recognizes all keywords and prints them in upper
	case along with their corresponding line and column numbers. 
	Identifies the 32 ANSI C keywords.
*/

static char keywords[32][10] = {"auto", "break", "case", "char", "const","continue","default", "double", "enum","extern","float", "for", "goto", "if", "int", "long", "register", "return", "short", "signed", "sizeof", "static", "struct", "switch", "typeof", "union", "unsigned", "void", "volatile", "while", "do", "else"};

int isKeyword ( char str[] );
void toUpper ( char str[] );

int main( int argc, char *argv[] ){
	
	if( argc != 2){
		fprintf(stderr, "Usage: source c file as sole input\n");
		exit(0);	
	}

	FILE *fin;
	char line[BUFFER_LENGTH], *token;
	int line_no = 0, col_no = 0;
 
	fin = fopen(argv[1], "r");
	if(fin == NULL){
		fprintf(stderr, "Couldn't open input c source file\n");
		exit(1);
	}
	
	//Read line by line.
	while ( fgets(line, BUFFER_LENGTH, fin) != NULL ){
		line_no++;	
		col_no = 0;
		//Read each word in the line. 
		token = strtok(line, " ");
		while ( token ) {
			if( isKeyword(token) ){
				toUpper(token);
				printf("line: %d, col: %d, keyword: %s\n", line_no, col_no, token);
			}
			col_no +=  strlen(token) + 1;
			token = strtok(NULL, " ");
		}
	}
	
	fclose(fin);	
	return 0;
}

int isKeyword( char str[] ){
	for( int i = 0; i < 32 ; i++ ) 
		if(strcmp(str, keywords[i]) == 0) return 1; 

	return 0;
}

void toUpper ( char str[] ){
	for( int i = 0; str[i]!='\0'; i++ ){
		str[i] -= 32;
	}
}

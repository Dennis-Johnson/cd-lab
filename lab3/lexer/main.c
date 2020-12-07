#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TOKEN_NAME_LENGTH 128
#define TOKEN_TYPE_LENGTH 128

/*
Generates lexemes for the given C file after preprocessing it. 
*/

typedef struct Token {
	int index;
	int row, col;
	char token_name[TOKEN_NAME_LENGTH];
	char type[TOKEN_TYPE_LENGTH];
} Token;

int removeWhitespace(const char inputPath[], const char outputPath[]);
int stripPreprocessorDirectives(const char inputPath[], const char outputPath[]);
int removeComments(const char inputPath[], const char outputPath[]);

Token* createToken();
Token* getNextToken(FILE *fin);
void displayToken(Token* token);

int main(int argc, char* argv[]){
	if(argc != 2){
		fprintf(stderr, "Usage: arg1 -> input c file");
		exit(EXIT_FAILURE);
	}

	// Preprocessing the input c file:
	// 1. Remove whitespace
	const char* removedWhitespacePath	= "./temp/removedWS.i";
	removeWhitespace(argv[1], removedWhitespacePath);
	
	// 2. Strip preprocessor directives
	const char* strippedPPDirectives = "./temp/removedPPD.i";
	stripPreprocessorDirectives(removedWhitespacePath, strippedPPDirectives);
	
	// 3. Remove Comments 
	const char* removedCommentsPath = "./temp/removedComments.i";
	removeComments(strippedPPDirectives, removedCommentsPath); 

	
	// Identify tokens and output to stdout
	FILE *fin = fopen(removedCommentsPath, "r");
	Token* token;
	
	//Get just the first lexeme
	token = getNextToken(fin);
	displayToken(token);
	
	fclose(fin);
	return 0;
}

int removeWhitespace(const char inputPath[], const char outputPath[]){
	int wspace_count = 0, ch;
	FILE *fin, *fout;

	fin = fopen(inputPath, "r");
	fout = fopen(outputPath, "w");
	
	if(fin == NULL){
		fprintf(stderr, "Couldn't open input file %s\n", inputPath);
		exit(EXIT_FAILURE);
	}	
	if(fout == NULL){
		fprintf(stderr, "Couldn't open output file %s\n", outputPath);
		exit(EXIT_FAILURE);
	}	
		
	while((ch = fgetc(fin))!= EOF){
		if(ch == '\t' || ch == ' '){
			wspace_count++;
		} 
		else {	
			if(wspace_count > 0 && ch != '\n')
				fputc(' ', fout);
			
			fputc(ch, fout);
			wspace_count = 0;
		}
	}
	
	fclose(fin);
	fclose(fout);

	return 0;
}


int stripPreprocessorDirectives(const char inputPath[], const char outputPath[]){
	
	int ch, temp;
	FILE *fin, *fout;
	fin = fopen(inputPath, "r");
	fout = fopen(outputPath, "w");
	
	if(fin == NULL){
		fprintf(stderr, "Couldn't open input file %s\n", inputPath);
		exit(EXIT_FAILURE);
	}	
	if(fout == NULL){
		fprintf(stderr, "Couldn't open output file %s\n", outputPath);
		exit(EXIT_FAILURE);
	}	

	while ((ch = fgetc(fin)) != EOF){
		if(ch == '#'){
			//Read and ignore chars till the end of the line. 
			while( (temp = fgetc(fin)) != '\n');
		} else {
			fputc(ch, fout);
		}
	}
	
	fclose(fin);
	fclose(fout);
	return 0;
}

int removeComments(const char inputPath[], const char outputPath[]){
	FILE *fin, *fout;
	int ca, cb;
	
	fin = fopen(inputPath, "r");
	if(fin == NULL){
		fprintf(stderr, "Cannot open input file%s\n", inputPath);
		exit(EXIT_FAILURE);
	}	

	fout = fopen(outputPath, "w");
	if(fout == NULL){
		fprintf(stderr, "Error opening output file%s\n", outputPath);
		exit(EXIT_FAILURE);
	}

	while((ca = fgetc(fin)) != EOF){
		if(ca == ' '){
			fputc(ca, fout);
			while(ca == ' ')
				ca = fgetc(fin);
		}

		if(ca == '/'){
			cb = fgetc(fin);
			
			if(cb == '/'){
				while(ca != '\n')
					ca = fgetc(fin);
				ca = fgetc(fin);
			}
			
			else if(cb == '*'){
				do{
					while(ca != '*')
						ca = fgetc(fin);
					ca = fgetc(fin);
				}while(ca != '/');
			}
			
			else{
					fputc(ca, fout);
					fputc(cb, fout);
			}
		}
		
		else fputc(ca, fout);
	}
	
	fclose(fin);
	fclose(fout);
	return 0;
}

void displayToken(Token* token){
	if(!token){
		fprintf(stderr, "Empty token, cannot display");
	}
	
	printf("Index: %d, Row: %d, Col: %d, Name: %s, Type: %s\n", token->index, token->row, token->col, token->token_name, token->type); 
}

Token* createToken(){
	Token* ptr = malloc(sizeof(Token));
	ptr->index = 0;
	ptr->row =  0;
	ptr->col = 0;
	strncpy(ptr->token_name, "DEFAULT", TOKEN_NAME_LENGTH);
	strncpy(ptr->type,  "DEFAULT", TOKEN_TYPE_LENGTH);

	return ptr;
}

Token* getNextToken(FILE *fin){
	static int index  = 0;
	static int rowNum = 0;
	static int colNum = 0;
	
	Token* token = createToken();
	return token;
}

#define TOKEN_NAME_LENGTH 128
#define TOKEN_TYPE_LENGTH 128
#include <stdio.h>

typedef struct Token
{
  int index;
  int row, col;
  char token_name[TOKEN_NAME_LENGTH];
  char type[TOKEN_TYPE_LENGTH];
} Token;

Token *getNextToken(FILE *fin);
void displayToken(Token *token);

#ifndef TOKEN_H_
#define TOKEN_H_

#include <stdio.h>
#define TOKEN_NAME_LENGTH 128
#define TOKEN_TYPE_LENGTH 128

typedef enum
{
  KEYWORD,
  IDENTIFIER,
  DATA_TYPE,
  FUNCTION,
  STRING_LIT,
  NUM_CONST,
  REL_OP,
  ARTH_OP,
  ASSIGN_OP,
  LOG_OP,
  END_OF_FILE,
  SEMI_COLON,
  LEFT_PAREN,
  RIGHT_PAREN,
  LEFT_SQUARE_BRACKET,
  RIGHT_SQUARE_BRACKET,
  LEFT_CURLY_BRACE,
  RIGHT_CURLY_BRACE,
  AMPERSAND,
  PERIOD,
  COLON,
  COMMA,
  NEW_LINE,
  WHITESPACE
} TokenType;

typedef struct Token
{
  int index;
  int row, col;
  char token_name[TOKEN_NAME_LENGTH];
  TokenType type;
} Token;

char *TokenTypeToString(TokenType t);
Token *getNextToken(FILE *fin);
void displayToken(Token *token);
void displayAngleBrackets(Token *token);
size_t getDataTypeSize(char* dt);

int isDataType(char str[]);
#endif //TOKEN_H_

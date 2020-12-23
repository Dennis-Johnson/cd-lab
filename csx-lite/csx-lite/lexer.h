#ifndef LEXER_H_
#define LEXER_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define LEN_NAME 16
#define NUM_KEYWORDS 1

typedef enum
{
    KEYWORD,
    IDENTIFIER,
    ARTH_OP,
    ASSIGN_OP,
    LEFT_CURLY,
    RIGHT_CURLY,
    LEFT_PAREN,
    RIGHT_PAREN,
    SEMICOLON,
    END_OF_FILE,
    DEFAULT
} TokenType;

typedef struct Token
{
    int row, col;
    char name[LEN_NAME];
    TokenType type;
} Token;

void displayToken(Token *token);
int isKeyword(char *str);
char *typeToString(TokenType t);
Token *getNextToken(FILE *fin);
void ungetToken(Token *token, FILE *fin);

#endif //LEXER_H_

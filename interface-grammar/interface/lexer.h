#ifndef LEXER_H_
#define LEXER_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define LEN_NAME 16
#define NUM_KEYWORDS 10

typedef enum
{
    KEYWORD,
    IDENTIFIER,
    NUMBER,
    LEFT_SQUARE,
    RIGHT_SQUARE,
    LEFT_CURLY,
    RIGHT_CURLY,
    COMMA,
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
Token *getNextToken(FILE *fin);
void ungetToken(Token *token, FILE *fin);

#endif //LEXER_H_

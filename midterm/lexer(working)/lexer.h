#ifndef LEXER_H_
#define LEXER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define LEN_INPUT_BUFFER 2046
#define LEN_NAME 32

typedef enum
{
    KEYWORD,
    IDENTIFIER,
    DATA_TYPE,
    NUMBER,
    STRING,
    COLON,
    SEMICOLON,
    DOT,
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_CURLY,
    RIGHT_CURLY,
    END_OF_FILE,
    ADD_OP,
    ARROW,
    DEFAULT,
    WHITESPACE,
    NEW_LINE
} TokenType;

char *TokenStrings[] = {
    "KEYWORD",
    "IDENTIFIER",
    "DATA_TYPE",
    "NUMBER",
    "STRING",
    "COLON",
    "SEMICOLON",
    "DOT",
    "LEFT_PAREN",
    "RIGHT_PAREN",
    "LEFT_CURLY",
    "RIGHT_CURLY",
    "END_OF_FILE",
    "ADD_OP",
    "ARROW",
    "DEFAULT",
    "WHITESPACE",
    "NEW_LINE"};

typedef struct Token
{
    int row, col;
    char name[LEN_NAME];
    TokenType type;
} Token;

extern char input_buffer[LEN_INPUT_BUFFER];
extern int ip_indx;
extern FILE *fin;
extern char *TokenStrings[];

Token *getNextToken(FILE *fin);
void displayToken(Token *token);
char *tokenTypeString(TokenType type);

#endif //LEXER_H_

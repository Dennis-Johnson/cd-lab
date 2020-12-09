#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define LEN_BUFFER 128
int isKeyword(char str[]);

typedef enum
{
    KEYWORD,
    IDENTIFIER,
    NUM_CONST,
    END_OF_FILE,
    NEW_LINE,
    WHITESPACE
} TokenType;

static char *strings[] = {
    "KEYWORD",
    "IDENTIFIER",
    "NUM_CONST",
    "END_OF_FILE",
    "NEW_LINE",
    "WHITESPACE"};

char *TokenTypeToString(TokenType t)
{
    return strings[t];
}

Token *initToken(TokenType type, char *value, int index, int row, int col)
{
    Token *ptr = malloc(sizeof(Token));
    ptr->index = index;
    ptr->row = row;
    ptr->col = col;
    strncpy(ptr->token_name, value, TOKEN_NAME_LENGTH);
    strncpy(ptr->type, TokenTypeToString(type), TOKEN_TYPE_LENGTH);
    return ptr;
}

Token *getNextToken(FILE *fin)
{
    static int index = 0;
    static int rowNum = 0;
    static int colNum = 0;

    TokenType type;

    // Buffer to accumulate multi-char lexemes
    char buffer[LEN_BUFFER];
    memset(buffer, '\0', sizeof(buffer));

    int ch, buf_index = 0;
    ch = fgetc(fin);
    colNum++;

    if (ch == EOF)
        type = END_OF_FILE;
    else if (ch == '\n')
    {
        type = NEW_LINE;
        rowNum++;
    }
    else if (isspace(ch))
    {
        type = WHITESPACE;
    }
    else if (isalpha(ch))
    {
        while (isalpha(ch))
        {
            buffer[buf_index++] = ch;
            ch = fgetc(fin);
        }
        // At this point, you probably hit the whitespace after the word

        if (isKeyword(buffer))
            type = KEYWORD;
        else
            type = IDENTIFIER;
    }
    else if (isdigit(ch))
    {
        type = NUM_CONST;
        while (isdigit(ch))
        {
            buffer[buf_index++] = ch;
            ch = fgetc(fin);
        }
    }
    // Don't create Tokens for these
    if (type == NEW_LINE || type == WHITESPACE)
        return NULL;

    return initToken(type, buffer, index, rowNum, colNum);
}

void displayToken(Token *token)
{
    if (!token)
    {
        fprintf(stderr, "Empty token, cannot display");
    }

    printf("Index: %d, Row: %d, Col: %d, Name: %s, Type: %s\n", token->index, token->row, token->col, token->token_name, token->type);
}

static char keywords[32][10] = {"auto", "break", "case", "char", "const", "continue", "default", "double", "enum", "extern", "float", "for", "goto", "if", "int", "long", "register", "return", "short", "signed", "sizeof", "static", "struct", "switch", "typeof", "union", "unsigned", "void", "volatile", "while", "do", "else"};
int isKeyword(char str[])
{
    for (int i = 0; i < 32; i++)
        if (strcmp(str, keywords[i]) == 0)
            return 1;

    return 0;
}
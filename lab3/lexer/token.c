#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN_BUFFER 128

Token *createToken()
{
    Token *ptr = malloc(sizeof(Token));
    ptr->index = 0;
    ptr->row = 0;
    ptr->col = 0;
    strncpy(ptr->token_name, "DEFAULT", TOKEN_NAME_LENGTH);
    strncpy(ptr->type, "DEFAULT", TOKEN_TYPE_LENGTH);

    return ptr;
}

Token *getNextToken(FILE *fin)
{
    static int index = 0;
    static int rowNum = 0;
    static int colNum = 0;

    Token *token = createToken();

    char buffer[LEN_BUFFER];
    int ch;

    ch = fgetc(fin);
    if (ch == EOF)

        return token;
}

void displayToken(Token *token)
{
    if (!token)
    {
        fprintf(stderr, "Empty token, cannot display");
    }

    printf("Index: %d, Row: %d, Col: %d, Name: %s, Type: %s\n", token->index, token->row, token->col, token->token_name, token->type);
}

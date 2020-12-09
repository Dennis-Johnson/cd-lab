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
    REL_OP,
    ARTH_OP,
    ASSIGN_OP,
    END_OF_FILE,
    NEW_LINE,
    WHITESPACE
} TokenType;

static char *strings[] = {
    "KEYWORD",
    "IDENTIFIER",
    "NUM_CONST",
    "REL_OP",
    "ARTH_OP",
    "ASSIGN_OP",
    "END_OF_FILE",
    "NEW_LINE",
    "WHITESPACE"};

char *TokenTypeToString(TokenType t)
{
    return strings[t];
}

Token *initToken(TokenType type, char *value, int row, int col)
{
    static int index = 0;
    index++;

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
        colNum = 0;
        rowNum++;
    }
    else if (isspace(ch))
    {
        colNum++;
        type = WHITESPACE;
        while (isspace(ch))
            ch = fgetc(fin);

        fseek(fin, -1L, SEEK_CUR);
    }
    else if (isalpha(ch))
    {
        colNum++;
        while (isalpha(ch))
        {
            colNum++;
            buffer[buf_index++] = ch;
            ch = fgetc(fin);
        }
        fseek(fin, -1L, SEEK_CUR);

        if (isKeyword(buffer))
            type = KEYWORD;
        else
            type = IDENTIFIER;
    }
    else if (isdigit(ch))
    {
        colNum++;
        type = NUM_CONST;
        while (isdigit(ch))
        {
            colNum++;
            buffer[buf_index++] = ch;
            ch = fgetc(fin);
        }
        fseek(fin, -1L, SEEK_CUR);
    }
    else if (ch == '=')
    {
        colNum++;
        buffer[buf_index++] = ch;
        ch = fgetc(fin);
        colNum++;

        if (ch == '=')
        {
            //Relational Operator ==
            buffer[buf_index++] = ch;
            type = REL_OP;
        }
        else
        {
            type = ASSIGN_OP;
            fseek(fin, -1L, SEEK_CUR);
        }
    }
    else if (ch == '<' || ch == '>' || ch == '!')
    {
        type = REL_OP;
        colNum++;
        buffer[buf_index++] = ch;
        ch = fgetc(fin);
        colNum++;

        if (ch == '=')
            buffer[buf_index++] = ch;
        else
            fseek(fin, -1L, SEEK_CUR);
    }
    else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%')
    {
        type = ARTH_OP;
        buffer[buf_index++] = ch;
    }

    return initToken(type, buffer, rowNum, colNum);
}

void displayToken(Token *token)
{
    if (!token)
    {
        fprintf(stderr, "Empty token, cannot display");
        perror(" ");
    }

    if (strcmp(token->type, "NEW_LINE") == 0 || strcmp(token->type, "WHITESPACE") == 0)
        return;

    printf("Index: %d, Row: %d, Col: %d, Name: %s, Type: %s\n", token->index, token->row, token->col, token->token_name, token->type);
}

static char keywords[32][10] = {"auto", "break", "case", "char", "const", "continue", "default", "double", "enum", "extern", "float", "for", "goto", "if", "int", "long", "register", "return", "short", "signed", "sizeof", "static", "struct", "switch", "typeof", "union", "unsigned", "void", "volatile", "while", "do", "else"};
static char dataTypes[][10] = {
    "double",
    "float",
    "int",
    "long",
    "short",
    "char"};

int isKeyword(char str[])
{
    for (int i = 0; i < 32; i++)
        if (strcmp(str, keywords[i]) == 0)
            return 1;

    return 0;
}

int isDataType(char str[])
{
    for (int i = 0; i < 6; i++)
        if (strcmp(str, dataTypes[i]) == 0)
            return 1;
    return 0;
}
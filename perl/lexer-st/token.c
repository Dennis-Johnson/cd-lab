#include "token.h"
#include "st.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define NUM_KEYWORDS 2
#define NUM_DATATYPES 1
#define LEN_BUFFER 64

int isKeyword(char str[]);
int isDataType(char str[]);

char *token_strings[] = {
    "KEYWORD",
    "IDENTIFIER",
    "NUM_CONST",
    "ARTH_OP",
    "ASSIGN_OP",
    "END_OF_FILE",
    "SEMI_COLON",
    "LEFT_PAREN",
    "RIGHT_PAREN",
    "LEFT_CURLY_BRACE",
    "RIGHT_CURLY_BRACE",
    "AT",
    "PERIOD",
    "PLUS_EQUAL",
    "COMMA",
    "NEW_LINE",
    "WHITESPACE", "DEFAULT"};

char *TokenTypeToString(TokenType t)
{
    return token_strings[t];
}

Token *createToken(TokenType type, char *value, int row, int col)
{
    Token *ptr = malloc(sizeof(Token));
    ptr->row = row;
    ptr->col = col;

    // Index is -1 by dafault, only assigned if type is function or identifier
    ptr->index = -1;

    strncpy(ptr->token_name, value, TOKEN_NAME_LENGTH);
    ptr->type = type;

    if (type == IDENTIFIER)
    {
        // Add to the current local symbol table
        // Assign the returned value to the index

        int ret_val = addEntry(ptr);
        if (ret_val < 0)
        {
            printf("Symbol table entry exists for %s\n", ptr->token_name);
        }
        else
            ptr->index = ret_val;
    }
    else if (type == SEMI_COLON)
    {
        memset(data_type_buffer, '\0', sizeof(data_type_buffer));
    }

    return ptr;
}

Token *getNextToken(FILE *fin)
{
    static int rowNum = 0;
    static int colNum = 0;

    TokenType type = DEFAULT;

    // Buffer to accumulate multi-char lexemes
    char buffer[LEN_BUFFER];
    memset(buffer, '\0', sizeof(buffer));

    int ch, buf_index = 0;
    ch = fgetc(fin);

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
        type = WHITESPACE;
        while (isspace(ch))
        {
            colNum++;
            ch = fgetc(fin);
        }
        fseek(fin, -1L, SEEK_CUR);
    }
    else if (ch == '#')
    {
        while (ch != '\n')
        {
            ch = fgetc(fin);
        }
        type = WHITESPACE;
        rowNum++;
        colNum = 0;
    }
    else if (ch == '$')
    {
        type = IDENTIFIER;
        buffer[buf_index++] = ch;
        ch = fgetc(fin);

        while (isalpha(ch))
        {
            buffer[buf_index++] = ch;
            ch = fgetc(fin);
        }
        fseek(fin, -1L, SEEK_CUR);
    }
    else if (isalpha(ch))
    {
        type = KEYWORD;
        while (isalpha(ch))
        {
            colNum++;
            buffer[buf_index++] = ch;
            ch = fgetc(fin);
        }

        fseek(fin, -1L, SEEK_CUR);
    }
    else if (isdigit(ch))
    {
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
        type = ASSIGN_OP;
    }
    else if (ch == '+')
    {
        type = ARTH_OP;
        buffer[buf_index++] = ch;

        ch = fgetc(fin);
        if (ch == '=')
        {
            type = PLUS_EQUAL;
            buffer[buf_index++] = ch;
        }
        else
        {
            fseek(fin, -1L, SEEK_CUR);
        }
    }
    else if (ch == '@')
    {
        colNum++;
        buffer[buf_index++] = ch;
        ch = fgetc(fin);

        if (ch == '_')
        {
            type = AT;
            colNum++;
        }
        else
        {
            fseek(fin, -1L, SEEK_CUR);
            printf("Unrecognized token!\n");
            type = WHITESPACE;
        }
    }
    else if (ch == ';')
    {
        colNum++;
        buffer[buf_index++] = ch;
        type = SEMI_COLON;
    }
    else if (ch == '(' || ch == ')' || ch == '{' || ch == '}')
    {
        colNum++;
        buffer[buf_index++] = ch;
        switch (ch)
        {
        case '(':
            type = LEFT_PAREN;
            break;
        case ')':
            type = RIGHT_PAREN;
            break;
        case '{':
            type = LEFT_CURLY_BRACE;
            break;
        case '}':
            type = RIGHT_CURLY_BRACE;
            break;
        }
    }
    else if (ch == '.')
    {
        colNum++;
        buffer[buf_index++] = ch;
        type = PERIOD;
    }
    else if (ch == ',')
    {
        colNum++;
        buffer[buf_index++] = ch;
        type = COMMA;
    }

    return createToken(type, buffer, rowNum, colNum);
}

void displayToken(Token *token)
{
    if (!token)
    {
        fprintf(stderr, "Empty token, cannot display");
        perror(" ");
    }

    if (token->type == NEW_LINE || token->type == WHITESPACE)
        return;

    printf("Index: %-3d | Row: %-3d | Col: %-3d | Type: %-8s | Name: %-10s\n", token->index, token->row, token->col, TokenTypeToString(token->type), token->token_name);
}

static char keywords[NUM_KEYWORDS][10] = {"scalar", "foreach"};
static char dataTypes[][10] = {"void"};

size_t getDataTypeSize(char *dt)
{
    if (strcmp(dt, "int") == 0)
        return sizeof(int);
    else if (strcmp(dt, "char") == 0)
        return sizeof(char);
    else if (strcmp(dt, "float") == 0)
        return sizeof(float);
    else if (strcmp(dt, "void") == 0)
        return sizeof(void);
    else if (strcmp(dt, "double") == 0)
        return sizeof(double);
    else if (strcmp(dt, "long") == 0)
        return sizeof(long);
    else if (strcmp(dt, "bool") == 0 || strcmp(dt, "short") == 0)
        return sizeof(short);
    else
        return 0;
}

int isKeyword(char str[])
{
    for (int i = 0; i < NUM_KEYWORDS; i++)
        if (strcmp(str, keywords[i]) == 0)
            return 1;

    return 0;
}

int isDataType(char str[])
{
    for (int i = 0; i < NUM_DATATYPES; i++)
        if (strcmp(str, dataTypes[i]) == 0)
            return 1;
    return 0;
}

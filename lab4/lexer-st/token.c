#include "token.h"
#include "st.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define NUM_KEYWORDS 33
#define NUM_DATATYPES 7
#define LEN_BUFFER 64

int isKeyword(char str[]);
int isDataType(char str[]);

char *token_strings[] = {
    "KEYWORD",
    "IDENTIFIER",
    "DATA_TYPE",
    "FUNCTION",
    "STRING_LIT",
    "NUM_CONST",
    "REL_OP",
    "ARTH_OP",
    "LOG_OP",
    "ASSIGN_OP",
    "END_OF_FILE",
    "SEMI_COLON",
    "LEFT_PAREN",
    "RIGHT_PAREN",
    "LEFT_SQUARE_BRACKET",
    "RIGHT_SQUARE_BRACKET",
    "LEFT_CURLY_BRACE",
    "RIGHT_CURLY_BRACE",
    "PERIOD",
    "COLON",
    "COMMA",
    "NEW_LINE",
    "WHITESPACE"};

char *TokenTypeToString(TokenType t)
{
    return token_strings[t];
}

Token *createToken(TokenType type, char *value, int row, int col)
{
    static int index = 0;

    Token *ptr = malloc(sizeof(Token));
    ptr->row = row;
    ptr->col = col;
  

    if (type == FUNCTION){
      //Add entry will create a new local symbol table
      
    }
    else if (type == DATA_TYPE){
      //Buffered so that addSymbol in st.c knows what the data type is
      memset(data_type_buffer, '\0', sizeof(data_type_buffer));
      strcat(data_type_buffer, value);
    }
    else if (type == IDENTIFIER)
        ptr->index = ++index;
    else
        ptr->index = 0;
    
    strncpy(ptr->token_name, value, TOKEN_NAME_LENGTH);
    ptr->type = type;

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
    else if (isalpha(ch))
    {
        while (isalpha(ch))
        {
            colNum++;
            buffer[buf_index++] = ch;
            ch = fgetc(fin);
        }
        
        if (isDataType(buffer))
            type = DATA_TYPE;
        else if (isKeyword(buffer))
            type = KEYWORD;
        else {
           if(ch == '(')
             type = FUNCTION;
           else 
             type = IDENTIFIER;
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
    else if (ch == '"')
    {
        type = STRING_LIT;
        colNum++;
        buffer[buf_index++] = ch;

        ch = fgetc(fin);
        colNum++;

        while (ch != '"')
        {
            colNum++;
            buffer[buf_index++] = ch;
            ch = fgetc(fin);
        }

        buffer[buf_index++] = ch;
    }
    else if (ch == '=')
    {
        colNum++;
        buffer[buf_index++] = ch;
        ch = fgetc(fin);

        if (ch == '=')
        {
            //Relational Operator ==
            buffer[buf_index++] = ch;
            colNum++;
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
        buffer[buf_index++] = ch;
        colNum++;

        ch = fgetc(fin);
        if (ch == '=')
        {
            buffer[buf_index++] = ch;
            colNum++;
        }
        else
            fseek(fin, -1L, SEEK_CUR);
    }
    else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%')
    {
        type = ARTH_OP;
        buffer[buf_index++] = ch;
    }
    else if (ch == '&')
    {
        colNum++;
        buffer[buf_index++] = ch;
        ch = fgetc(fin);

        if (ch == '&')
        {
            type = LOG_OP;
            colNum++;
        }
        else
            fseek(fin, -1L, SEEK_CUR);
    }
    else if (ch == '|')
    {
        colNum++;
        buffer[buf_index++] = ch;
        ch = fgetc(fin);

        if (ch == '|')
        {
            type = LOG_OP;
            colNum++;
        }
        else
            fseek(fin, -1L, SEEK_CUR);
    }
    else if (ch == ';')
    {
        colNum++;
        buffer[buf_index++] = ch;
        type = SEMI_COLON;
    }
    else if (ch == '(' || ch == ')' || ch == '[' || ch == ']' || ch == '{' || ch == '}')
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
        case '[':
            type = LEFT_SQUARE_BRACKET;
            break;
        case ']':
            type = RIGHT_SQUARE_BRACKET;
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
    else if (ch == ':')
    {
        colNum++;
        buffer[buf_index++] = ch;
        type = COLON;
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

    printf("Index: %d, Row: %d, Col: %d, Name: %s, Type: %s\n", token->index, token->row, token->col, token->token_name, TokenTypeToString(token->type));
}

void displayAngleBrackets(Token *token)
{
}

static char keywords[NUM_KEYWORDS][10] = {"auto", "bool", "break", "case", "char", "const", "continue", "default", "double", "enum", "extern", "float", "for", "goto", "if", "int", "long", "register", "return", "short", "signed", "sizeof", "static", "struct", "switch", "typeof", "union", "unsigned", "void", "volatile", "while", "do", "else"};
static char dataTypes[][10] = {
    "double",
    "float",
    "int",
    "long",
    "short",
    "char", "bool"};

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

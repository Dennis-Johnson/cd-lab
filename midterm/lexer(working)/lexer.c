#include "lexer.h"

#define NUM_KEYWORDS 2
#define NUM_DATA_TYPES 2

int isKeyword(char *str);
int isDataType(char *str);

//Contains entire input file
FILE *fin;
char input_buffer[LEN_INPUT_BUFFER];
int ip_indx = -1;

Token *createToken(int row, int col, char *name, TokenType type)
{
    Token *token = malloc(sizeof(Token));
    token->row = row;
    token->col = col;
    strcpy(token->name, name);
    token->type = type;

    displayToken(token);
    return token;
}

Token *getNextToken(FILE *fin)
{
    static int row = 0;
    static int col = 0;
    TokenType type = DEFAULT;
    char name[LEN_NAME];
    int name_indx, ch;

    do
    {
        memset(name, 0, sizeof(name));
        name_indx = 0;

        ch = fgetc(fin);

        if (ch == EOF)
        {
            type = END_OF_FILE;
            strcpy(name, "EOF");
            break;
        }
        else if (ch == '\n')
        {
            type = NEW_LINE;
            row++;
        }
        else if (isspace(ch))
        {
            type = WHITESPACE;
            while (isspace(ch))
            {
                ch = fgetc(fin);
            }
            fseek(fin, -1L, SEEK_CUR);
        }
        else if (isalpha(ch))
        {
            while (isalpha(ch))
            {
                name[name_indx++] = ch;
                ch = fgetc(fin);
            }
            fseek(fin, -1L, SEEK_CUR);

            if (isKeyword(name))
                type = KEYWORD;
            else if (isDataType(name))
                type = DATA_TYPE;
            else
                type = IDENTIFIER;
        }
        else if (isdigit(ch))
        {
            type = NUMBER;
            while (isdigit(ch))
            {
                name[name_indx++] = ch;
                ch = fgetc(fin);
            }
            fseek(fin, -1L, SEEK_CUR);
        }
        else if (ch == '"')
        {
            type = STRING;
            ch = fgetc(fin);
            while (ch != '"')
            {
                name[name_indx++] = ch;
                ch = fgetc(fin);
            }
        }
        else if (ch == '<')
        {
            name[name_indx++] = ch;
            ch = fgetc(fin);
            if (ch == '-')
            {
                name[name_indx++] = ch;
                type = ARROW;
            }
            else
            {
                fprintf(stderr, "Error: after < char read\n");
            }
        }
        else if (ch == '+')
        {
            name[name_indx++] = ch;
            type = ADD_OP;
        }
        else if (ch == '-')
        {
            name[name_indx++] = ch;
            //lookahead
            if ((ch = fgetc(fin)) == '-')
            {
                //This is a comment
                while (ch != '\n')
                {
                    ch = fgetc(fin);
                }
                fseek(fin, -1L, SEEK_CUR);
            }
            else
            {
                fprintf(stderr, "Error: after - char read\n");
            }
        }
        else if (ch == '(')
        {
            name[name_indx++] = ch;
            type = LEFT_PAREN;
        }
        else if (ch == ')')
        {
            name[name_indx++] = ch;
            type = RIGHT_PAREN;
        }
        else if (ch == '{')
        {
            name[name_indx++] = ch;
            type = LEFT_CURLY;
        }
        else if (ch == '}')
        {
            name[name_indx++] = ch;
            type = RIGHT_CURLY;
        }
        else if (ch == '.')
        {
            name[name_indx++] = ch;
            type = DOT;
        }
        else if (ch == ':')
        {
            name[name_indx++] = ch;
            type = COLON;
        }
        else if (ch == ';')
        {
            name[name_indx++] = ch;
            type = SEMICOLON;
        }
        else
        {
            fprintf(stderr, "Unrecognized char %c\n", ch);
        }
    } while (type == WHITESPACE || type == NEW_LINE);

    return createToken(row, col, name, type);
}

char *Keywords[NUM_KEYWORDS] = {
    "class", "concat"};
char *DataTypes[NUM_DATA_TYPES] = {
    "int",
    "string"};

int isKeyword(char *str)
{
    for (int i = 0; i < NUM_KEYWORDS; i++)
    {
        if (strcmp(str, Keywords[i]) == 0)
            return 1;
    }

    return 0;
}
int isDataType(char *str)
{
    for (int i = 0; i < NUM_KEYWORDS; i++)
    {
        if (strcmp(str, DataTypes[i]) == 0)
            return 1;
    }

    return 0;
}
char *tokenTypeString(TokenType t)
{
    return TokenStrings[t];
}

void displayToken(Token *token)
{
    if (!token)
    {
        fprintf(stderr, "Empty token pointer, can't display\n");
        return;
    }

    printf("Row: %-3d, Col: %-3d, Type: %-10s, Name: %-10s\n", token->row, token->col, tokenTypeString(token->type), token->name);
}

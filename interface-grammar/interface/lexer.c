#include "lexer.h"

int isKeyword(char *str);
Token *createToken(int row, int col, char *name, TokenType type)
{
    Token *token = malloc(sizeof(Token));
    token->row = row;
    token->col = col;
    token->type = type;
    strcpy(token->name, name);

    return token;
}

Token *getNextToken(FILE *fin)
{
    static int row = 0;
    static int col = 0;
    TokenType type = DEFAULT;
    char name[LEN_NAME];
    int name_indx = 0, ch;

    do
    {
        memset(name, 0, sizeof(name));
        name_indx = 0;

        ch = fgetc(fin);
        if (ch == EOF)
        {
            type = END_OF_FILE;
            strcpy(name, "EOF");
        }
        else if (ch == '\n')
        {
            row++;
            col = 0;
        }
        else if (isspace(ch))
        {
            while (isspace(ch))
            {
                col++;
                ch = fgetc(fin);
            }
            fseek(fin, -1L, SEEK_CUR);
        }
        else if (isalpha(ch))
        {
            while (isalpha(ch) || isdigit(ch))
            {
                col++;
                name[name_indx++] = ch;
                ch = fgetc(fin);
            }
            fseek(fin, -1L, SEEK_CUR);

            if (isKeyword(name))
                type = KEYWORD;
            else
                type = IDENTIFIER;
        }
        else if (isdigit(ch))
        {
            type = NUMBER;
            while (isdigit(ch))
            {
                col++;
                name[name_indx++] = ch;
                ch = fgetc(fin);
            }
            fseek(fin, -1L, SEEK_CUR);
        }
        else if (ch == '{')
        {
            type = LEFT_CURLY;
            col++;
            name[name_indx++] = ch;
        }
        else if (ch == '}')
        {
            type = RIGHT_CURLY;
            col++;
            name[name_indx++] = ch;
        }
        else if (ch == '[')
        {
            type = LEFT_SQUARE;
            col++;
            name[name_indx++] = ch;
        }
        else if (ch == ']')
        {
            type = RIGHT_SQUARE;
            col++;
            name[name_indx++] = ch;
        }
        else if (ch == ',')
        {
            type = COMMA;
            col++;
            name[name_indx++] = ch;
        }
        else if (ch == ';')
        {
            type = SEMICOLON;
            col++;
            name[name_indx++] = ch;
        }
        else
        {
            printf("Unrecognized char %c\n", ch);
        }
    } while (type == DEFAULT);

    return createToken(row, col, name, type);
}

void ungetToken(Token *token, FILE *fin)
{
    int len = strlen(token->name);
    fseek(fin, -1 * len, SEEK_CUR);
}

char *TokenStrings[] =
    {
        "KEYWORD",
        "IDENTIFIER",
        "NUMBER",
        "LEFT_SQUARE",
        "RIGHT_SQUARE",
        "LEFT_CURLY",
        "RIGHT_CURLY",
        "COMMA",
        "SEMICOLON",
        "END_OF_FILE",
        "DEFAULT",
};

char *Keywords[NUM_KEYWORDS] = {
    "interface",
    "extends",
    "public",
    "private",
    "abstract",
    "static",
    "final",
    "int",
    "float",
    "boolean",
};

char *typeToString(TokenType t)
{
    return TokenStrings[t];
}
void displayToken(Token *token)
{
    if (!token)
    {
        fprintf(stderr, "NULL Token ptr!\n");
        return;
    }

    printf("Row: %-3d, Col: %-3d, Type: %-15s, Name: %-10s\n", token->row, token->col, typeToString(token->type), token->name);
}

int isKeyword(char *str)
{
    for (int i = 0; i < NUM_KEYWORDS; i++)
    {
        if (strcmp(str, Keywords[i]) == 0)
            return 1;
    }

    return 0;
}
#include "lexer.h"

Token *createToken(int row, int col, char name[], TokenType type)
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
    int name_indx, ch;

    do
    {
        ch = fgetc(fin);
        memset(name, '\0', sizeof(name));
        name_indx = 0;

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
            while (isalpha(ch))
            {
                name[name_indx++] = ch;
                col++;
                ch = fgetc(fin);
            }
            fseek(fin, -1L, SEEK_CUR);

            if (isKeyword(name))
                type = KEYWORD;
            else
                type = IDENTIFIER;
        }
        else if (ch == '+' || ch == '-')
        {
            name[name_indx++] = ch;
            col++;
            type = ARTH_OP;
        }
        else if (ch == '=')
        {
            name[name_indx++] = ch;
            col++;
            type = ASSIGN_OP;
        }
        else if (ch == '(')
        {
            name[name_indx++] = ch;
            col++;
            type = LEFT_PAREN;
        }
        else if (ch == ')')
        {
            name[name_indx++] = ch;
            col++;
            type = RIGHT_PAREN;
        }
        else if (ch == '{')
        {
            name[name_indx++] = ch;
            col++;
            type = LEFT_CURLY;
        }
        else if (ch == '}')
        {
            name[name_indx++] = ch;
            col++;
            type = RIGHT_CURLY;
        }
        else if (ch == ';')
        {
            name[name_indx++] = ch;
            col++;
            type = SEMICOLON;
        }
        else
            printf("Unrecognized char %c\n", ch);

    } while (type == DEFAULT);

    return createToken(row, col, name, type);
}

void ungetToken(Token *token, FILE *fin)
{
    int len = strlen(token->name);
    fseek(fin, -1 * len, SEEK_CUR);
}

char *TokenStrings[] = {
    "KEYWORD",
    "IDENTIFIER",
    "ARTH_OP",
    "ASSIGN_OP",
    "LEFT_CURLY",
    "RIGHT_CURLY",
    "LEFT_PAREN",
    "RIGHT_PAREN",
    "SEMICOLON",
    "END_OF_FILE",
    "DEFAULT",
};

char *Keywords[NUM_KEYWORDS] = {
    "if",
};

char *typeToString(TokenType t)
{
    return TokenStrings[t];
}

void displayToken(Token *token)
{
    if (!token)
    {
        fprintf(stderr, "NULL token pointer!\n");
        return;
    }

    printf("Row: %-3d, Col: %-3d, Type: %-10s, Name: %-10s\n", token->row, token->col, typeToString(token->type), token->name);
}

int isKeyword(char *str)
{
    for (int i = 0; i < NUM_KEYWORDS; i++)
    {
        if (strcmp(Keywords[i], str) == 0)
            return 1;
    }
    return 0;
}

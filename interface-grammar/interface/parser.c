#include "parser.h"
#include "lexer.h"

FILE *fin;
Token *token;

void Intf_Decl();
void Intf_Modifier();
void Ex_Intfs();
void Ex_Intfs_Prime();
void Intf_Body();
void Intf_Mem_Declarations();
void Intf_Mem_Declarations_Prime();
void Intf_Mem_Dec();
void Const_Decl();
void Const_Idlist();
void Const_Idlist_Prime();
void Const_Idlist_PrimePrime();
void Const_Modifier();
void Type();
void Intf_Type();
void error(char *str);

void parse()
{
    // while (1)
    // {
    //     token = getNextToken(fin);
    //     displayToken(token);

    //     if (token->type == END_OF_FILE)
    //         break;
    // }
    Intf_Decl();
}

void Intf_Decl()
{
    token = getNextToken(fin);
    if (strcmp(token->name, "public") == 0 || strcmp(token->name, "private") == 0 || strcmp(token->name, "abstract") == 0 || strcmp(token->name, "interface") == 0)
    {
        ungetToken(token, fin);
        Intf_Modifier();

        token = getNextToken(fin);
        if (strcmp(token->name, "interface"))
        {

            token = getNextToken(fin);
            if (token->type == IDENTIFIER)
            {
                token = getNextToken(fin);
                if (strcmp(token->name, "extends") == 0)
                {
                    Ex_Intfs();
                    Intf_Body();

                    token = getNextToken(fin);
                    if (token->type == END_OF_FILE)
                    {
                        printf("Parsing succesful!\n");
                        return;
                    }
                    else
                        error("END_OF_FILE");
                }
                else
                    error("extends");
            }
            else
                error("IDENTIFIER");
        }
        else
            error("Interface");
    }
    else
        error("IntfDecl");
}
void Intf_Modifier()
{
    token = getNextToken(fin);
    if (strcmp(token->name, "public") == 0 || strcmp(token->name, "private") == 0 || strcmp(token->name, "abstract") == 0)
    {
        return;
    }
    else
    {
        ungetToken(token, fin);
    }
}

void Ex_Intfs()
{
    token = getNextToken(fin);
    if (token->type == IDENTIFIER)
    {
        Intf_Type();
        Ex_Intfs_Prime();
    }
    else
        error("IDENTIFIER");
}

void Ex_Intfs_Prime()
{
    token = getNextToken(fin);
    if (token->type == COMMA)
    {
        Intf_Type();
        Ex_Intfs_Prime();
    }
    else
    {
        ungetToken(token, fin);
    }
}

void Intf_Body()
{
    token = getNextToken(fin);
    if (token->type == LEFT_CURLY)
    {
        Intf_Mem_Declarations();
    }
    else
        error("LEFT_CURLY");
}

void Intf_Mem_Declarations()
{
    token = getNextToken(fin);
    if (strcmp(token->name, "public") == 0 || strcmp(token->name, "static") == 0 || strcmp(token->name, "final") == 0)
    {
        ungetToken(token, fin);
        Intf_Mem_Dec();
        Intf_Mem_Declarations_Prime();
    }
    else
    {
        ungetToken(token, fin);
    }
}
void Intf_Mem_Declarations_Prime()
{
    token = getNextToken(fin);
    if (strcmp(token->name, "public") == 0 || strcmp(token->name, "static") == 0 || strcmp(token->name, "final") == 0)
    {
        ungetToken(token, fin);
        Intf_Mem_Dec();
        Intf_Mem_Declarations_Prime();
    }
    else
    {
        ungetToken(token, fin);
    }
}

void Intf_Mem_Dec()
{
    token = getNextToken(fin);
    if (strcmp(token->name, "public") == 0 || strcmp(token->name, "static") == 0 || strcmp(token->name, "final") == 0)
    {
        ungetToken(token, fin);
        Const_Decl();
    }
    else
    {
        ungetToken(token, fin);
    }
}

void Const_Decl()
{
    token = getNextToken(fin);
    if (strcmp(token->name, "public") == 0 || strcmp(token->name, "static") == 0 || strcmp(token->name, "final") == 0)
    {
        ungetToken(token, fin);
        Const_Modifier();
        Type();
        Const_Idlist();

        token = getNextToken(fin);
        if (token->type == SEMICOLON)
        {
            Const_Decl();
        }
        else
            error("SEMICOLON");
    }
    else
    {
        ungetToken(token, fin);
    }
}

void Const_Idlist()
{
    token = getNextToken(fin);
    if (token->type == IDENTIFIER)
    {
        Const_Idlist_Prime();
    }
    else
        error("IDENTIFIER");
}

void Const_Idlist_Prime()
{
    token = getNextToken(fin);
    if (token->type == COMMA)
    {
        Const_Idlist();
    }
    else if (token->type == LEFT_SQUARE)
    {
        token = getNextToken(fin);
        if (token->type == NUMBER)
        {
            token = getNextToken(fin);
            if (token->type == RIGHT_SQUARE)
            {
                token = getNextToken(fin);
                if (token->type == COMMA)
                {
                    Const_Idlist_PrimePrime();
                }
                else
                    error("COMMA");
            }
            else
                error("RIGHT_SQUARE");
        }
        else
            error("NUMBER");
    }
    else
        ungetToken(token, fin);
}

void Const_Idlist_PrimePrime()
{
    token = getNextToken(fin);
    if (token->type == COMMA)
    {
        Const_Idlist();
    }
    else
        ungetToken(token, fin);
}

void Const_Modifier()
{
    token = getNextToken(fin);
    if (strcmp(token->name, "public") == 0 || strcmp(token->name, "static") == 0 || strcmp(token->name, "final") == 0)
    {
        return;
    }
    else
        error("Const_Modifier");
}

void Type()
{
    token = getNextToken(fin);
    if (strcmp(token->name, "int") == 0 || strcmp(token->name, "float") == 0 || strcmp(token->name, "boolean") == 0)
    {
        return;
    }
    else
        error("Type");
}

void Intf_Type()
{
    token = getNextToken(fin);
    if (token->type == IDENTIFIER)
    {
        return;
    }
    else
        error("IDENTIFIER");
}

void error(char *str)
{
    printf("Error: expected %s\n", str);
}
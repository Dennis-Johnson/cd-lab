#include "parser.h"
#include "lexer.h"

Token *token;
FILE *fin;

void Intf_Decl();
void Intf_Mod();
void Ex_Intf();
void Ex_Intf_Prime();
void Intf_Body();
void Intf_Mem_Declarations();
void Intf_Mem_Declarations_Prime();
void Intf_Mem_Dec();
void Const_Decl();
void Const_Idlist();
void Const_Idlist_Prime();
void Const_Idlist_Prime_Prime();
void Const_Mod();
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
    Intf_Mod();

    token = getNextToken(fin);
    if (strcmp(token->name, "interface") == 0)
    {

        token = getNextToken(fin);
        if (token->type == IDENTIFIER)
        {
            token = getNextToken(fin);
            if (strcmp(token->name, "extends") == 0)
            {
                Ex_Intf();
                Intf_Body();

                token = getNextToken(fin);
                if (token->type == END_OF_FILE)
                {
                    printf("Parsing successful!\n");
                }
                else
                    error("EOF");
            }
            else
                error("extends");
        }
        else
            error("identifier");
    }
    else
        error("Interface");
}

void Intf_Mod()
{
    token = getNextToken(fin);
    if (strcmp(token->name, "public") == 0 || strcmp(token->name, "private") == 0 || strcmp(token->name, "abstract") == 0)
    {
        return;
    }
    else
        ungetToken(token, fin);
}

void Ex_Intf()
{
    Intf_Type();
    Ex_Intf_Prime();
}

void Ex_Intf_Prime()
{
    token = getNextToken(fin);
    if (token->type == COMMA)
    {
        Intf_Type();
        Ex_Intf_Prime();
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

        token = getNextToken(fin);
        if (token->type == RIGHT_CURLY)
        {
            return;
        }
        else
            error("Right curly");
    }
    else
        error("Left curly");
}

void Intf_Mem_Declarations()
{
    Intf_Mem_Dec();
    Intf_Mem_Declarations_Prime();
}
void Intf_Mem_Declarations_Prime()
{
    Intf_Mem_Dec();
    Intf_Mem_Declarations_Prime();
}

void Intf_Mem_Dec()
{
    Const_Decl();
}

void Const_Decl()
{
    token = getNextToken(fin);
    if (strcmp(token->name, "public") == 0 || strcmp(token->name, "static") == 0 || strcmp(token->name, "final") == 0)
    {
        // ungetToken(token, fin);
        // Const_Mod();
        Type();
        Const_Idlist();

        token = getNextToken(fin);
        if (token->type == SEMICOLON)
        {
            Const_Decl();
        }
        else
            error("Semicolon");
    }
    else
        ungetToken(token, fin);
}

void Const_Idlist()
{
    token = getNextToken(fin);
    if (token->type == IDENTIFIER)
    {
        Const_Idlist_Prime();
    }
    else
        error("Identifier");
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
                Const_Idlist_Prime_Prime();
            }
            else
                error("RIGHT_SQUARE");
        }
        else
            error("Number");
    }
    else
    {
        ungetToken(token, fin);
    }
}

void Const_Idlist_Prime_Prime()
{
    token = getNextToken(fin);
    if (token->type == COMMA)
    {
        Const_Idlist();
    }
    else
        ungetToken(token, fin);
}

void Const_Mod()
{
    token = getNextToken(fin);
    if (strcmp(token->name, "public") == 0 || strcmp(token->name, "static") == 0 || strcmp(token->name, "final") == 0)
        return;
    else
        error("Const Modifier");
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
        error("Identifier");
}

void error(char *str)
{
    printf("Error: expected %s\n", str);
}

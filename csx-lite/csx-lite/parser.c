#include "parser.h"

Token *token;
FILE *fin;

void Prog();  //Prog -> { Stmts } EOF
void Stmts(); //Stmts -> Stmt Stmts | EPSILON
void Stmt();  //Stmt -> id = Expr; | if (Expr) Stmt
void Expr();  //Expr -> id Etail
void Etail(); //Etail -> + Expr | - Expr | EPSILON
void error(char *str);

void parse()
{
    Prog();
}

void Prog()
{
    token = getNextToken(fin);
    if (token->type == LEFT_CURLY)
    {
        Stmts();

        token = getNextToken(fin);
        if (token->type == RIGHT_CURLY)
        {
            token = getNextToken(fin);
            if (token->type == END_OF_FILE)
            {
                printf("Parsing successful\n");
            }
            else
                error("EOF");
        }
        else
            error("RIGHT_CURLY");
    }
    else
        error("LEFT_CURLY");
}

void Stmts()
{
    token = getNextToken(fin);
    if (token->type == IDENTIFIER || strcmp(token->name, "if") == 0)
    {
        ungetToken(token, fin);
        Stmt();
        Stmts();
    }
    else
    {
        ungetToken(token, fin);
    }
}

void Stmt()
{
    token = getNextToken(fin);
    if (token->type == IDENTIFIER)
    {
        token = getNextToken(fin);
        if (token->type == ASSIGN_OP)
        {
            Expr();
            token = getNextToken(fin);
            if (token->type == SEMICOLON)
            {
                return;
            }
            else
                error("SEMICOLON");
        }
        else
            error("ASSIGN_OP");
    }
    else if (strcmp(token->name, "if") == 0)
    {
        token = getNextToken(fin);
        if (token->type == LEFT_PAREN)
        {
            Expr();

            token = getNextToken(fin);
            if (token->type == RIGHT_PAREN)
            {

                Stmt();
            }
            else
                error("RIGHT_PAREN");
        }
        else
            error("LEFT_PAREN");
    }
    else
    {
        error("Wrong path");
    }
}

void Expr()
{
    token = getNextToken(fin);
    if (token->type == IDENTIFIER)
    {
        Etail();
    }
    else
        error("IDENTIFIER");
}

void Etail()
{
    token = getNextToken(fin);
    if (token->type == ARTH_OP)
    {
        Expr();
    }
    else
    {
        ungetToken(token, fin);
    }
}

void error(char *str)
{
    printf("Error: expected %s\n", str);
}

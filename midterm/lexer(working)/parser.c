#include "parser.h"
#include "lexer.h"

void runParser()
{
    Token *token;
    while (1)
    {

        token = getNextToken(fin);
        if (token->type == END_OF_FILE)
            break;
    }
}

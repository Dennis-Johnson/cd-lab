#include <stdio.h>
#include "lexer.h"
#include "parser.h"

#define FILE_PATH "input.lang"

int main()
{
    if ((fin = fopen(FILE_PATH, "r")) == NULL)
    {
        perror("Can't open input file: ");
        exit(EXIT_FAILURE);
    }
    parse(fin);
    fclose(fin);
    return 0;
}

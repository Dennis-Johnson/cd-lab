#include <stdio.h>
#include "parser.h"
#include "lexer.h"

int main()
{
    char *path = "input.txt";

    fin = fopen(path, "r");
    if (fin == NULL)
    {
        perror("Can't open input file: ");
        exit(EXIT_FAILURE);
    }

    runParser();
    fclose(fin);
    return 0;
}

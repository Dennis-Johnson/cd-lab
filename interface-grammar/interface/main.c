#include <stdio.h>
#include "parser.h"

#define FILE_PATH "input.int"
int main()
{
    fin = fopen(FILE_PATH, "r");
    parse();
    fclose(fin);
    return 0;
}

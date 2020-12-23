#ifndef PARSER_H_
#define PARSER_H_

#include <stdio.h>
#include "lexer.h"

void parse();
extern Token *token;
extern FILE *fin;

#endif

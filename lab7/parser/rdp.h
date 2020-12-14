#ifndef RDP_H_
#define RDP_H_

#include "token.h"
extern Token *token;
extern FILE *fin;

int parse(FILE *fin);
#endif //RDP_H_

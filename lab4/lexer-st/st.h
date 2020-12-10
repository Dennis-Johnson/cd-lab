#include "token.h"

Token* createSymbolTable();
Token* addEntry(Token* SymbolTable, Token* NewToken);
int symbolExists(Token *SymbolTable, Token* token);

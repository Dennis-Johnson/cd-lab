#include "token.h"
#include "st.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_LENGTH 10
#define L_BUFFER_LEN 10

// Create a new symbol table
// For this week, make only LOCAL symbol tables for each function
Token* createSymbolTable(){
  Token* SymbolTable = malloc(TABLE_LENGTH * sizeof(Token*));
  return SymbolTable;  
}

//Add an entry to an existing symbol table
Token* addEntry(Token* SymbolTable, Token* NewToken){
  //The index in the ST is the index on the token, assigned by getNextToken;
  int index = NewToken->index;
  SymbolTable[index] = NewToken; 
  return SymbolTable;  
}

//Check whether Token index already exists on ST or not
int symbolExists(Token *SymbolTable, Token* token){
  if (!token || !SymbolTable){
    fprintf(stderr, "Empty token or symbol table\n");
    return 0;  
  }

  int t_index = token->index;
  for(int i = 0; i < TABLE_LENGTH; i++){
    ptr = SymbolTable[i];

    if(ptr == NULL){
      //NOT FOUND
      return 0;
    }

    if(ptr->index == t_index)
      return 1;
  }

  return 0;
}

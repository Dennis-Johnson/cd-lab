#include "token.h"
#include "st.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_LENGTH 10

char data_type_buffer[LEN_SYMBOL_BUFFER];

// Create a new symbol table
// For this week, make only LOCAL symbol tables for each function
Symbol* createSymbolTable(){
  Symbol* SymbolTable = calloc(TABLE_LENGTH,  sizeof(Symbol));
  return SymbolTable;  
}

//Add an entry to an existing symbol table
int addEntry(Symbol* SymbolTable, Token* NewToken){
  //The index in the ST is the index on the token, assigned by getNextToken;
  if(!NewToken){
    fprintf(stderr, "Error: empty token\n");
    return -1;
  }

  static int index = 0;

  if(!SymbolTable){
    //Create a new symbol table
    index = 0;
    SymbolTable = createSymbolTable();
  }
  else if (symbolExists(SymbolTable, NewToken)){
    // This symbol entry was made previously
    return -1;
  }
  else index++;

  //TODO: Fix this index stuff later, buffer, size as well!
  Symbol entry;
  
  entry.index = index;
  strcpy(entry.lexeme_name, NewToken->token_name);
  strcpy(entry.data_type, data_type_buffer);
  entry.size = 0;

  SymbolTable[index] = entry; 
  return 1;  
}

//Check whether Token index already exists on ST or not
int symbolExists(Symbol *SymbolTable, Token* token){
  if (!token || !SymbolTable){
    fprintf(stderr, "Empty token or symbol table\n");
    return -1;  
  }
  
  Symbol* ptr = SymbolTable;
  for(int i = 0; i < TABLE_LENGTH; i++, ptr++){
    
    if(ptr == NULL){
    // Not found
      return 0;
    }

    if(ptr->index == token->index)
      return 1;
  }

  return 0;
}

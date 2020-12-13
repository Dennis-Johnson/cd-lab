#include "token.h"
#include "st.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Symbol* LocalSymbolTables[NUM_LOCAL_TABLES];
int local_st_index = 0;
char data_type_buffer[LEN_SYMBOL_BUFFER];

// Create a new symbol table
// For this week, make only LOCAL symbol tables for each function
Symbol* createSymbolTable(){
  Symbol* SymbolTable = calloc(TABLE_LENGTH,  sizeof(Symbol));
  return SymbolTable;  
}

//Add an entry to an existing symbol table
// If added returns the index of the entry to be assigned to index of token as well
int addEntry(Token* token){
  if(token == NULL){
    fprintf(stderr, "Error: NULL token\n");
    return -1;
  } 

  if( token->type != FUNCTION && token->type != IDENTIFIER){
    fprintf(stderr, "Error: Can't make entry for type - %s\n", TokenTypeToString(token->type));
    return -1;
  }

  static int index = 0;
  if (token->type == FUNCTION)
    printf("\nDT_BUFFER: %s, isDT: %d\n",data_type_buffer, isDataType(data_type_buffer));
  
  if(token->type == FUNCTION && isDataType(data_type_buffer)){
    //Create a new symbol table
    local_st_index++;

    if(local_st_index == NUM_LOCAL_TABLES){
      fprintf(stderr, "Error: Can't make any more local symbol tables!\n");
      exit(EXIT_FAILURE);
    }

    index = 0;
    LocalSymbolTables[local_st_index] = createSymbolTable();
  }
  
  else if (symbolExists(LocalSymbolTables[local_st_index], token)){
    // This symbol entry was made previously
    return -2;
  }
  
  else index++;

  //TODO: Fix this index stuff later, buffer, size as well!
  Symbol* entry = &(LocalSymbolTables[local_st_index][index]);
  
  entry->index = index;
  strcpy(entry->lexeme_name, token->token_name);
  strcpy(entry->data_type, data_type_buffer);
  entry->size = 0;
  
  printf("Added entry for lexeme_name %s index %d\n", entry->lexeme_name, entry->index);
  //displaySymbolTable(LocalSymbolTables[local_st_index]);
  return index;  
}

//Check whether Token index already exists on ST or not
int symbolExists(Symbol *SymbolTable, Token* token){
  if (!token || !SymbolTable){
    fprintf(stderr, "Empty token or symbol table\n");
    return -1;  
  }
  
  Symbol* ptr = SymbolTable;
  printf("Checking local symbol table %d\n", local_st_index);

  for(int i = 0; i < TABLE_LENGTH; i++, ptr++){
    printf("Compare token t_name %s and table lex_name %s\n", token->token_name, ptr->lexeme_name); 
    if(strcmp(ptr->lexeme_name, token->token_name) != 0){
    // Not found
      printf("%s not found in symbol table\n", token->token_name);
      return 0;
    }
    else{
      printf("Found in table!\n");
      return 1;
    }
  }

  return 0;
}

void displaySymbolTable(Symbol* st){
  if(st == NULL){
    fprintf(stderr, "NULLPTR: SymbolTable does not exist!\n");
    return;
  }
}

void displayAllLocalSymbolTables(){
 int index = 0;

 printf("Displaying All Local Symbol Tables:\n");

 if(index <= local_st_index){
  displaySymbolTable(LocalSymbolTables[index]);
 }

}

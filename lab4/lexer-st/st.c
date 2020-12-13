#include "token.h"
#include "st.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Symbol* LocalSymbolTables[NUM_LOCAL_TABLES];
int local_st_index = -1;
char data_type_buffer[LEN_SYMBOL_BUFFER];

void formattedLine(int width); // Used before and after displaying st contents

// Create a new symbol table
// For this week, make only LOCAL symbol tables for each function
Symbol* createSymbolTable(){
  Symbol* SymbolTable = calloc(TABLE_LENGTH,  sizeof(Symbol));

  for(int i = 0; i < TABLE_LENGTH; i++){
    SymbolTable[i].index = -1;
    strcpy(SymbolTable[i].lexeme_name, "-");
    strcpy(SymbolTable[i].data_type, "-");
    SymbolTable[i].size = 0;
  }

  return SymbolTable;  
}

//Add an entry to an existing symbol table
// If added returns the index of the entry to be assigned to index of token as well
int addEntry(Token* token){
  if(token == NULL){
    fprintf(stderr, "Error: NULL token\n");
    return -1;
  } 

  //returned index of entry, if already present. Else -1 for not found. 
  int ret_ind;

  if( token->type != FUNCTION && token->type != IDENTIFIER){
    fprintf(stderr, "Error: Can't make entry for type - %s\n", TokenTypeToString(token->type));
    return -1;
  }

  static int index = 0;
  
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
  
  else{
    int ret_ind = symbolExists(LocalSymbolTables[local_st_index], token);

    if (ret_ind > 0)
      return ret_ind; // This symbol entry was made previously
    else 
      index++;
  }
  
  Symbol* entry = &(LocalSymbolTables[local_st_index][index]);
  
  entry->index = index;
  strcpy(entry->lexeme_name, token->token_name);
  strcpy(entry->data_type, data_type_buffer);
  entry->size = 0;
 
  return index;  
}

//Check whether Token index already exists on local ST or not
int symbolExists(Symbol *SymbolTable, Token* token){
  if (!token || !SymbolTable){
    fprintf(stderr, "Empty token or symbol table\n");
    return -1;  
  }
  
  Symbol* ptr = SymbolTable;

  for(int i = 0; i < TABLE_LENGTH; i++, ptr++){
    if(ptr->index == -1){
      // There are no more entries in the table
      break;
    }

    if(strcmp(ptr->lexeme_name, token->token_name) == 0){
      // Found matching entry in the local symbol table
      return ptr->index;
    }
  }

  return -1;
}

void displaySymbolTable(Symbol* st){
  if(st == NULL){
    fprintf(stderr, "NULLPTR: SymbolTable does not exist!\n");
    return;
  }
  
  int width = 70;
  formattedLine(width);

  Symbol* ptr = st;
  for(int i = 0; i < TABLE_LENGTH; i++, ptr++){

    if(ptr->index == -1){
      //End of entries in the table
      break;
    }

    printf("| Index: %2d | Lex_Name: %-10s | Data_Type: %-10s | Size: %3d |\n", ptr->index, ptr->lexeme_name, ptr->data_type, ptr->size);
  }
  formattedLine(width);
}

void formattedLine(int width){
  printf("+");
  for(int i = 0; i < width; i++)
    printf("-");
  printf("+\n");
}

void displayAllLocalSymbolTables(){
 for(int index = 0; index <= local_st_index; index++){
   printf("\nDisplay Local Symbol Table %d\n", index);
   displaySymbolTable(LocalSymbolTables[index]);
 }
}

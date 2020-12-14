#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"

FILE *fin;
Token *token;
int unget_flag = 0;

/*
Recursive Descent Parser for the subset of C grammar in Q7.1
*/

void Program();   // Program --> int main () { Declarations Assign_Stmt }
void Declarations();   // Declarations --> Data_Type Id_List ; Declarations | EPSILON
void Data_Type();     // Data_Type --> int | char
void Id_List();     // Id_List --> id Id_List_Prime
void Id_List_Prime();   //Id_List_Prime --> , Id_List | EPSILON
void Assign_Stmt();   // Assign_Stmt --> id = Assign_Stmt_Prime
void Assign_Stmt_Prime(); //Assign_Stmt_Prime --> id ; | num ;
void error(TokenType type);

int parse(FILE *fin){
  // fin is an open file pointer to the preprocessed input file

  if(fin == NULL){
    fprintf(stderr, "Error: Invalid file pointer!\n");
    exit(EXIT_FAILURE);
  }

  printf("Parsing\n");
  
  /*
  while(1){
    token = getNextToken(fin);
    displayToken(token);

    if (token->type == END_OF_FILE)
      break;
  }*/
  Program();
  return 0;  
}

void Program(){
  token = getNextToken(fin);

  if(strcmp(token->token_name, "int") == 0){
    token = getNextToken(fin);
    
    if(strcmp(token->token_name, "main") == 0){
      token = getNextToken(fin);

      if(token->type == LEFT_PAREN){
        token = getNextToken(fin);

        if(token->type == RIGHT_PAREN){
          token = getNextToken(fin);

          if(token->type == LEFT_CURLY_BRACE){
            Declarations();
            Assign_Stmt();

            token = getNextToken(fin);
            if(token->type == RIGHT_CURLY_BRACE){
              
              token = getNextToken(fin);
              if(token->type == END_OF_FILE){
                printf("Parsing Sucessful!\n");
                exit(EXIT_SUCCESS);
              }
              else error(END_OF_FILE);
            }
            else error(RIGHT_CURLY_BRACE);
          }
          else error(LEFT_CURLY_BRACE);
        }
        else error(RIGHT_PAREN);
      }
      else error(LEFT_PAREN);
    }
    else error(FUNCTION);
  }
  else error(DATA_TYPE);
}

void Declarations(){ //Produces EPSILON
  token = getNextToken(fin);
  unget_flag = 1;

  //Check FIRST set of Declarations
  if(isDataType(token->token_name)){
    Data_Type();
    Id_List();

    // token = getNextToken(fin);
    if(token->type == SEMI_COLON){
      Declarations();
    }
    else error(SEMI_COLON);
  }
}

void Data_Type(){
  if(unget_flag == 1)
    unget_flag = 0;
  else
    token = getNextToken(fin);
  
  if(isDataType(token->token_name)){
    return;
  }
  else error(DATA_TYPE);
}

void Id_List(){
  token = getNextToken(fin);

  if(token->type == IDENTIFIER){
    Id_List_Prime();
  }
  else error(IDENTIFIER);
}

void Id_List_Prime(){
  token = getNextToken(fin);

  if(token->type == COMMA){
    Id_List();  
  }
}

void Assign_Stmt(){
  if(unget_flag)
    unget_flag = 0;
  else 
    token = getNextToken(fin);

  if(token->type == IDENTIFIER){
    token = getNextToken(fin);

    if(token->type == ASSIGN_OP){
      Assign_Stmt_Prime();
    }
    else error(ASSIGN_OP);
  }
  else error(IDENTIFIER);
}

void Assign_Stmt_Prime(){
  token = getNextToken(fin);

  if(token->type == IDENTIFIER || token->type == NUM_CONST){
    token = getNextToken(fin);

    if(token->type == SEMI_COLON){
      return;  
    }
    else error(SEMI_COLON);
  }
  else error(IDENTIFIER); //TODO: Make this a variadic function
}


void error(TokenType type){
  printf("ERROR: Expected %s\n",TokenTypeToString(type));
  // exit(EXIT_FAILURE);
}

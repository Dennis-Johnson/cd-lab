/*
Recursive Descent Parser for the subset of C grammar in Lab Q7.1
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"

#define NUM_OF_RULES 16

typedef enum
{
  Follow_Program,
  Follow_Declarations,
  Follow_Data_Type,
  Follow_Id_List,
  Follow_Id_List_Prime,
  Follow_Id_List_Prime_Prime,
  Follow_Stmt_List,
  Follow_Stmt, 
  Follow_Assign_Stmt,
  Follow_Expr,
  Follow_Expr_Prime,
  Follow_Simple_Expr,
  Follow_Simple_Expr_Prime,
  Follow_Term,
  Follow_Term_Prime,
  Follow_Factor
} Rule_Numbers;

int LEN_FOLLOW_SET[NUM_OF_RULES] = {1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 2, 2, 3, 3, 4};

TokenType FollowSet[NUM_OF_RULES][5] =
    {{END_OF_FILE},
     {IDENTIFIER, RIGHT_CURLY_BRACE},
     {IDENTIFIER},
     {SEMI_COLON},
     {SEMI_COLON},
     {SEMI_COLON},
     {RIGHT_CURLY_BRACE},
     {IDENTIFIER, RIGHT_CURLY_BRACE},
     {SEMI_COLON},
     {SEMI_COLON},
     {SEMI_COLON},
     {SEMI_COLON, REL_OP},
     {SEMI_COLON, REL_OP},
     {SEMI_COLON, REL_OP, ADD_OP},
     {SEMI_COLON, REL_OP, ADD_OP},
     {SEMI_COLON, REL_OP, ADD_OP, MUL_OP},
    };

FILE *fin;
Token *prevToken;
Token *token;

void Program();             // Program --> int main () { Declarations Stmt_List }
void Declarations();        // Declarations --> Data_Type Id_List ; Declarations | EPSILON
void Data_Type();           // Data_Type --> int | char | double | void .....
void Id_List();             // Id_List --> id Id_List_Prime
void Id_List_Prime();       // Id_List_Prime --> , Id_List | [ NUM_CONST ] ID_List_Prime_Prime | EPSILON
void Id_List_Prime_Prime(); // Id_List_Prime_Prime --> , Id_List | EPSILON
void Stmt_List();           // Stmt_List --> Stmt Stmt_List | EPSILON
void Stmt();                // Stmt --> Assign_Stmt ;
void Assign_Stmt();         // Assign_Stmt --> id = Expr
void Expr();                // Expr --> Simple_Expr Expr_Prime | EPSILON
void Expr_Prime();          // REL_OP Simple_Expr | EPSILON
void Simple_Expr();         // Term Simple_Expr_Prime 
void Simple_Expr_Prime();   // ADD_OP Term Simple_Expr_Prime | EPISILON
void Term();                // Factor Term_Prime
void Term_Prime();          // MUL_OP Factor Term_Prime | EPSILON
void Factor();              // IDENTIFIER | NUM_CONST
// Note: EPSILON is not a real token, we move on to the next Production with the last token read. 

void error(TokenType type);
void errorWithString(TokenType type, char *name);
void errorFollow(Rule_Numbers rule_number);
int inSyncSet(Rule_Numbers rule_number);

void _getNextToken()
{
  prevToken = token;
  token = getNextToken(fin);
}

int parse(FILE *fin)
{
  // fin is an open file pointer to the preprocessed input file
  if (fin == NULL)
  {
    fprintf(stderr, "Error: Invalid file pointer!\n");
    exit(EXIT_FAILURE);
  }

  printf("Begin Parser: \n");

  Program();
  return 0;
}

void Program()
{
  _getNextToken();

  if (strcmp(token->token_name, "int") == 0)
  {
    _getNextToken();

    if (strcmp(token->token_name, "main") == 0)
    {
      _getNextToken();

      if (token->type == LEFT_PAREN)
      {
        _getNextToken();

        if (token->type == RIGHT_PAREN)
        {
          _getNextToken();

          if (token->type == LEFT_CURLY_BRACE)
          {
            Declarations();
            Stmt_List();

            _getNextToken();
            if (token->type == RIGHT_CURLY_BRACE)
            {

              _getNextToken();
              if (token->type == END_OF_FILE)
              {
                printf("Parsing Sucessful!\n");
                return;
              }
              else
                error(END_OF_FILE);
            }
            else
              error(RIGHT_CURLY_BRACE);
          }
          else
            error(LEFT_CURLY_BRACE);
        }
        else
          error(RIGHT_PAREN);
      }
      else
        error(LEFT_PAREN);
    }
    else
      errorWithString(FUNCTION, "main");
  }
  else
    errorWithString(DATA_TYPE, "int");
}

void Declarations() //Produces EPSILON
{
  _getNextToken();

  //Checking FIRST set of Declarations
  if (isDataType(token->token_name))
  {
    Data_Type();
    Id_List();

    _getNextToken();
    if (token->type == SEMI_COLON)
    {
      Declarations();
    }
    else
      error(SEMI_COLON);
  }
  else if (inSyncSet(Follow_Declarations))
  {
    ungetToken(fin, token);
    return;
  }
  else
    errorFollow(Follow_Declarations);
}

void Data_Type()
{
  if (isDataType(token->token_name))
  {
    return;
  }
  else
    error(DATA_TYPE);
}

void Id_List()
{
  _getNextToken();

  if (token->type == IDENTIFIER)
  {
    Id_List_Prime();
  }
  else
    error(IDENTIFIER);
}

void Id_List_Prime() //Produces EPSILON
{
  _getNextToken();

  if (token->type == COMMA)
  {
    Id_List();
  }
  else if (token->type == LEFT_SQUARE_BRACKET){
    _getNextToken();

    if (token->type == NUM_CONST){
      _getNextToken();

      if(token->type == RIGHT_SQUARE_BRACKET){
        Id_List_Prime_Prime();
      }
      else 
        error(RIGHT_SQUARE_BRACKET);
    }
    else 
      error(NUM_CONST);
  }
  else if (inSyncSet(Follow_Id_List_Prime))
  {
    // This means we produced an EPSILON
    ungetToken(fin, token);
    return;
  }
  else
    errorFollow(Follow_Id_List_Prime);
}

void Id_List_Prime_Prime(){ //Produces EPISLON
  _getNextToken();

  if(token->type == COMMA){
    Id_List();
  }
  else if(inSyncSet(Follow_Id_List_Prime_Prime)){
    //Produced EPSILON
    ungetToken(fin, token);
    return;
  }
  else errorFollow(Follow_Id_List_Prime_Prime);
}


void Stmt_List(){ //Produces EPSILON
  _getNextToken();
  
  //Check FIRST of Stmt_List
  if(token->type == IDENTIFIER){
    Stmt();
    Stmt_List();
  }
  else if(inSyncSet(Follow_Stmt_List)){
    //Produced EPSILON
    ungetToken(fin, token);
    return;
  }
  else errorFollow(Follow_Stmt_List);
}

void Stmt(){
  // Checking FIRST of Stmt
  if(token->type == IDENTIFIER){
    Assign_Stmt();

    _getNextToken();
    if(token->type == SEMI_COLON){
      return;
    }
    else error(SEMI_COLON);
  }
  else error(IDENTIFIER);
}


void Assign_Stmt(){
  //Check first of Assign_Stmt
  if(token->type == IDENTIFIER){
    _getNextToken();

    if(token->type == ASSIGN_OP){
      Expr();
    }
    else error(ASSIGN_OP);
  }
  else error(IDENTIFIER);
}

void Expr(){
  Simple_Expr();
  Expr_Prime();
}

void Expr_Prime(){ //Produces EPSILON
  _getNextToken();

  //Check FIRST set of Expr_Prime
  if(token->type == REL_OP){
    Simple_Expr();
  }
  else if(inSyncSet(Follow_Expr_Prime)){
    //Produced EPSILON
    ungetToken(fin, token);
    return;
  }
  else errorFollow(Follow_Expr_Prime);
}

void Simple_Expr(){
  Term();
  Simple_Expr_Prime();
}

void Simple_Expr_Prime(){ //Produces EPSILON
  _getNextToken();

  //Check FIRST set of Simple_Expr_Prime
  if(token->type == ADD_OP){
    Term();
    Simple_Expr_Prime();
  }
  else if(inSyncSet(Follow_Simple_Expr_Prime)){
    //Produced EPSILON
    ungetToken(fin, token);
    return;
  }
  else errorFollow(Follow_Simple_Expr_Prime);
}

void Term(){
  Factor();
  Term_Prime();
}

void Term_Prime(){
  _getNextToken();

  //Check FIRST set of Term_Prime
  if(token->type == MUL_OP){
    Factor();
    Term_Prime();
  }
  else if(inSyncSet(Follow_Term_Prime)){
    ungetToken(fin, token);
    return;
  }
  else errorFollow(Follow_Term_Prime);
}

void Factor(){
  _getNextToken();

  if(token->type == IDENTIFIER || token->type == NUM_CONST)
    return;
  else {
    printf("Should have been either, fix this ambiguity!");
    error(IDENTIFIER);
    error(NUM_CONST);
  }
}

int inSyncSet(Rule_Numbers rule_number)
{
  // Used by productions that generate EPSILON to check if token read
  // is in it's Follow/Sync set or not.
  for (int i = 0; i < LEN_FOLLOW_SET[rule_number]; i++)
    if (FollowSet[rule_number][i] == token->type)
      return 1;

  return 0;
}

void error(TokenType type)
{
  printf("Row: %d, Col: %d -- ERROR: Expected %s after %s: %s\n", token->row, token->col, TokenTypeToString(type), TokenTypeToString(prevToken->type), prevToken->token_name);
  exit(EXIT_FAILURE);
}

void errorWithString(TokenType type, char *name)
{
  printf("Row: %d, Col: %d -- ERROR: Expected %s: %s after %s: %s\n", token->row, token->col, TokenTypeToString(type), name, TokenTypeToString(prevToken->type), prevToken->token_name);
  exit(EXIT_FAILURE);
}

void errorFollow(Rule_Numbers rule_number)
{
  // For productions that produce EPSILON, if the token we read isn't part of it's follow set,
  // Tell the user we expected to see a token from it's follow/synch set
  printf("Row: %d, Col: %d -- ERROR: Expected {", token->row, token->col);
  for (int i = 0; i < LEN_FOLLOW_SET[rule_number]; i++)
  {
    printf(" %s ", TokenTypeToString(FollowSet[rule_number][i]));

    // Beautify :P
    if (i + 1 != LEN_FOLLOW_SET[rule_number])
      printf("or ");
    else
      printf("}\n");
  }

  exit(EXIT_FAILURE);
}

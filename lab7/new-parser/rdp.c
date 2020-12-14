/*
Recursive Descent Parser for the subset of C grammar in Q7.1
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"

#define NUM_OF_RULES 7

typedef enum
{
  Follow_Program,
  Follow_Declarations,
  Follow_Data_Type,
  Follow_Id_List,
  Follow_Id_List_Prime,
  Follow_Assign_Stmt,
  Follow_Assign_Stmt_Prime
} Rule_Numbers;

int LEN_FOLLOW_SET[NUM_OF_RULES] = {1, 1, 1, 1, 1, 1, 1};

TokenType FollowSet[NUM_OF_RULES][5] =
    {{END_OF_FILE},
     {IDENTIFIER},
     {IDENTIFIER},
     {SEMI_COLON},
     {SEMI_COLON},
     {RIGHT_CURLY_BRACE},
     {RIGHT_CURLY_BRACE}};

FILE *fin;
Token *prevToken;
Token *token;

void Program();           // Program --> int main () { Declarations Assign_Stmt }
void Declarations();      // Declarations --> Data_Type Id_List ; Declarations | EPSILON
void Data_Type();         // Data_Type --> int | char
void Id_List();           // Id_List --> id Id_List_Prime
void Id_List_Prime();     //Id_List_Prime --> , Id_List | EPSILON
void Assign_Stmt();       // Assign_Stmt --> id = Assign_Stmt_Prime
void Assign_Stmt_Prime(); //Assign_Stmt_Prime --> id ; | num ;

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

  printf("Parsing\n");

  /*
  while(1){
    _getNextToken();
    displayToken(token);

    if (token->type == END_OF_FILE)
      break;
  }*/

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
            Assign_Stmt();

            _getNextToken();
            if (token->type == RIGHT_CURLY_BRACE)
            {

              _getNextToken();
              if (token->type == END_OF_FILE)
              {
                printf("Parsing Sucessful!\n");
                exit(EXIT_SUCCESS);
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

  //Check FIRST set of Declarations
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
    return;
  else
    errorFollow(Follow_Declarations);
}

void Data_Type()
{
  _getNextToken();

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

void Id_List_Prime()
{
  _getNextToken();

  if (token->type == COMMA)
  {
    Id_List();
  }
  else if (inSyncSet(Follow_Id_List_Prime))
    return;
  else
    errorFollow(Follow_Id_List_Prime);
}

void Assign_Stmt()
{

  _getNextToken();

  if (token->type == IDENTIFIER)
  {
    _getNextToken();

    if (token->type == ASSIGN_OP)
    {
      Assign_Stmt_Prime();
    }
    else
      error(ASSIGN_OP);
  }
  else
    error(IDENTIFIER);
}

void Assign_Stmt_Prime()
{
  _getNextToken();

  if (token->type == IDENTIFIER || token->type == NUM_CONST)
  {
    _getNextToken();

    if (token->type == SEMI_COLON)
    {
      return;
    }
    else
      error(SEMI_COLON);
  }
  else
    error(IDENTIFIER); //TODO: Make this a variadic function
}

int inSyncSet(Rule_Numbers rule_number)
{
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
  printf("Row: %d, Col: %d -- ERROR: Expected ", token->row, token->col);
  for (int i = 0; i < LEN_FOLLOW_SET[rule_number]; i++)
    printf("%s, ", TokenTypeToString(FollowSet[rule_number][i]));

  exit(EXIT_FAILURE);
}
#ifndef ST_H_
#define ST_H_

#define LEN_NAME 64
#define LEN_DATATYPE 16
#define LEN_SYMBOL_BUFFER 16

extern char symbol_buffer[LEN_SYMBOL_BUFFER];

typedef struct Symbol {
  int index;
  char lexeme_name[LEN_NAME];
  char data_type[LEN_DATATYPE];
  int size;
}Symbol;

Symbol* createSymbolTable();
int addEntry(Symbol* symbol_table, Token* token);
int symbolExists(Symbol *symbol_table, Token* token);

#endif //ST_H_

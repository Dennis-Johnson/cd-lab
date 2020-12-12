#ifndef ST_H_
#define ST_H_

#define NUM_LOCAL_TABLES 5
#define TABLE_LENGTH 10

#define LEN_NAME 64
#define LEN_DATATYPE 16
#define LEN_SYMBOL_BUFFER 16

typedef struct Symbol {
  int index;
  char lexeme_name[LEN_NAME];
  char data_type[LEN_DATATYPE];
  int size;
}Symbol;


// Implemented in st.c 
extern char data_type_buffer[LEN_SYMBOL_BUFFER];
extern int local_st_index;
extern Symbol* LocalSymbolTables[NUM_LOCAL_TABLES];

Symbol* createSymbolTable();
int addEntry(Token* token);
int symbolExists(Symbol *symbol_table, Token* token);
void displaySymbolTable(Symbol *st);
void displayAllLocalSymbolTables();

#endif //ST_H_

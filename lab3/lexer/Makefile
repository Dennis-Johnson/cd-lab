cc = gcc
flags = -Wall -pedantic

all: lexer

lexer: main.o preprocess.o token.o
	$(cc) $(flags) main.o preprocess.o token.o -o lexer

main: main.o 
	$(cc) $(flags) -c main.c

preprocess: preprocess.o
	$(cc) $(flags) -c preprocess.c

token: token.o
	$(cc) $(flags) -c token.c

clean: 
	rm lexer *.o


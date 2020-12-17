cc = gcc
flags = -g -Wall -pedantic

all: lexer

lexer: main.o token.o st.o
	$(cc) $(flags) main.o token.o st.o -o lexer

main: main.o 
	$(cc) $(flags) -c main.c

token: token.o
	$(cc) $(flags) -c token.c

st: st.o
	$(cc) $(flags) -c st.c

clean: 
	rm main lexer *.o


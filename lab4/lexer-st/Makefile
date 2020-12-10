cc = clang
flags = -Wall -pedantic

all: lexer

lexer: main.o preprocess.o token.o st.o
	$(cc) $(flags) main.o preprocess.o token.o st.o -o lexer

main: main.o 
	$(cc) $(flags) -c main.c

preprocess: preprocess.o
	$(cc) $(flags) -c preprocess.c

token: token.o
	$(cc) $(flags) -c token.c

st: st.o
	$(cc) $(flags) -c st.c

clean: 
	rm lexer *.o


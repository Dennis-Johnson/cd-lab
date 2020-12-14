cc = gcc
flags = -Wall -g -pedantic

all: parser

parser: main.o preprocess.o token.o st.o rdp.o
	$(cc) $(flags) main.o preprocess.o token.o st.o rdp.o -o parser

main: main.o 
	$(cc) $(flags) -c main.c

preprocess: preprocess.o
	$(cc) $(flags) -c preprocess.c

token: token.o
	$(cc) $(flags) -c token.c

st: st.o
	$(cc) $(flags) -c st.c

rdp: rdp.o
	$(cc) $(flags) -c rdp.c

clean: 
	rm -rf *.dSYM main parser *.o


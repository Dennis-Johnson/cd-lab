cc = gcc
flags = -pedantic -Wall -o2
target = rdp

all: $(target)

$(target): main.o lexer.o parser.o
	$(cc) $(flags) main.o lexer.o parser.o -o $(target)

main: main.c
	$(cc) $(flags) -c main.c

lexer: lexer.c
	$(cc) $(flags) -c lexer.c

parser: parser.c
	$(cc) $(flags) -c parser.c

clean:
	rm -rf .dSYM *.o $(target)
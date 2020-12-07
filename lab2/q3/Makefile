cc = gcc 
flags = -Wall -pedantic
debug_flags = -g -Wall -pedantic
all: main

main: main.c
	$(cc) $(flags) -o main main.c

debug: main.c 
	$(cc) $(debug_flags) -g -o debug  main.c

clean: 
	rm main debug a.out
	rm -rf *.dSYM

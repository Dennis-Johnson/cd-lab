CC = clang
CFLAGS = -pedantic -Wall
TARGET = rdp

all: $(TARGET)

$(TARGET): main.o lexer.o parser.o
	$(CC) $(CFLAGS) main.o lexer.o parser.o -o $(TARGET)

main: main.c
	$(CC) $(CFLAGS) -c main.c

lexer: lexer.c
	$(CC) $(CFLAGS) -c lexer.c

parser: parser.c
	$(CC) $(CFLAGS) -c parser.c

clean:
	rm -f *.o $(TARGET)
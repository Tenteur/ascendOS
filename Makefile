CC=gcc
CFLAGS=-Iinclude -Wall

SRC=$(wildcard src/*.c)
OBJ=$(SRC:.c=.o)

main: $(OBJ)
	$(CC) -o build/$@ $^

clean:
	rm -f src/*.o build/main

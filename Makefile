# Compiler
CC = gcc

# SDL2 configuration (evaluated at Makefile parse time)
SDL_CFLAGS := $(shell sdl2-config --cflags)
SDL_LIBS   := $(shell sdl2-config --libs)

# Additional SDL2-related libs
LIBS = $(SDL_LIBS) -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm

# Compiler flags
CFLAGS = -Wall -Wextra -Iinclude $(SDL_CFLAGS)

# Files
SRC = $(filter-out src/jsoncParser.c, $(wildcard src/*.c))
OBJ = $(SRC:.c=.o)

# Output binary
TARGET = build/myapp

# Default rule
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	rm -f src/*.o $(TARGET)

# Compiler and flags
CC      := gcc
CFLAGS  := -Wall -Wextra -Iinclude \
            $(shell pkg-config --cflags sdl3) \
            $(shell pkg-config --cflags yaml-0.1)
CFLAGS += -g -O0
LANGUAGE := C
export LANGUAGE

# SDL3 and related libraries
SDL_LIBS := $(shell pkg-config --libs sdl3)
YAML_LIBS := $(shell pkg-config --libs yaml-0.1)
LIBS     := $(SDL_LIBS) -lSDL3_image -lSDL3_ttf -lSDL3_mixer -lm $(YAML_LIBS)

# Source and object files
SRC := $(filter-out src/jsoncParser.c, $(wildcard src/*.c))
OBJ := $(SRC:.c=.o)

# Output binary
TARGET := build/myapp

.PHONY: all debug release clean

all: CFLAGS += -g -fno-omit-frame-pointer
all: $(TARGET)

release: CFLAGS += -O3
release: clean all

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	rm -f src/*.o $(TARGET)
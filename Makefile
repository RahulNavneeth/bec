CC = clang
CFLAGS = -std=c11 -O3 -g -Wall
CFLAGS += -Iinclude

BIN = bin
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c,$(BIN)/src/%.o,$(SRC))

.PHONY: all clean run

all: clean dirs exec

dirs:
	mkdir -p $(BIN)/src

exec: $(OBJ)
	ar rcs $(BIN)/libbec.a $^

$(BIN)/src/%.o: src/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

run: all
	$(BIN)/main

clean:
	rm -rf $(BIN)

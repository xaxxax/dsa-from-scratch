# Makefile for Project 7 — HashSet (composes your Lesson 6 HashMap)
#
#   make test / make / make clean
#
# Multi-module build: compiles this project's src/ AND ../06-hashmap/src/. A set
# is just a map that ignores values, so HashSet HAS-A HashMap. Your Lesson 6 must
# compile for this to build and be implemented for these tests to pass.

CC      := cc
CFLAGS  := -std=c11 -Wall -Wextra -Wpedantic -g -fsanitize=address,undefined

DEP_SRC := ../06-hashmap/src/hashmap.c
INCLUDE := -Isrc -I../06-hashmap/src -I../../docs

SRC  := $(wildcard src/*.c)
TEST := $(wildcard tests/*.c)
BIN  := out_test

$(BIN): $(SRC) $(DEP_SRC) $(TEST)
	$(CC) $(CFLAGS) $(INCLUDE) $(SRC) $(DEP_SRC) $(TEST) -o $(BIN)

.PHONY: test clean
test: $(BIN)
	./$(BIN)

clean:
	rm -f $(BIN)
	rm -rf *.dSYM

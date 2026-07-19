# Makefile for Project 4 — Stack (composes your Lesson 1 Dynamic Array)
#
#   make test    build (ASan + UBSan) and run
#   make         build only (for valgrind)
#   make clean   remove build artifacts
#
# COMPOSITION / MULTI-MODULE BUILD: this project does not reinvent storage — it
# reuses your Lesson 1 dynamic array. So the build compiles TWO source trees:
# this project's src/ AND ../01-dynamic-array/src/. That's why DEP_SRC and the
# extra -I are here. It also means: your Lesson 1 must at least COMPILE for this
# to build, and must be fully IMPLEMENTED for these tests to pass. That coupling
# is the lesson — real systems are built from modules that depend on each other.

CC      := cc
CFLAGS  := -std=c11 -Wall -Wextra -Wpedantic -g -fsanitize=address,undefined

DEP_SRC := ../01-dynamic-array/src/dynamic_array.c
INCLUDE := -Isrc -I../01-dynamic-array/src -I../../docs

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

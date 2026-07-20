# Makefile for Project 13 — Breadth-First Search (composes your Lesson 11 Graph)
#
#   make test / make / make clean
#
# Multi-module build: compiles this project's src/ AND ../11-graph/src/. BFS
# traverses the adjacency list via graph_neighbors(). Your Lesson 11 must compile
# for this to build and be implemented for these tests to pass. The visited array
# and the FIFO frontier queue are kept INTERNAL here (a simple array/ring buffer),
# not composed from Lesson 5, to keep the build simple.

CC      := cc
CFLAGS  := -std=c11 -Wall -Wextra -Wpedantic -g -fsanitize=address,undefined

DEP_SRC := ../11-graph/src/graph.c
INCLUDE := -Isrc -I../11-graph/src -I../../docs

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

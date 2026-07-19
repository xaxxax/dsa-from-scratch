# Makefile for Project 5 — Queue (composes your Lesson 2 Singly Linked List)
#
#   make test / make / make clean
#
# Like Lesson 4, this is a multi-module build: it compiles this project's src/
# AND ../02-singly-linked-list/src/. The queue reuses your linked list as its
# backing store — enqueue == add_last (O(1) via tail), dequeue == remove_first
# (O(1)). Your Lesson 2 must compile for this to build, and be implemented for
# these tests to pass.

CC      := cc
CFLAGS  := -std=c11 -Wall -Wextra -Wpedantic -g -fsanitize=address,undefined

DEP_SRC := ../02-singly-linked-list/src/linked_list.c
INCLUDE := -Isrc -I../02-singly-linked-list/src -I../../docs

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

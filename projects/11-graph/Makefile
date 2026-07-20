# Makefile for Project 11 — Graph (adjacency-list representation)
#
#   make test    build (with AddressSanitizer + UBSan) and run the tests
#   make         build the test binary without running it (for valgrind)
#   make clean   remove build artifacts
#
# Why these flags: -Wall -Wextra -Wpedantic surface real bugs (treat warnings as
# errors to fix); -g adds line numbers to sanitizer/valgrind/gdb output;
# -fsanitize=address,undefined instruments the binary so memory bugs crash loudly
# instead of corrupting silently. A sanitizer report is a FAILING test.

CC      := cc
CFLAGS  := -std=c11 -Wall -Wextra -Wpedantic -g -fsanitize=address,undefined
INCLUDE := -Isrc -I../../docs

SRC  := $(wildcard src/*.c)
TEST := $(wildcard tests/*.c)
BIN  := out_test

$(BIN): $(SRC) $(TEST)
	$(CC) $(CFLAGS) $(INCLUDE) $(SRC) $(TEST) -o $(BIN)

.PHONY: test clean
test: $(BIN)
	./$(BIN)

clean:
	rm -f $(BIN)
	rm -rf *.dSYM

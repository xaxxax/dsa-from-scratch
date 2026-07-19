/*
 * test_utils.h — a tiny, zero-dependency test helper for this course (C11).
 *
 * WHY THIS EXISTS:
 *   You could pull in a framework (Unity, Check, Criterion), but then you'd spend
 *   your first hour fighting a build instead of learning data structures. This one
 *   header gives you enough to test rigorously: value checks, boolean checks, and
 *   a pass/fail summary at the end.
 *
 * HOW TO USE IT:
 *   1. #include "test_utils.h" from your test file. The per-project Makefile adds
 *      docs/ to the include path (-I../../docs), so the short name resolves.
 *   2. Call the CHECK_* macros, then return test_summary() from main().
 *
 *   Example (tests/test_dynamic_array.c):
 *
 *       #include "test_utils.h"
 *       #include "dynamic_array.h"
 *
 *       int main(void) {
 *           DynamicArray *a = da_new();
 *           CHECK_INT(0, da_size(a), "new array is empty");
 *           da_add(a, 42);
 *           CHECK_INT(1, da_size(a), "size grows after add");
 *           CHECK_INT(42, da_get(a, 0), "stored value is retrievable");
 *           da_free(a);
 *           return test_summary();   // non-zero exit if any check failed
 *       }
 *
 *   Build & run (from the project folder):
 *       make test
 *
 * NOTE ON ERROR CASES:
 *   C has no exceptions. If your structure signals "index out of bounds" with
 *   assert() (a hard abort), you cannot catch it in a unit test the way Java's
 *   throwsException did — an abort kills the whole test binary. So test the
 *   *happy path and edge values you can reach*, and verify the abort paths by
 *   running the program by hand. If instead you signal errors with return codes,
 *   you CAN test them with CHECK_INT on the returned status. Which approach to
 *   use is a design decision each lesson asks you to make.
 *
 * GRADUATION GOAL:
 *   Once comfortable, rewrite a project's tests in a real framework (Unity is a
 *   good next step). Knowing what a framework does for you — and doing without it
 *   first — is part of the education.
 */
#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <stdio.h>
#include <string.h>

/* File-local counters. Fine because each test binary has exactly one .c with a
 * main() that includes this header. */
static int tests_passed = 0;
static int tests_failed = 0;

/* Generic boolean check. */
#define CHECK(cond, msg)                                                      \
    do {                                                                      \
        if (cond) {                                                           \
            tests_passed++;                                                   \
            printf("  PASS  %s\n", (msg));                                    \
        } else {                                                             \
            tests_failed++;                                                   \
            printf("  FAIL  %s   [%s:%d]\n", (msg), __FILE__, __LINE__);       \
        }                                                                     \
    } while (0)

/* Integer equality (works for any integer/char type; promoted to long). */
#define CHECK_INT(expected, actual, msg)                                      \
    do {                                                                      \
        long _e = (long)(expected);                                          \
        long _a = (long)(actual);                                            \
        if (_e == _a) {                                                       \
            tests_passed++;                                                   \
            printf("  PASS  %s\n", (msg));                                    \
        } else {                                                             \
            tests_failed++;                                                   \
            printf("  FAIL  %s   [expected %ld but got %ld]  [%s:%d]\n",       \
                   (msg), _e, _a, __FILE__, __LINE__);                         \
        }                                                                     \
    } while (0)

/* C-string equality via strcmp. */
#define CHECK_STR(expected, actual, msg)                                      \
    do {                                                                      \
        const char *_e = (expected);                                         \
        const char *_a = (actual);                                           \
        if (_e && _a && strcmp(_e, _a) == 0) {                                \
            tests_passed++;                                                   \
            printf("  PASS  %s\n", (msg));                                    \
        } else {                                                             \
            tests_failed++;                                                   \
            printf("  FAIL  %s   [expected \"%s\" but got \"%s\"]  [%s:%d]\n",  \
                   (msg), _e ? _e : "(null)", _a ? _a : "(null)",             \
                   __FILE__, __LINE__);                                        \
        }                                                                     \
    } while (0)

/* Call once at the end of main() and return its value. */
static int test_summary(void) {
    printf("------------------------------------------------------------\n");
    printf("%d passed, %d failed.\n", tests_passed, tests_failed);
    if (tests_failed > 0) {
        printf("SOME TESTS FAILED.\n");
        return 1;   /* non-zero exit so make/CI notice */
    }
    printf("ALL TESTS PASSED.\n");
    return 0;
}

#endif /* TEST_UTILS_H */

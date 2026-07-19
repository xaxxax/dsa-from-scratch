/*
 * test_stack.c — YOUR test harness for Project 4.
 *
 * Run:        make test
 * Leak audit: make && valgrind --leak-check=full --error-exitcode=1 ./out_test
 *
 * NOTE: these tests exercise your Lesson 1 dynamic array THROUGH the stack. If
 * they fail with a TODO-assert from dynamic_array.c, finish Lesson 1 first.
 */
#include "test_utils.h"
#include "stack.h"

int main(void) {

    Stack *s = stack_new();
    CHECK_INT(0, stack_size(s), "new stack size 0");
    CHECK(stack_is_empty(s), "new stack is empty");

    stack_push(s, 1);
    stack_push(s, 2);
    stack_push(s, 3);   /* bottom [1,2,3] top */
    CHECK_INT(3, stack_size(s), "size 3 after three pushes");
    CHECK(!stack_is_empty(s), "not empty after pushes");

    CHECK_INT(3, stack_peek(s), "peek sees top (3) without removing");
    CHECK_INT(3, stack_size(s), "peek did not change size");

    CHECK_INT(3, stack_pop(s), "pop returns 3 (LIFO)");
    CHECK_INT(2, stack_pop(s), "pop returns 2");
    CHECK_INT(1, stack_size(s), "size 1 after two pops");
    CHECK_INT(1, stack_peek(s), "1 is now on top");

    CHECK_INT(1, stack_pop(s), "pop returns 1");
    CHECK(stack_is_empty(s), "empty after popping everything");

    /* push again after emptying — the structure must recover */
    stack_push(s, 42);
    CHECK_INT(42, stack_peek(s), "reusable after empty");

    stack_free(s);   /* frees backing array AND handle — zero leaks */

    /* ====================================================================== */
    /* TODO: push/pop many (100+) to force the backing array to grow, and      */
    /* confirm LIFO order survives; test your empty-pop / empty-peek policy;   */
    /* stack_free on a brand-new stack.                                        */
    /* ====================================================================== */

    return test_summary();
}

/*
 * stack.c — YOUR stack, built by COMPOSING the Lesson 1 dynamic array.
 *
 * SCAFFOLD: fill each TODO (README §4). Because you're delegating to da_*, the
 * bodies are short — the learning here is conceptual (composition, why the top
 * maps to the array's END), not algorithmic. Still: get the ownership right.
 * stack_free must free the BACKING array and then the Stack struct.
 *
 * Almost every operation is one line delegating to a da_* call. The skill is
 * choosing the RIGHT da_* call and index so that "top" is always the last slot.
 */
#include "stack.h"

#include <assert.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ M1 ---- */

Stack *stack_new(void) {
    /* TODO (M1): malloc the Stack; set s->backing = da_new(). Check both for
     * NULL (if da_new fails, don't leak the Stack). */
    assert(0 && "TODO: implement stack_new (README M1)");
    return NULL;
}

void stack_free(Stack *s) {
    (void)s;
    /* TODO (M1): if s is NULL, nothing. Else da_free(s->backing) THEN free(s).
     * You own the backing array — releasing it is YOUR job, not the array's. */
    assert(0 && "TODO: implement stack_free (README M1)");
}

/* ------------------------------------------------------------------ M2 ---- */

size_t stack_size(const Stack *s) {
    (void)s;
    /* TODO (M2): delegate to da_size(s->backing). */
    assert(0 && "TODO: implement stack_size (README M2)");
    return 0;
}

int stack_is_empty(const Stack *s) {
    (void)s;
    /* TODO (M2): true iff size == 0 (or delegate to da_is_empty). */
    assert(0 && "TODO: implement stack_is_empty (README M2)");
    return 0;
}

/* ------------------------------------------------------------------ M3 ---- */

void stack_push(Stack *s, stack_elem item) {
    (void)s; (void)item;
    /* TODO (M3): da_add appends at the END — which IS the top. One line. */
    assert(0 && "TODO: implement stack_push (README M3)");
}

/* ------------------------------------------------------------------ M4 ---- */

stack_elem stack_peek(const Stack *s) {
    (void)s;
    /* TODO (M4): empty-stack policy (assert vs. status). Otherwise the top is
     * da_get(s->backing, size - 1). Beware: size - 1 on an empty stack is the
     * unsigned trap (wraps to SIZE_MAX) — check emptiness FIRST. */
    assert(0 && "TODO: implement stack_peek (README M4)");
    return 0;
}

/* ------------------------------------------------------------------ M5 ---- */

stack_elem stack_pop(Stack *s) {
    (void)s;
    /* TODO (M5): empty check first. Then remove-and-return the last element:
     * da_remove(s->backing, size - 1) already returns the removed value, and
     * removing the LAST index shifts nothing — so pop is O(1). */
    assert(0 && "TODO: implement stack_pop (README M5)");
    return 0;
}

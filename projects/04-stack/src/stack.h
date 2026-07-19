/*
 * stack.h — the CONTRACT for your LIFO stack.
 *
 * The big idea of this lesson is NOT a new storage mechanism — it's COMPOSITION.
 * A stack HAS-A dynamic array (your Lesson 1) and exposes only four operations:
 * push, pop, peek, is_empty. By hiding da_get/da_insert/da_remove-at-index, the
 * stack makes LIFO trivial and misuse (poking the middle) impossible. Restricting
 * the interface is a FEATURE.
 *
 *      push ─┐   ┌─ pop / peek        (both act on the TOP)
 *            ▼   ▼
 *          ┌───────┐  <- top  (== the END of the backing array, where the
 *          │   D   │             dynamic array is O(1) amortized)
 *          │   C   │
 *          │   B   │
 *          │   A   │  <- bottom (index 0)
 *          └───────┘
 *
 * Why the end and not the front? Pushing/popping the FRONT of an array is O(n)
 * (shift everything); the END is O(1). The whole trick is mapping "top" to "end".
 */
#ifndef STACK_H
#define STACK_H

#include <stddef.h>
#include "dynamic_array.h"   /* found via -I../01-dynamic-array/src */

/* The stack stores whatever the dynamic array stores. */
typedef da_elem stack_elem;

/* HAS-A, not IS-A: we hold a DynamicArray*, we do not "extend" it. In C there's
 * no inheritance anyway — composition is the only tool, and it's the right one. */
typedef struct {
    DynamicArray *backing;
} Stack;

Stack     *stack_new(void);
void       stack_free(Stack *s);

void       stack_push(Stack *s, stack_elem item);  /* add on top      */
stack_elem stack_pop (Stack *s);                   /* remove + return top */
stack_elem stack_peek(const Stack *s);             /* look at top, no remove */

size_t     stack_size    (const Stack *s);
int        stack_is_empty(const Stack *s);

#endif /* STACK_H */

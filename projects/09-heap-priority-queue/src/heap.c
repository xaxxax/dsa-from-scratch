/*
 * heap.c — YOUR binary min-heap. SCAFFOLD (README §4).
 *
 * Implement milestone by milestone under ASan. The invariant to restore after
 * EVERY push/pop:
 *      heap property:  data[parent(i)] <= data[i]  for all i > 0
 * The whole game is: make the change, then sift to restore the property.
 *
 * All the "tree" navigation is INDEX ARITHMETIC — there are no node pointers.
 * Beware the size_t unsigned trap in parent(): (0 - 1) / 2 underflows, so never
 * call parent(0); the root has no parent, and your sift-up loop must stop at i==0.
 */
#include "heap.h"

#include <assert.h>
#include <stdlib.h>

#define HEAP_DEFAULT_CAPACITY 16

/* ---- index helpers (pure arithmetic; a complete tree needs no pointers) --- */
static size_t parent_of(size_t i) { return (i - 1) / 2; }  /* never call with i==0 */
static size_t left_of  (size_t i) { return 2 * i + 1; }
static size_t right_of (size_t i) { return 2 * i + 2; }

/* Swap two slots — you'll use this constantly while sifting. */
static void swap(heap_elem *a, heap_elem *b) {
    heap_elem t = *a; *a = *b; *b = t;
}

/* Grow the backing array (double capacity). Same realloc-may-move care as the
 * dynamic array in Lesson 1. Unused until heap_push calls it (M2) — the "defined
 * but not used" warning until then is expected. */
static void heap_grow(MinHeap *h) {
    (void)h;
    /* TODO (M2): realloc data to capacity*2 (handle capacity 0), check NULL,
     * adopt the (possibly new) pointer, update capacity. */
    assert(0 && "TODO: implement heap_grow (README M2)");
}

/* ------------------------------------------------------------------ M1 ---- */

MinHeap *heap_new(void) {
    /* TODO (M1): malloc the MinHeap; malloc data of HEAP_DEFAULT_CAPACITY;
     * size = 0. Check allocations (free the struct if the buffer fails). */
    assert(0 && "TODO: implement heap_new (README M1)");
    return NULL;
}

void heap_free(MinHeap *h) {
    (void)h;
    /* TODO (M1): if NULL, nothing. Else free(h->data), then free(h). */
    assert(0 && "TODO: implement heap_free (README M1)");
}

/* ------------------------------------------------------------------ M2 ---- */

void heap_push(MinHeap *h, heap_elem item) {
    (void)h; (void)item;
    /* TODO (M2): if size == capacity, heap_grow(h). Place item at data[size]
     * (the next free leaf); size++. Then SIFT UP: let i = size-1; while i > 0
     * and data[i] < data[parent_of(i)], swap them and set i = parent_of(i).
     * The new element bubbles up until its parent is smaller (or it's the root). */
    assert(0 && "TODO: implement heap_push (README M2)");
}

/* ------------------------------------------------------------------ M3 ---- */

heap_elem heap_peek(const MinHeap *h) {
    (void)h;
    /* TODO (M3): empty policy first (assert or status). Otherwise the min is
     * ALWAYS data[0] — O(1), no search. That's the whole point of a heap. */
    assert(0 && "TODO: implement heap_peek (README M3)");
    return 0;
}

/* ------------------------------------------------------------------ M4 ---- */

heap_elem heap_pop(MinHeap *h) {
    (void)h;
    /* TODO (M4): empty check. Save min = data[0]. Move the LAST leaf to the root
     * (data[0] = data[size-1]); size--. Then SIFT DOWN from index 0: repeatedly
     * find the smaller of the two children; if it is smaller than the current
     * node, swap and descend into that child; stop when both children are >=
     * current (or there are no children — watch left_of(i) >= size). Return min.
     * Draw the sift-down for a 6-element heap before coding — the "smaller child"
     * choice is where bugs live. */
    assert(0 && "TODO: implement heap_pop (README M4)");
    return 0;
}

/* ------------------------------------------------------------------ M5 ---- */

size_t heap_size(const MinHeap *h) {
    (void)h;
    assert(0 && "TODO: implement heap_size (README M5)");
    return 0;
}

int heap_is_empty(const MinHeap *h) {
    (void)h;
    assert(0 && "TODO: implement heap_is_empty (README M5)");
    return 0;
}

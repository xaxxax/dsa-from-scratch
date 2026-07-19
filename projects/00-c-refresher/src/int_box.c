/*
 * int_box.c — Phase 0 warm-up. A FIXED-capacity, heap-allocated box of ints.
 *
 * This is a SCAFFOLD. The struct, the signatures, and a main() are here to keep
 * you oriented. Every function body marked TODO is yours to write. The goal is to
 * exercise pointers, malloc/free, structs, and the build model BEFORE the dynamic
 * array (Project 1) adds growth (realloc) on top.
 *
 * Build & run with the sanitizers on (from this folder):
 *     cc -std=c11 -Wall -Wextra -g -fsanitize=address,undefined src/int_box.c -o box
 *     ./box
 *
 * Success = your prints look right AND the sanitizer reports no leaks/errors.
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int   *data;      /* heap-allocated backing array */
    size_t size;      /* how many ints are currently stored */
    size_t capacity;  /* how many ints data can hold (fixed for this warm-up) */
} IntBox;

/* Allocate the struct AND its backing array. Decide your policy for capacity 0
 * (return NULL? assert?) and document it. Remember: TWO allocations here. */
IntBox *box_new(size_t capacity) {
    (void)capacity;
    /* TODO: malloc the IntBox; malloc the data array of `capacity` ints;
     *       initialize size = 0; return the box (or NULL on failure). */
    assert(0 && "TODO: implement box_new");
    return NULL;
}

/* Free everything you allocated, in the right order. Why must you free `data`
 * BEFORE `b` itself? (Hint: once b is freed, you can't reach b->data.) */
void box_free(IntBox *b) {
    (void)b;
    /* TODO: free(b->data); then free(b);  (guard against b == NULL) */
    assert(0 && "TODO: implement box_free");
}

/* Append value. Return 0 on success, non-zero if the box is already full. */
int box_push(IntBox *b, int value) {
    (void)b; (void)value;
    /* TODO: if full, return non-zero; else data[size] = value; size++; return 0 */
    assert(0 && "TODO: implement box_push");
    return 0;
}

/* Read the value at index. This is a PROGRAMMER-error check — use assert. */
int box_get(const IntBox *b, size_t index) {
    (void)b; (void)index;
    /* TODO: assert(index < b->size); return b->data[index]; */
    assert(0 && "TODO: implement box_get");
    return 0;
}

size_t box_size(const IntBox *b) {
    (void)b;
    /* TODO: return b->size; */
    assert(0 && "TODO: implement box_size");
    return 0;
}

int box_is_full(const IntBox *b) {
    (void)b;
    /* TODO: return b->size == b->capacity; */
    assert(0 && "TODO: implement box_is_full");
    return 0;
}

int main(void) {
    IntBox *b = box_new(3);

    printf("size=%zu full=%d\n", box_size(b), box_is_full(b));  /* 0, 0 */
    box_push(b, 10);
    box_push(b, 20);
    box_push(b, 30);
    printf("size=%zu full=%d\n", box_size(b), box_is_full(b));  /* 3, 1 */
    printf("push when full returns: %d\n", box_push(b, 40));     /* non-zero */
    printf("get(0)=%d get(2)=%d\n", box_get(b, 0), box_get(b, 2));  /* 10, 30 */

    box_free(b);   /* must leave the sanitizer with zero leaks */
    return 0;
}

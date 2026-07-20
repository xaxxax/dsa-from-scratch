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

// these are needed to be put here because the C compiler only allows
// functions called when they are after other functions
// i.e is_full(), then size(), if trying to call size() inside of is_full()
// this will throw a implicit function declaratio error
IntBox *box_new(size_t capacity); 
void box_free(IntBox *b);
int box_push(IntBox *b, int value);
int box_get(const IntBox *b, size_t index);
size_t box_size(const IntBox *b);
int box_is_full(const IntBox *b);

IntBox *box_new(size_t capacity) {
    if(capacity == 0) return NULL; // unsigned integer so we do not care about c < 0 (neg not possible)

    IntBox *box = malloc(sizeof(IntBox));
    if(!box) return NULL; // malloc may return NULL (0) if no memory assignable
                          // if we no check, and do box->data (data[0]) this will try 
                          // and assign data to address 0, which is 'null pointer dereference' and progrem segfaults 

    box->data = malloc(capacity * sizeof(int)); // follow the box pointer, and access data field inside that struct (this.data)
    if(!box->data) {
      free(box); // if true, then need to free alocated memory for box
      return NULL; // same as above malloc    
    }
    
    box->size = 0;
    box->capacity = capacity;

    return box;
}

void box_free(IntBox *b) {
    if(!b) return; // 'return NULL' fails because NULL is the same as 0, which is not of void return type
    
    free(b->data);
    free(b);
}

int box_push(IntBox *b, int value) {
    if(box_is_full(b)) return -1;
    int index = box_size(b); // size starts at 0, so if length (size) is 1
                             // that means something is at index 0, so next index to push is at 1
                             // could probably just use b->size instead of index, but index reads cleaner? 
    b->data[index] = value;
    b->size++;

    return 0;
}

int box_get(const IntBox *b, size_t index) {
    assert(index <= b->size);
    
    return b->data[index];
}

size_t box_size(const IntBox *b) {
    assert(b != NULL);
    return b->size;
}

int box_is_full(const IntBox *b) {
    if(!b) return -1;
    return b->size == b->capacity;
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

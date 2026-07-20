/*
 * heap.h — the CONTRACT for your binary MIN-heap (a priority queue).
 *
 * A heap is a COMPLETE binary tree with one rule (the heap property): every
 * parent is <= both its children (min-heap). So the SMALLEST element is always at
 * the root — peek is O(1) — while push and pop are O(log n).
 *
 * THE TRICK: no pointers, no nodes. A complete tree maps perfectly onto an array,
 * where for the element at index i:
 *      parent(i) = (i - 1) / 2
 *      left(i)   = 2*i + 1
 *      right(i)  = 2*i + 2
 *
 *   heap (min):        2                 array: [ 2 | 4 | 7 | 9 | 5 | 8 ]
 *                    /   \                 index:  0   1   2   3   4   5
 *                  4       7               2's children (idx1,2) are 4,7 ✓
 *                 / \     /                4's children (idx3,4) are 9,5 ✓
 *                9   5   8
 *
 * A heap is NOT sorted (the array above isn't) — it's just enough order to always
 * surface the min. To restore the property after a change you "sift": UP after a
 * push (bubble the new leaf toward the root), DOWN after a pop (sink the moved
 * root toward the leaves). To make it a MAX-heap, flip every comparison.
 */
#ifndef HEAP_H
#define HEAP_H

#include <stddef.h>

typedef int heap_elem;

/* Array-backed complete tree. Same size/capacity split as the dynamic array —
 * a heap is "a dynamic array kept in heap order," so it grows the same way. */
typedef struct {
    heap_elem *data;
    size_t     size;
    size_t     capacity;
} MinHeap;

/* ---- lifecycle ---------------------------------------------------------- */
MinHeap *heap_new(void);
void     heap_free(MinHeap *h);

/* ---- core operations ---------------------------------------------------- */
void      heap_push(MinHeap *h, heap_elem item);  /* insert, then sift up   */
heap_elem heap_pop (MinHeap *h);                   /* remove+return the min  */
heap_elem heap_peek(const MinHeap *h);             /* the min, no removal    */

/* ---- introspection ------------------------------------------------------ */
size_t heap_size    (const MinHeap *h);
int    heap_is_empty(const MinHeap *h);

#endif /* HEAP_H */

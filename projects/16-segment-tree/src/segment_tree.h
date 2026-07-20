/*
 * segment_tree.h — the CONTRACT for your segment tree (range sum + point update).
 *
 * THE PROBLEM it solves: given an array, answer many "sum of elements in [l, r]"
 * queries WHILE the array is being updated. A prefix-sum array gives O(1) queries
 * but O(n) updates (rebuild the sums). A plain array gives O(1) updates but O(n)
 * queries (loop the range). The segment tree gets BOTH to O(log n).
 *
 * THE IDEA: a binary tree where each node stores the sum of a RANGE. The root
 * covers [0, n-1]; each node splits its range in half between two children; leaves
 * are single elements. A query for [l, r] combines only O(log n) nodes whose ranges
 * tile [l, r]; an update fixes only the O(log n) nodes on one root-to-leaf path.
 *
 *   values [5, 2, 7, 1]:
 *                       [0..3]=15
 *                      /         \
 *                 [0..1]=7      [2..3]=8
 *                 /     \        /     \
 *              [0]=5  [1]=2   [2]=7  [3]=1
 *
 *   query(1,2) = 2 + 7 = 9  → combine node [1] and node [2]  (a few nodes, not a loop)
 *   update(2, 10) → leaf [2], then [2..3], then root recompute upward
 *
 * Like the heap, the tree lives in a FLAT ARRAY (no node pointers) — but it is not
 * complete, so you allocate ~4n slots to be safe (the README explains why 4n).
 * The "combine" operation is SUM here; swapping it for min/max/gcd gives a
 * different query with the same structure — a one-function change.
 */
#ifndef SEGMENT_TREE_H
#define SEGMENT_TREE_H

#include <stddef.h>

typedef struct {
    long   *tree;   /* internal node sums, ~4*n slots (see README) */
    size_t  n;      /* number of leaves = size of the original array */
} SegmentTree;

/* ---- lifecycle ---------------------------------------------------------- */
/* Build a segment tree over `values[0..n-1]` (O(n)). NULL on allocation failure
 * or n == 0 (decide your n==0 policy). */
SegmentTree *seg_new(const int *values, size_t n);
void         seg_free(SegmentTree *t);

/* ---- operations (both O(log n)) ----------------------------------------- */
/* Sum of values over the INCLUSIVE range [l, r]  (0 <= l <= r < n). */
long seg_query(const SegmentTree *t, size_t l, size_t r);

/* Set the element at `index` to `new_value` and repair the affected sums. */
void seg_update(SegmentTree *t, size_t index, int new_value);

size_t seg_size(const SegmentTree *t);   /* n, the number of leaves */

#endif /* SEGMENT_TREE_H */

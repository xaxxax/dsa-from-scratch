/*
 * segment_tree.c — YOUR segment tree. SCAFFOLD (README §4).
 *
 * The recursion pattern is the same for build, query, and update: a helper knows
 * the array index of the current tree node and the [lo, hi] range that node
 * covers, and recurses into two children that split [lo, hi] at mid = (lo+hi)/2.
 *
 * Node indexing (1-based is cleanest): root = 1 covering [0, n-1];
 *   left child  of node = 2*node       covers [lo, mid]
 *   right child of node = 2*node + 1   covers [mid+1, hi]
 * This is why you allocate ~4n slots — a non-complete tree can reach index ~4n.
 *
 * THE QUERY DECISION at each node (memorize these three cases):
 *   - NO overlap   ([l,r] outside [lo,hi]):     contribute the identity (0 for sum)
 *   - FULL overlap ([lo,hi] entirely in [l,r]): return this node's stored sum
 *   - PARTIAL overlap:                          recurse both children and combine
 */
#include "segment_tree.h"

#include <assert.h>
#include <stdlib.h>

/* The combine operation. SUM here; change ONLY this (and the identity 0 in query)
 * to get min/max/gcd/... Unused until the helpers call it — expected "unused"
 * warning until M1/M2. */
static long combine(long a, long b) {
    return a + b;
}

/* Recursively build node `node` (covering [lo,hi]) from values[]. Unused until
 * seg_new calls it — expected "unused" warning until M1. */
static void build(SegmentTree *t, const int *values, size_t node, size_t lo, size_t hi) {
    (void)t; (void)values; (void)node; (void)lo; (void)hi;
    /* TODO (M1): if lo == hi (leaf): t->tree[node] = values[lo]; return.
     * else mid = (lo+hi)/2; build left child (2*node, lo, mid); build right
     * (2*node+1, mid+1, hi); then t->tree[node] = combine(left, right). */
    assert(0 && "TODO: implement build (README M1)");
}

/* Sum over [l,r] within node `node` covering [lo,hi]. */
static long query(const SegmentTree *t, size_t node, size_t lo, size_t hi,
                  size_t l, size_t r) {
    (void)t; (void)node; (void)lo; (void)hi; (void)l; (void)r;
    /* TODO (M2): the three cases.
     *   if r < lo || hi < l: return 0;                 // no overlap → identity
     *   if l <= lo && hi <= r: return t->tree[node];    // full overlap
     *   mid = (lo+hi)/2;                                // partial → split
     *   return combine(query(...left, lo, mid...), query(...right, mid+1, hi...)); */
    assert(0 && "TODO: implement query (README M2)");
    return 0;
}

/* Set position `idx` to `val` within node `node` covering [lo,hi], recomputing. */
static void update(SegmentTree *t, size_t node, size_t lo, size_t hi,
                   size_t idx, int val) {
    (void)t; (void)node; (void)lo; (void)hi; (void)idx; (void)val;
    /* TODO (M3): if lo == hi (the leaf for idx): t->tree[node] = val; return.
     * else mid = (lo+hi)/2; recurse into the child whose range contains idx
     * (idx <= mid → left 2*node; else right 2*node+1); then recombine:
     * t->tree[node] = combine(t->tree[2*node], t->tree[2*node+1]). */
    assert(0 && "TODO: implement update (README M3)");
}

/* ------------------------------------------------------------------ M1 ---- */

SegmentTree *seg_new(const int *values, size_t n) {
    (void)values; (void)n;
    /* TODO (M1): decide n==0 policy. malloc the SegmentTree; malloc tree of ~4*n
     * longs (check NULL). t->n = n. build(t, values, 1, 0, n-1). Return t. */
    assert(0 && "TODO: implement seg_new (README M1)");
    return NULL;
}

void seg_free(SegmentTree *t) {
    (void)t;
    /* TODO (M1): if NULL nothing; else free(t->tree), then free(t). */
    assert(0 && "TODO: implement seg_free (README M1)");
}

/* ------------------------------------------------------------------ M2 ---- */

long seg_query(const SegmentTree *t, size_t l, size_t r) {
    (void)t; (void)l; (void)r;
    /* TODO (M2): bounds-check (l <= r < n). return query(t, 1, 0, n-1, l, r). */
    assert(0 && "TODO: implement seg_query (README M2)");
    return 0;
}

/* ------------------------------------------------------------------ M3 ---- */

void seg_update(SegmentTree *t, size_t index, int new_value) {
    (void)t; (void)index; (void)new_value;
    /* TODO (M3): bounds-check index < n. update(t, 1, 0, n-1, index, new_value). */
    assert(0 && "TODO: implement seg_update (README M3)");
}

/* ------------------------------------------------------------------ M4 ---- */

size_t seg_size(const SegmentTree *t) {
    (void)t;
    assert(0 && "TODO: implement seg_size (README M4)");
    return 0;
}

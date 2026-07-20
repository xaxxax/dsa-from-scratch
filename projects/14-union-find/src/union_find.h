/*
 * union_find.h — the CONTRACT for your Union-Find (Disjoint Set Union).
 *
 * The whole structure is a FOREST OF TREES stored inside a flat array. Each
 * element 0..n-1 has a parent; follow parents upward and you reach a ROOT, and
 * the root is the NAME of the set. Two elements are "connected" iff they share a
 * root. There are no pointers and no nodes — just an int array where parent[i]
 * is "who i points at."
 *
 *   elements:   0   1   2   3   4
 *   parent[]:   0   0   1   3   3     (parent[i] == i means "i is a root")
 *
 *   as a forest:        0            3
 *                       |           / \
 *                       1          (4 points at 3)
 *                       |
 *                       2
 *
 *   set {0,1,2} has root 0;  set {3,4} has root 3.
 *
 * TWO OPTIMIZATIONS make find/union effectively O(1) (inverse Ackermann α(n),
 * which is < 5 for any n you can store):
 *   - UNION BY RANK/SIZE: attach the shorter/smaller tree under the taller/larger
 *     one, so trees never get needlessly deep.
 *   - PATH COMPRESSION: during find(x), re-point every node on the path straight
 *     at the root, flattening the tree for next time.
 * Skip both and a bad sequence of unions builds a degenerate n-deep chain, making
 * find O(n) — a linked list wearing an array's clothes.
 *
 * WHY the API takes plain size_t indices, not values:
 *   Union-Find is defined over a FIXED universe 0..n-1 chosen at construction.
 *   If your real keys are strings or coordinates, you map them to 0..n-1 first
 *   (a job for the HashMap from Lesson 6) and run this underneath.
 */
#ifndef UNION_FIND_H
#define UNION_FIND_H

#include <stddef.h>

/* The handle. `parent` and `rank` are two parallel arrays of length `n`.
 * `count` caches how many disjoint sets currently exist (starts at n, drops by
 * one on every union that actually merges two different sets). */
typedef struct {
    size_t *parent;   /* parent[i] = i's parent; parent[i] == i iff i is a root */
    size_t *rank;      /* upper bound on the height of the tree rooted at i      */
    size_t  n;         /* universe size: valid elements are 0..n-1               */
    size_t  count;     /* number of disjoint sets right now                      */
} UnionFind;

/* ---- lifecycle ---------------------------------------------------------- */
UnionFind *uf_new(size_t n);   /* n singletons: parent[i]=i, rank[i]=0, count=n */
void       uf_free(UnionFind *uf);

/* ---- core operations ---------------------------------------------------- */
size_t uf_find(UnionFind *uf, size_t x);          /* root of x's set (compresses) */
int    uf_union(UnionFind *uf, size_t x, size_t y);/* merge; 1 if it joined 2 sets */
int    uf_connected(UnionFind *uf, size_t x, size_t y); /* same set? 1/0          */

/* ---- introspection ------------------------------------------------------ */
size_t uf_count(const UnionFind *uf);   /* how many disjoint sets remain */

#endif /* UNION_FIND_H */

/*
 * union_find.c — YOUR Union-Find (Disjoint Set Union).
 *
 * SCAFFOLD (README §4). Implement milestone by milestone under ASan. This is the
 * most code-light structure in the whole course — two int arrays and a handful
 * of lines per operation — but the REASONING (why it's near-O(1)) is deep.
 *
 * Invariants to protect on every mutation:
 *   - parent[i] is always a valid index in 0..n-1 (never dangles)
 *   - following parent[] from any i terminates at a root (parent[r] == r) — the
 *     forest has NO cycles except each root's self-loop
 *   - count == number of distinct roots == number of disjoint sets
 *   - rank[r] is a (non-tight) upper bound on the height of the tree rooted at r
 */
#include "union_find.h"

#include <assert.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ M1 ---- */

UnionFind *uf_new(size_t n) {
    (void)n;
    /* TODO (M1): malloc the UnionFind handle. malloc parent[n] and rank[n]
     * (check every allocation). Initialize EACH element as its own singleton
     * set: parent[i] = i, rank[i] = 0. Set uf->n = n and uf->count = n (n
     * separate sets to begin with). Decide: what should n == 0 do?
     * Order tip: if the second malloc fails, free the first before returning
     * NULL so a failed constructor leaks nothing. */
    assert(0 && "TODO: implement uf_new (README M1)");
    return NULL;
}

void uf_free(UnionFind *uf) {
    (void)uf;
    /* TODO (M1): if uf is NULL, do nothing (free-on-NULL is a no-op — callers
     * rely on it). Otherwise free the buffer(s) FIRST — free(uf->parent);
     * free(uf->rank); — THEN free(uf). Once you free the handle you can no
     * longer reach the arrays, so order matters. */
    assert(0 && "TODO: implement uf_free (README M1)");
}

/* ------------------------------------------------------------------ M2 ---- */

/* Return the root of x's set.
 *
 * M2 (FIRST version — no compression): walk parent[] upward until you reach a
 * node that is its own parent (parent[r] == r); return r. Bounds-check x < n
 * first (the size_t unsigned trap: a "negative" x arrives as a huge number).
 *
 * M4 (UPGRADE — path compression): after (or during) the walk, re-point the
 * nodes you passed straight at the root so the next find is O(1). Two classic
 * ways:
 *   - two-pass: find the root, then walk again setting parent[i] = root.
 *   - path halving: while walking, set parent[x] = parent[parent[x]] (point each
 *     node at its grandparent) — one pass, no recursion, nearly as flat.
 * Either is correct; path halving is the neat one-pass trick worth knowing.
 * NOTE this is why uf_find takes a non-const UnionFind* — a read that MUTATES.
 */
size_t uf_find(UnionFind *uf, size_t x) {
    (void)uf; (void)x;
    /* TODO (M2): bounds-check, then walk to the root and return it.
     * TODO (M4): add path compression here. */
    assert(0 && "TODO: implement uf_find (README M2, compression M4)");
    return 0;
}

/* ------------------------------------------------------------------ M3 ---- */

/* Merge the sets containing x and y. Return 1 if they were in DIFFERENT sets
 * (a real merge happened, count dropped by 1) and 0 if they were already
 * together (nothing to do). That return value is what makes cycle detection
 * trivial: an edge whose endpoints are already connected closes a cycle. */
int uf_union(UnionFind *uf, size_t x, size_t y) {
    (void)uf; (void)x; (void)y;
    /* TODO (M3): rx = uf_find(uf, x); ry = uf_find(uf, y). If rx == ry they are
     * already one set — return 0. Otherwise link one root under the other and
     * do uf->count-- and return 1.
     *
     * UNION BY RANK (do NOT just always hang rx under ry — that risks a deep
     * chain and O(n) finds):
     *   - if rank[rx] < rank[ry]:  parent[rx] = ry
     *   - else if rank[rx] > rank[ry]:  parent[ry] = rx
     *   - else (equal): pick either as the new root, say parent[ry] = rx, and
     *     then rank[rx]++ (the tree got one level taller only in a tie).
     * Attaching the shorter tree under the taller keeps the height logarithmic
     * even before path compression flattens it further. */
    assert(0 && "TODO: implement uf_union (README M3)");
    return 0;
}

/* ------------------------------------------------------------------ M5 ---- */

int uf_connected(UnionFind *uf, size_t x, size_t y) {
    (void)uf; (void)x; (void)y;
    /* TODO (M5): x and y are in the same set iff they share a root.
     * return uf_find(uf, x) == uf_find(uf, y). One line — but note it still
     * benefits from (and triggers) path compression on both arguments. */
    assert(0 && "TODO: implement uf_connected (README M5)");
    return 0;
}

size_t uf_count(const UnionFind *uf) {
    (void)uf;
    /* TODO (M5): return the cached uf->count. It is maintained by uf_union, so
     * this is O(1) — no scanning the array counting roots. (Counting roots by
     * scanning would be O(n); the cached counter is why uf_count is O(1).) */
    assert(0 && "TODO: implement uf_count (README M5)");
    return 0;
}

/*
 * lru_cache.h — the CONTRACT for your LRU (Least Recently Used) cache.
 *
 * THE CAPSTONE. A cache with a fixed capacity that, when full, evicts the entry
 * that hasn't been used for the longest time. Every operation is O(1) — and that
 * requires TWO structures working together, which is the whole lesson:
 *
 *   1. a HASH MAP    key -> node     → O(1) "is this key cached, and where?"
 *   2. a DOUBLY LINKED LIST of nodes in recency order (most-recent at the HEAD,
 *                                     least-recent at the TAIL)
 *                                    → O(1) move-to-front and O(1) evict-the-tail
 *
 * Neither alone suffices: a map has no notion of "oldest"; a list can't find a key
 * in O(1). Together they do. And note: `get` is a MUTATION — touching a key makes
 * it most-recent, so it moves to the front.
 *
 * INTRUSIVE DESIGN (the elegant C trick): each node lives in BOTH structures at
 * once. `prev`/`next` thread it through the recency list; `hnext` threads it
 * through its hash bucket's collision chain. One malloc'd node, two memberships —
 * so eviction removes it from BOTH in O(1). Keeping the two views in sync on every
 * operation is exactly where the bugs (and the learning) live.
 *
 * Keys and values are ints here (matching the classic LeetCode 146). A production
 * cache would be generic — see the README's "real design" discussion.
 */
#ifndef LRU_CACHE_H
#define LRU_CACHE_H

#include <stddef.h>

typedef struct LRUNode {
    int              key;
    int              value;
    struct LRUNode  *prev;   /* recency list: toward the HEAD (more recent)  */
    struct LRUNode  *next;   /* recency list: toward the TAIL (less recent)  */
    struct LRUNode  *hnext;  /* hash bucket chain: next node in this bucket  */
} LRUNode;

typedef struct {
    LRUNode **buckets;      /* hash table: key -> node (via hnext chains) */
    size_t    num_buckets;
    LRUNode  *head;         /* most-recently used (MRU)  */
    LRUNode  *tail;         /* least-recently used (LRU) — the eviction victim */
    size_t    size;         /* entries currently cached */
    size_t    capacity;     /* max entries before eviction */
} LRUCache;

/* ---- lifecycle ---------------------------------------------------------- */
LRUCache *lru_new(size_t capacity);
void      lru_free(LRUCache *c);

/* ---- operations (all O(1)) ---------------------------------------------- */
/* Look up key. If present, write the value to *out, MOVE it to most-recent, and
 * return 1. If absent, return 0 (and leave *out untouched). Yes — a read reorders. */
int  lru_get(LRUCache *c, int key, int *out);

/* Insert or update key->value. Makes it most-recent. If inserting would exceed
 * capacity, first EVICT the least-recently-used entry (the tail). */
void lru_put(LRUCache *c, int key, int value);

size_t lru_size(const LRUCache *c);

#endif /* LRU_CACHE_H */

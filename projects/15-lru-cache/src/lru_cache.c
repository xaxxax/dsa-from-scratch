/*
 * lru_cache.c — YOUR LRU cache. SCAFFOLD (README §4).
 *
 * The invariant to hold after EVERY operation (the heart of the lesson):
 *   - the hash map and the recency list contain EXACTLY the same set of nodes
 *   - head = most-recently used, tail = least-recently used
 *   - size == node count == entries in the map, and size <= capacity
 *
 * Every mutation touches BOTH structures. The discipline that saves you: when you
 * unlink a node from the list, ask "did I also remove it from its hash bucket?" —
 * and vice versa. A node freed from one but not the other is a dangling bug.
 *
 * Build the small static helpers first (M2/M3); lru_get/lru_put (M4/M5) then read
 * like plain English on top of them.
 */
#include "lru_cache.h"

#include <assert.h>
#include <stdlib.h>

/* ---- hash helper --------------------------------------------------------- */
/* Map an int key to a bucket index. A decent mix for ints, then % num_buckets.
 * Unused until the map helpers call it — expected "unused" warning until M3. */
static size_t bucket_of(const LRUCache *c, int key) {
    (void)c; (void)key;
    /* TODO (M3): turn key into an unsigned, mix it a little, return % num_buckets.
     * (Even (unsigned)key % num_buckets works to start; mixing reduces clustering.) */
    assert(0 && "TODO: implement bucket_of (README M3)");
    return 0;
}

/* ---- recency-list primitives (M2) --------------------------------------- */

/* Unlink node from the doubly linked recency list (fix neighbors, head, tail).
 * Does NOT touch the hash map or free the node. */
static void list_unlink(LRUCache *c, LRUNode *n) {
    (void)c; (void)n;
    /* TODO (M2): standard doubly-linked-list removal (Lesson 3):
     *   if n->prev: n->prev->next = n->next;  else c->head = n->next;
     *   if n->next: n->next->prev = n->prev;  else c->tail = n->prev; */
    assert(0 && "TODO: implement list_unlink (README M2)");
}

/* Insert node at the HEAD (most-recent position). */
static void list_push_front(LRUCache *c, LRUNode *n) {
    (void)c; (void)n;
    /* TODO (M2): n->prev = NULL; n->next = c->head;
     *   if c->head: c->head->prev = n;  else c->tail = n;  (was empty)
     *   c->head = n; */
    assert(0 && "TODO: implement list_push_front (README M2)");
}

/* Move an already-listed node to the head (used on every access). */
static void move_to_front(LRUCache *c, LRUNode *n) {
    (void)c; (void)n;
    /* TODO (M2): if n is already head, nothing to do; else list_unlink(c,n) then
     * list_push_front(c,n). Composing the two primitives keeps this trivial. */
    assert(0 && "TODO: implement move_to_front (README M2)");
}

/* ---- hash-map primitives (M3) ------------------------------------------- */

/* Find the node for key (or NULL) by walking its bucket's hnext chain. */
static LRUNode *map_find(const LRUCache *c, int key) {
    (void)c; (void)key;
    /* TODO (M3): idx = bucket_of(c,key); walk c->buckets[idx] via hnext; return
     * the node whose key matches, or NULL. */
    assert(0 && "TODO: implement map_find (README M3)");
    return NULL;
}

/* Insert a node into its hash bucket (push on the bucket-chain head). */
static void map_insert(LRUCache *c, LRUNode *n) {
    (void)c; (void)n;
    /* TODO (M3): idx = bucket_of(c, n->key); n->hnext = c->buckets[idx];
     * c->buckets[idx] = n; */
    assert(0 && "TODO: implement map_insert (README M3)");
}

/* Remove a node from its hash bucket chain (does NOT free it). */
static void map_remove(LRUCache *c, LRUNode *n) {
    (void)c; (void)n;
    /* TODO (M3): walk c->buckets[bucket_of(c,n->key)]; unlink n from the hnext
     * chain (fix the predecessor's hnext, or the bucket head if n is first). */
    assert(0 && "TODO: implement map_remove (README M3)");
}

/* ------------------------------------------------------------------ M1 ---- */

LRUCache *lru_new(size_t capacity) {
    (void)capacity;
    /* TODO (M1): malloc the cache. Choose num_buckets (e.g. capacity, or the next
     * power of two >= capacity, min 1). calloc the buckets (all NULL). head =
     * tail = NULL; size = 0; capacity = capacity. Check every allocation. */
    assert(0 && "TODO: implement lru_new (README M1)");
    return NULL;
}

void lru_free(LRUCache *c) {
    (void)c;
    /* TODO (M1): if NULL nothing. Walk the recency list (head->next) freeing every
     * node — that reaches all of them, since map and list hold the same nodes.
     * Then free(c->buckets); then free(c). Don't double-free by also walking the
     * buckets. */
    assert(0 && "TODO: implement lru_free (README M1)");
}

/* ------------------------------------------------------------------ M4 ---- */

int lru_get(LRUCache *c, int key, int *out) {
    (void)c; (void)key; (void)out;
    /* TODO (M4): n = map_find(c, key). If NULL return 0. Else *out = n->value;
     * move_to_front(c, n); return 1. (The move is why get takes a non-const cache
     * — a read that reorders.) */
    assert(0 && "TODO: implement lru_get (README M4)");
    return 0;
}

/* ------------------------------------------------------------------ M5 ---- */

void lru_put(LRUCache *c, int key, int value) {
    (void)c; (void)key; (void)value;
    /* TODO (M5):
     *   1. n = map_find(c, key). If found: n->value = value; move_to_front; return.
     *   2. Otherwise create a new node (key,value). map_insert it AND
     *      list_push_front it (both structures!). size++.
     *   3. If size > capacity: EVICT the LRU = c->tail. list_unlink it,
     *      map_remove it, free it, size--. (Grab the tail pointer BEFORE unlinking.)
     * The eviction touching BOTH structures is the crux — miss one and you leak or
     * dangle. */
    assert(0 && "TODO: implement lru_put (README M5)");
}

/* ------------------------------------------------------------------ M6 ---- */

size_t lru_size(const LRUCache *c) {
    (void)c;
    assert(0 && "TODO: implement lru_size (README M6)");
    return 0;
}

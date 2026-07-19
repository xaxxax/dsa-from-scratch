/*
 * hashmap.c — YOUR hash map. The crown jewel — budget real time.
 *
 * SCAFFOLD (README §4). Implement milestone by milestone under ASan. The memory
 * here is the deepest yet: a bucket array, a chain of entries per bucket, and a
 * strdup'd key per entry. hm_free must release ALL of it, in the right order.
 *
 * Invariants:
 *   - size == total entries across all buckets
 *   - every key appears at most once (put updates rather than duplicates)
 *   - each entry's key is a heap copy the map owns
 */
#include "hashmap.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>   /* strcmp, strlen, memcpy (for your strdup) */

#define HM_DEFAULT_CAPACITY 16
#define HM_MAX_LOAD_FACTOR  0.75

/* ---- helpers you will write ---------------------------------------------- */

/* Hash a NUL-terminated string to a 64-bit value. Recommended: FNV-1a —
 *     hash = 1469598103934665603ULL (offset basis)
 *     for each byte c:  hash ^= c;  hash *= 1099511628211ULL (FNV prime)
 * Then the bucket index is (hash % capacity). WHY not just sum the bytes? Because
 * "abc" and "cba" would collide, and low-quality hashes cluster — good mixing
 * spreads keys evenly, which is what keeps chains short and lookups O(1). */
static unsigned long long hm_hash(const char *key) {
    (void)key;
    /* TODO (M2): implement FNV-1a. */
    assert(0 && "TODO: implement hm_hash (README M2)");
    return 0;
}

/* Duplicate a string onto the heap (C has no standard strdup in c11). Caller owns
 * the result and must free it. malloc strlen+1, memcpy including the NUL. */
static char *hm_strdup(const char *s) {
    (void)s;
    /* TODO (M1): malloc(strlen(s)+1); copy; return it. Check malloc. */
    assert(0 && "TODO: implement hm_strdup (README M1)");
    return NULL;
}

/* ------------------------------------------------------------------ M1 ---- */

HashMap *hm_with_capacity(size_t capacity) {
    (void)capacity;
    /* TODO (M1): malloc the HashMap; malloc buckets = capacity pointers, all set
     * to NULL (calloc is perfect here — why is calloc better than malloc+loop?);
     * size = 0. Handle capacity == 0. Check every allocation. */
    assert(0 && "TODO: implement hm_with_capacity (README M1)");
    return NULL;
}

HashMap *hm_new(void) {
    /* TODO (M1): hm_with_capacity(HM_DEFAULT_CAPACITY). */
    assert(0 && "TODO: implement hm_new (README M1)");
    return NULL;
}

void hm_free(HashMap *m) {
    (void)m;
    /* TODO (M1): if NULL nothing. Else for each bucket, walk its chain freeing
     * each entry's KEY (free(e->key)) and then the entry; then free the buckets
     * array; then free the struct. Miss any layer and ASan reports the leak. */
    assert(0 && "TODO: implement hm_free (README M1)");
}

/* ------------------------------------------------------------------ M3 ---- */

void hm_put(HashMap *m, const char *key, hm_value value) {
    (void)m; (void)key; (void)value;
    /* TODO (M3): index = hm_hash(key) % capacity. Walk that bucket's chain: if a
     * matching key exists (strcmp == 0), UPDATE its value and return (no new
     * entry, size unchanged). Otherwise malloc a new Entry, key = hm_strdup(key),
     * value = value, push it onto the bucket head, size++.
     * TODO (M6): after inserting, if load factor > HM_MAX_LOAD_FACTOR, resize. */
    assert(0 && "TODO: implement hm_put (README M3/M6)");
}

/* ------------------------------------------------------------------ M4 ---- */

int hm_get(const HashMap *m, const char *key, hm_value *out) {
    (void)m; (void)key; (void)out;
    /* TODO (M4): index = hash % capacity; walk the chain; on strcmp match write
     * *out = entry->value and return 1. Return 0 if not found (don't touch *out).
     * This is why get returns status, not the value: any int is a valid value. */
    assert(0 && "TODO: implement hm_get (README M4)");
    return 0;
}

int hm_contains(const HashMap *m, const char *key) {
    (void)m; (void)key;
    /* TODO (M4): reuse hm_get with a throwaway out, or scan directly. */
    assert(0 && "TODO: implement hm_contains (README M4)");
    return 0;
}

/* ------------------------------------------------------------------ M5 ---- */

int hm_remove(HashMap *m, const char *key) {
    (void)m; (void)key;
    /* TODO (M5): find the entry AND its predecessor in the chain (or track head).
     * Unlink it (prev->next = e->next, or bucket head = e->next). free(e->key)
     * THEN free(e). size--. Return 1 if removed, 0 if the key wasn't present. */
    assert(0 && "TODO: implement hm_remove (README M5)");
    return 0;
}

/* ------------------------------------------------------------ M6 (resize) -- */

/* Grow the table and REHASH every entry into the new buckets. You cannot just
 * copy buckets across: bucket index is (hash % capacity), so a bigger capacity
 * sends entries to DIFFERENT buckets. Rehashing is mandatory, and it's why an
 * occasional put is O(n). */
static void hm_resize(HashMap *m, size_t new_capacity) {
    (void)m; (void)new_capacity;
    /* TODO (M6): allocate a new NULL-initialized bucket array of new_capacity.
     * For every entry in the old buckets, recompute index % new_capacity and
     * move (re-link) it into the new array — REUSE the existing Entry nodes and
     * their keys (no need to strdup again; just relink). Free the old bucket
     * array (NOT the entries). Update m->buckets and m->capacity. */
    assert(0 && "TODO: implement hm_resize (README M6)");
}

/* ------------------------------------------------------------------ M7 ---- */

size_t hm_size(const HashMap *m) {
    (void)m;
    assert(0 && "TODO: implement hm_size (README M7)");
    return 0;
}

double hm_load_factor(const HashMap *m) {
    (void)m;
    /* TODO (M7): (double)size / (double)capacity. */
    assert(0 && "TODO: implement hm_load_factor (README M7)");
    return 0.0;
}

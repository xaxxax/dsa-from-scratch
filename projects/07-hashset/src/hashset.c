/*
 * hashset.c — YOUR hash set, composed from the Lesson 6 hash map.
 *
 * SCAFFOLD (README §4). The bodies are one-liners delegating to hm_*. The lesson
 * is conceptual: a set is a map minus values, and `add` must be IDEMPOTENT
 * (adding an existing member changes nothing, keeps size the same) — which you
 * get for free because hm_put updates rather than duplicates.
 */
#include "hashset.h"

#include <assert.h>
#include <stdlib.h>

/* A single sentinel value stored for every member. Its actual number is
 * irrelevant — the set only cares whether the KEY is present. */
#define HS_PRESENT 1

/* ------------------------------------------------------------------ M1 ---- */

HashSet *hs_new(void) {
    /* TODO (M1): malloc HashSet; s->backing = hm_new(); check both. */
    assert(0 && "TODO: implement hs_new (README M1)");
    return NULL;
}

void hs_free(HashSet *s) {
    (void)s;
    /* TODO (M1): if NULL nothing; else hm_free(s->backing) THEN free(s).
     * hm_free already frees every key copy — you rely on the map's ownership. */
    assert(0 && "TODO: implement hs_free (README M1)");
}

/* ------------------------------------------------------------------ M2 ---- */

void hs_add(HashSet *s, const char *key) {
    (void)s; (void)key;
    /* TODO (M2): hm_put(s->backing, key, HS_PRESENT). Adding an existing key
     * just re-updates the same entry — idempotent, size unchanged. */
    assert(0 && "TODO: implement hs_add (README M2)");
}

int hs_contains(const HashSet *s, const char *key) {
    (void)s; (void)key;
    /* TODO (M2): delegate to hm_contains. */
    assert(0 && "TODO: implement hs_contains (README M2)");
    return 0;
}

/* ------------------------------------------------------------------ M3 ---- */

int hs_remove(HashSet *s, const char *key) {
    (void)s; (void)key;
    /* TODO (M3): delegate to hm_remove (returns 1 if the key was present). */
    assert(0 && "TODO: implement hs_remove (README M3)");
    return 0;
}

size_t hs_size(const HashSet *s) {
    (void)s;
    /* TODO (M3): delegate to hm_size. */
    assert(0 && "TODO: implement hs_size (README M3)");
    return 0;
}

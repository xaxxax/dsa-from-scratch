/*
 * hashset.h — the CONTRACT for your hash set.
 *
 * A SET answers one question: "is this key present?" — no values, no duplicates.
 * That is EXACTLY a hash map whose values you ignore. So instead of rebuilding
 * hashing/chaining/resizing, you COMPOSE your Lesson 6 map: HashSet HAS-A HashMap,
 * and `add` stores the key with a throwaway value.
 *
 * This is the payoff of good abstraction: a whole new structure in a few lines,
 * because the hard part was already solved and given a clean contract.
 */
#ifndef HASHSET_H
#define HASHSET_H

#include <stddef.h>
#include "hashmap.h"   /* found via -I../06-hashmap/src */

typedef struct {
    HashMap *backing;   /* HAS-A: keys are the set members; values unused */
} HashSet;

HashSet *hs_new(void);
void     hs_free(HashSet *s);

void     hs_add     (HashSet *s, const char *key);   /* idempotent: adding twice = once */
int      hs_contains(const HashSet *s, const char *key);
int      hs_remove  (HashSet *s, const char *key);   /* 1 if it was present */

size_t   hs_size(const HashSet *s);

#endif /* HASHSET_H */

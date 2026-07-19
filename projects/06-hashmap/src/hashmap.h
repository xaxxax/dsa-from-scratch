/*
 * hashmap.h — the CONTRACT for your hash map (string keys → int values).
 *
 * This is the crown jewel. A hash map gives average-case O(1) lookup BY KEY —
 * not by integer index. The trick: a hash function turns a key into an integer,
 * and (integer % capacity) picks a bucket in an array. Collisions (two keys, same
 * bucket) are handled by SEPARATE CHAINING — each bucket is a small linked list.
 *
 *   buckets (capacity 8):
 *    [0] ─▶ (key="cat", val=1) ─▶ (key="dog", val=2)    ← collided in bucket 0
 *    [1] ─▶ (key="fox", val=9)
 *    [2]    NULL
 *    ...
 *
 * THE BIG C LESSON — OWNERSHIP OF KEYS:
 *   The caller passes a `const char *key`. If the map just stored that pointer, it
 *   would dangle the moment the caller's buffer changed or freed. So the map must
 *   store its OWN copy (strdup) and free that copy on remove/free. The map owns
 *   its keys. Values here are plain ints (nothing to own); if values were pointers
 *   to heap objects, you'd face the same ownership question for them.
 *
 * WHY get() RETURNS A STATUS, NOT THE VALUE:
 *   Any int is a legal stored value, so there's no "not found" int to return. We
 *   return 1/0 for found/not-found and deliver the value through an out-parameter.
 *   This is the idiomatic C answer to "Java returned null" (which conflated "absent"
 *   with "present-but-null").
 */
#ifndef HASHMAP_H
#define HASHMAP_H

#include <stddef.h>

typedef int hm_value;

/* One key/value entry; `next` chains colliding entries within a bucket. The map
 * owns `key` (a strdup'd copy it must free). */
typedef struct Entry {
    char         *key;
    hm_value      value;
    struct Entry *next;
} Entry;

typedef struct {
    Entry  **buckets;    /* array of `capacity` bucket heads (each a chain or NULL) */
    size_t   capacity;   /* number of buckets */
    size_t   size;       /* number of entries stored */
} HashMap;

/* ---- lifecycle ---------------------------------------------------------- */
HashMap *hm_new(void);
HashMap *hm_with_capacity(size_t capacity);
void     hm_free(HashMap *m);   /* frees every entry, every key, the bucket array */

/* ---- core operations ---------------------------------------------------- */
void hm_put(HashMap *m, const char *key, hm_value value); /* insert OR update    */
int  hm_get(const HashMap *m, const char *key, hm_value *out); /* 1 if found      */
int  hm_contains(const HashMap *m, const char *key);
int  hm_remove(HashMap *m, const char *key);              /* 1 if a key was removed */

/* ---- introspection ------------------------------------------------------ */
size_t hm_size(const HashMap *m);
double hm_load_factor(const HashMap *m);   /* size / capacity — drives resize */

#endif /* HASHMAP_H */

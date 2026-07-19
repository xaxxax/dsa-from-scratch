/*
 * dynamic_array.c — YOUR growable, array-backed list.
 *
 * This is a SCAFFOLD. The signatures are fixed by the header; every body marked
 * TODO is yours to write. Work milestone by milestone (README §4), compiling and
 * testing after each one. Run `make test` — it builds with AddressSanitizer on,
 * so a bad read/write or a leak becomes a loud crash, not silent corruption.
 *
 * The whole structure rests on ONE invariant you must never break:
 *
 *      0 <= size <= capacity
 *
 * Reminder (the C trap): size_t is UNSIGNED. The bounds check is `index >= size`
 * only — there is no "index < 0" to catch, because a "negative" index arrives as a
 * huge positive number. Let `index >= size` catch it.
 */
#include "dynamic_array.h"

#include <assert.h>
#include <stdlib.h>   /* malloc, realloc, free */

/* Tune these and JUSTIFY them in the README's design questions. */
#define DA_DEFAULT_CAPACITY 8
#define DA_GROWTH_FACTOR    2

/* ------------------------------------------------------------------ M1 ---- */

DynamicArray *da_new(void) {
    /* TODO (M1): implement in terms of da_with_capacity(DA_DEFAULT_CAPACITY). */
    assert(0 && "TODO: implement da_new (README M1)");
    return NULL;
}

DynamicArray *da_with_capacity(size_t initial_capacity) {
    (void)initial_capacity;
    /* TODO (M1):
     *   - decide + enforce your policy for initial_capacity == 0
     *   - malloc the DynamicArray struct  (check for NULL)
     *   - malloc the backing buffer of `initial_capacity` da_elem  (check for NULL;
     *     if it fails, don't leak the struct)
     *   - set size = 0, capacity = initial_capacity
     *   - return the struct
     */
    assert(0 && "TODO: implement da_with_capacity (README M1)");
    return NULL;
}

void da_free(DynamicArray *a) {
    (void)a;
    /* TODO (M1): if a is NULL, do nothing. Otherwise free(a->data), then free(a).
     * Why that order? Once you free(a), you can no longer read a->data. */
    assert(0 && "TODO: implement da_free (README M1)");
}

/* ------------------------------------------------------------------ M2 ---- */

size_t da_size(const DynamicArray *a) {
    (void)a;
    assert(0 && "TODO: implement da_size (README M2)");
    return 0;
}

size_t da_capacity(const DynamicArray *a) {
    (void)a;
    assert(0 && "TODO: implement da_capacity (README M2)");
    return 0;
}

int da_is_empty(const DynamicArray *a) {
    (void)a;
    assert(0 && "TODO: implement da_is_empty (README M2)");
    return 0;
}

/* ------------------------------------------------------------ M5 (helper) -- */

/* Grow the backing buffer (usually to capacity * DA_GROWTH_FACTOR). Private:
 * declared `static` so it's not part of the public API. Handle realloc failure
 * and the fact that realloc MAY return a different address.
 *
 * Until M5, nothing calls this, so the compiler warns "defined but not used" —
 * that's expected dead-code feedback; it disappears once da_add calls it. */
static void da_grow(DynamicArray *a) {
    (void)a;
    /* TODO (M5):
     *   size_t new_cap = a->capacity * DA_GROWTH_FACTOR;  (careful if capacity is 0)
     *   da_elem *bigger = realloc(a->data, new_cap * sizeof *a->data);
     *   if (bigger == NULL) { handle failure — a->data is still valid }
     *   a->data = bigger; a->capacity = new_cap;
     */
    assert(0 && "TODO: implement da_grow (README M5)");
}

/* ------------------------------------------------------------ M3, M5 ------ */

void da_add(DynamicArray *a, da_elem item) {
    (void)a; (void)item;
    /* TODO (M3 easy case): write data[size] = item; size++
     * TODO (M5): first, if size == capacity, da_grow(a); THEN write. */
    assert(0 && "TODO: implement da_add (README M3/M5)");
}

/* ------------------------------------------------------------------ M4 ---- */

da_elem da_get(const DynamicArray *a, size_t index) {
    (void)a; (void)index;
    /* TODO (M4): bounds-check with `index >= a->size` (see the unsigned trap),
     * then return a->data[index]. */
    assert(0 && "TODO: implement da_get (README M4)");
    return 0;
}

/* ------------------------------------------------------------------ M6 ---- */

da_elem da_set(DynamicArray *a, size_t index, da_elem item) {
    (void)a; (void)index; (void)item;
    /* TODO (M6): bounds-check, save old = data[index], write item, return old. */
    assert(0 && "TODO: implement da_set (README M6)");
    return 0;
}

/* ------------------------------------------------------------------ M7 ---- */

da_elem da_remove(DynamicArray *a, size_t index) {
    (void)a; (void)index;
    /* TODO (M7): bounds-check; save removed = data[index]; shift the tail left
     * (data[i] = data[i+1] for i in index..size-2); size--; return removed. */
    assert(0 && "TODO: implement da_remove (README M7)");
    return 0;
}

/* ------------------------------------------------------------------ M8 ---- */

int da_contains(const DynamicArray *a, da_elem item) {
    (void)a; (void)item;
    /* TODO (M8): linear scan 0..size-1; return 1 if an element equals item, else 0. */
    assert(0 && "TODO: implement da_contains (README M8)");
    return 0;
}

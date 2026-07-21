/*
 * dynamic_array.h — the CONTRACT for your growable, array-backed list.
 *
 * A header declares WHAT exists (the struct's shape and the function prototypes);
 * dynamic_array.c defines HOW it works. Test/other files #include this to use it.
 *
 * You generally do NOT need to change this file — implement the bodies in the .c.
 * (You MAY extend it: da_insert, da_clear, da_index_of, etc. are yours to add.)
 */
#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stddef.h>   /* size_t */

/*
 * The element type. Start CONCRETE so the mechanics are clear. Change this one
 * line to store a different type; making it hold ANY type (void* + elem_size) is
 * the generic stretch goal in the README.
 */
typedef int da_elem;

/*
 * The structure. A raw malloc'd buffer does NOT remember its own length, so —
 * unlike Java's data.length — you must track `capacity` yourself.
 *
 *      0 <= size <= capacity        (size = items in use; capacity = slots allocated)
 *
 * If a test fails, the first question is almost always: "did I violate that?"
 */
typedef struct {
    da_elem *data;      /* heap buffer holding the elements (the storage) */
    size_t   size;      /* number of elements currently in use */
    size_t   capacity;  /* number of elements the buffer can hold */
} DynamicArray;

/* ---- Lifecycle: in C, YOU own the memory. Every da_new needs a da_free. ---- */

/* Allocate an empty list with a sensible default capacity. Returns NULL on
 * allocation failure. */
DynamicArray *da_new(void);

/* Allocate an empty list with the given initial capacity. Decide your policy for
 * capacity 0 (the README asks you to). */
DynamicArray *da_with_capacity(size_t initial_capacity);

/* Release EVERYTHING: the backing buffer AND the struct. Safe to call on NULL.
 * Order matters — free the buffer before the struct. */
void da_free(DynamicArray *a);

/* ---- Core operations (implement in the README's milestone order) ---- */

void    da_add(DynamicArray *a, da_elem item);              /* append to the end */
da_elem da_get(const DynamicArray *a, size_t index);        /* read at index */
da_elem da_set(DynamicArray *a, size_t index, da_elem item);/* replace, return old */
da_elem da_remove(DynamicArray *a, size_t index);           /* remove+return, shift */
int     da_contains(const DynamicArray *a, da_elem item);   /* 1 if present, else 0 */

size_t  da_size(const DynamicArray *a);
size_t  da_capacity(const DynamicArray *a);
int     da_is_empty(const DynamicArray *a);                 /* 1 if size == 0 */
void    da_insert(DynamicArray *a, da_elem item, size_t index); /* insert element at index */
#endif /* DYNAMIC_ARRAY_H */

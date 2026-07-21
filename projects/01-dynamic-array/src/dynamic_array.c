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

DynamicArray *da_new(void) {
   return da_with_capacity(DA_DEFAULT_CAPACITY); 
}

DynamicArray *da_with_capacity(size_t initial_capacity) {
    if(initial_capacity == 0) initial_capacity = DA_DEFAULT_CAPACITY;

    DynamicArray *da = malloc(sizeof(DynamicArray)); // create DA with size of (data + size + capacity)
    
    if(!da) return NULL;

    da->data = malloc(initial_capacity * sizeof(da_elem));

    if(!da->data) {
      free(da);
      return NULL;
    } 

    da->size = 0;
    da->capacity = initial_capacity;

    return da;
}

void da_free(DynamicArray *a) {
    if(!a) return;  

    free(a->data);
    free(a);
}

size_t da_size(const DynamicArray *a) {
    assert(a != NULL);
    return a->size;
}

size_t da_capacity(const DynamicArray *a) {
    assert(a != NULL);
    return a->capacity;
}

int da_is_empty(const DynamicArray *a) {
    assert(a != NULL);

    return a->size == 0;
}

static void da_grow(DynamicArray *a) {
    assert(a != NULL);  

    // capacity should never be able to be 0 because we set it to DA_DEFAULT_CAPACITY when a user passes in capacity as 0
    size_t new_cap = a->capacity * DA_GROWTH_FACTOR; // take current capacity and grow it by previously decided DA_GROWTH_FACTOR
    
    da_elem *new_data = realloc(a->data, new_cap * sizeof(da_elem));
    if(!new_data) return;

    a->data = new_data;
    a->capacity = new_cap;
}

void da_add(DynamicArray *a, da_elem item) {
    assert(a != NULL);
    
    if(a->size == a->capacity) da_grow(a);
    
    a->data[a->size] = item;
    a->size++;
}

da_elem da_get(const DynamicArray *a, size_t index) {
    assert(a != NULL && index < a->size);  
    return a->data[index];
}

da_elem da_set(DynamicArray *a, size_t index, da_elem item) {
    assert(a != NULL && index < a->size);
    
    da_elem old = a->data[index];
    a->data[index] = item;

    return old;
}

da_elem da_remove(DynamicArray *a, size_t index) {
    assert(a != NULL && index < a->size);  
    
    da_elem remove_elem = a->data[index];
      
    // use loop and move everything left one
    for (size_t i = index; i < a->size; i++) {
      a->data[i] = a->data[i + 1];
    }
    
    //a->data[a->size - 1] = NULL; // possible to not have this, as when size shrinks the array will become one less
                                   // since we do not allow index >= size, it will never be accessible 
    a->size--;  

    return remove_elem;
}

int da_contains(const DynamicArray *a, da_elem item) {
    assert(a != NULL);
    for (size_t i = 0; i < a->size; i++) { // cannot be int type, must be same type for comparison with a->size
      if(a->data[i] == item) return 1;
    }
    return 0;
}

void da_insert(DynamicArray *a, da_elem item, size_t index) {
  assert(a != NULL);
  
  if(a->size == a->capacity) da_grow(a);

  for(size_t i = a->size; i > index; i--) {
    a->data[i] = a->data[i - 1];
  }
  
  a->size++;
  a->data[index] = item;
}

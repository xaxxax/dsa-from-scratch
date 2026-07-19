/*
 * linked_list.h — the CONTRACT for your singly linked list.
 *
 * This header is the one file you should read top to bottom before writing any
 * code. It fixes the types and the function signatures; linked_list.c is where
 * YOU fill in the bodies (see the README milestones).
 *
 * The whole structure is a chain of separately-allocated nodes:
 *
 *      head                                        tail
 *       │                                           │
 *       ▼                                           ▼
 *     [10|·]──▶[20|·]──▶[30|·]──▶[40| / ]     (/ = NULL, the end)
 *
 * Contrast with the dynamic array: there, ONE malloc held every element. Here,
 * EACH node is its own malloc — so ll_free must walk the chain and free every
 * node, and losing `head` mid-operation leaks the entire tail.
 */
#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stddef.h>   /* size_t */

/* The element type. One name, change it in one place (C has no generics). */
typedef int ll_elem;

/* A node owns its value and a pointer to the next node. `struct Node` names
 * itself so the `next` field can point at the same type it lives in. */
typedef struct Node {
    ll_elem       value;
    struct Node  *next;
} Node;

/* The list handle. We keep a `tail` so add_last is O(1) instead of O(n) — but
 * that convenience is a promise you must maintain in EVERY mutator (see README
 * §3: what breaks tail?). */
typedef struct {
    Node   *head;
    Node   *tail;
    size_t  size;
} LinkedList;

/* ---- lifecycle ---------------------------------------------------------- */
LinkedList *ll_new(void);            /* empty list; NULL on allocation failure */
void        ll_free(LinkedList *l);  /* free EVERY node, then the handle       */

/* ---- adding ------------------------------------------------------------- */
void ll_add_first(LinkedList *l, ll_elem item);   /* O(1) — new head          */
void ll_add_last (LinkedList *l, ll_elem item);   /* O(1) thanks to tail      */

/* ---- reading ------------------------------------------------------------ */
ll_elem ll_get     (const LinkedList *l, size_t index);  /* O(n) walk         */
int     ll_contains(const LinkedList *l, ll_elem item);  /* 1 if present      */
size_t  ll_size    (const LinkedList *l);
int     ll_is_empty(const LinkedList *l);

/* ---- removing ----------------------------------------------------------- */
ll_elem ll_remove_first(LinkedList *l);            /* return old head value    */
int     ll_remove      (LinkedList *l, ll_elem item); /* first match; 1 if hit */

/* ---- rearranging -------------------------------------------------------- */
void ll_reverse(LinkedList *l);   /* reverse in place — the pointer classic   */

#endif /* LINKED_LIST_H */

/*
 * linked_list.c — YOUR singly linked list.
 *
 * SCAFFOLD: signatures are fixed by the header; every TODO body is yours. Work
 * milestone by milestone (README §4), compiling and running `make test` after
 * each. ASan is on, so a use-after-free or a leak becomes a loud crash.
 *
 * The invariant to protect on EVERY mutation:
 *
 *      - size == number of reachable nodes from head
 *      - tail points at the last node (or NULL iff the list is empty)
 *      - the last node's `next` is NULL
 *
 * The #1 skill here: reassign pointers in an order that never drops a node you
 * still need. When in doubt, draw the three-node case and number every arrow.
 */
#include "linked_list.h"

#include <assert.h>
#include <stdlib.h>   /* malloc, free */

/* Optional private helper: allocate one node with value + next. Writing this
 * once keeps every add path honest. (Left for you — declare it static.) */

/* ------------------------------------------------------------------ M1 ---- */

LinkedList *ll_new(void) {
    /* TODO (M1): malloc the LinkedList; set head = tail = NULL, size = 0.
     * Check malloc for NULL. Return the handle. */
    assert(0 && "TODO: implement ll_new (README M1)");
    return NULL;
}

void ll_free(LinkedList *l) {
    (void)l;
    /* TODO (M1): if l is NULL, do nothing. Otherwise walk the chain:
     *     Node *cur = l->head;
     *     while (cur) { Node *next = cur->next; free(cur); cur = next; }
     * ...then free(l). Note WHY you must grab `next` BEFORE freeing `cur`:
     * after free(cur), reading cur->next is use-after-free. */
    assert(0 && "TODO: implement ll_free (README M1)");
}

/* ------------------------------------------------------------------ M2 ---- */

size_t ll_size(const LinkedList *l) {
    (void)l;
    assert(0 && "TODO: implement ll_size (README M2)");
    return 0;
}

int ll_is_empty(const LinkedList *l) {
    (void)l;
    assert(0 && "TODO: implement ll_is_empty (README M2)");
    return 0;
}

/* ------------------------------------------------------------------ M3 ---- */

void ll_add_first(LinkedList *l, ll_elem item) {
    (void)l; (void)item;
    /* TODO (M3): malloc a node; node->value = item; node->next = l->head;
     * l->head = node; size++. EDGE CASE: if the list was empty, tail must now
     * also point at this new (and only) node. */
    assert(0 && "TODO: implement ll_add_first (README M3)");
}

/* ------------------------------------------------------------------ M4 ---- */

void ll_add_last(LinkedList *l, ll_elem item) {
    (void)l; (void)item;
    /* TODO (M4): malloc a node with next = NULL. If empty, head = tail = node.
     * Otherwise l->tail->next = node; then l->tail = node. size++.
     * THIS is where the tail pointer earns its keep — O(1), no walk. */
    assert(0 && "TODO: implement ll_add_last (README M4)");
}

/* ------------------------------------------------------------------ M5 ---- */

ll_elem ll_get(const LinkedList *l, size_t index) {
    (void)l; (void)index;
    /* TODO (M5): bounds-check index >= l->size (unsigned trap, same as Lesson 1).
     * Walk `index` hops from head, then return cur->value. This O(n) walk is the
     * price of the linked list — contrast Lesson 1's O(1) data[index]. */
    assert(0 && "TODO: implement ll_get (README M5)");
    return 0;
}

/* ------------------------------------------------------------------ M6 ---- */

ll_elem ll_remove_first(LinkedList *l) {
    (void)l;
    /* TODO (M6): decide your empty-list policy (assert vs. status). Otherwise:
     * save old = l->head; grab value; l->head = old->next; free(old); size--.
     * EDGE CASE: if the list is now empty, tail must become NULL too. */
    assert(0 && "TODO: implement ll_remove_first (README M6)");
    return 0;
}

/* ------------------------------------------------------------------ M7 ---- */

int ll_remove(LinkedList *l, ll_elem item) {
    (void)l; (void)item;
    /* TODO (M7): remove the FIRST node whose value == item; return 1 if found,
     * 0 if not. You need the node BEFORE the target to rewire prev->next =
     * target->next. Watch three cases: removing the head, removing the tail
     * (update l->tail!), and removing a middle node. Free the removed node. */
    assert(0 && "TODO: implement ll_remove (README M7)");
    return 0;
}

/* ------------------------------------------------------------------ M8 ---- */

int ll_contains(const LinkedList *l, ll_elem item) {
    (void)l; (void)item;
    /* TODO (M8): walk from head; return 1 if any node's value == item, else 0. */
    assert(0 && "TODO: implement ll_contains (README M8)");
    return 0;
}

/* ------------------------------------------------------------------ M9 ---- */

void ll_reverse(LinkedList *l) {
    (void)l;
    /* TODO (M9): the canonical three-pointer walk.
     *     Node *prev = NULL, *cur = l->head;
     *     while (cur) { Node *next = cur->next; cur->next = prev;
     *                   prev = cur; cur = next; }
     * Then swap the roles of head and tail. Draw it for 3 nodes before coding. */
    assert(0 && "TODO: implement ll_reverse (README M9)");
}

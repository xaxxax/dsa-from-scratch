/*
 * doubly_linked_list.c — YOUR doubly linked list.
 *
 * SCAFFOLD: fill each TODO, milestone by milestone (README §4), `make test`
 * after each. ASan is on.
 *
 * The invariant, now TWO-directional:
 *   - for every node n except the head:  n->prev->next == n
 *   - for every node n except the tail:  n->next->prev == n
 *   - head->prev == NULL and tail->next == NULL
 *   - size == number of nodes
 *
 * Every insert/remove must leave BOTH directions consistent. The habit that
 * saves you: when you set a->next = b, immediately ask "did I also set
 * b->prev = a?" — the two writes come in pairs.
 */
#include "doubly_linked_list.h"

#include <assert.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ M1 ---- */

DoublyLinkedList *dll_new(void) {
    /* TODO (M1): malloc handle; head = tail = NULL; size = 0. Check NULL. */
    assert(0 && "TODO: implement dll_new (README M1)");
    return NULL;
}

void dll_free(DoublyLinkedList *l) {
    (void)l;
    /* TODO (M1): if NULL do nothing; else walk head->tail freeing each node
     * (save next before free!), then free the handle. Following `next` is fine;
     * you never need `prev` to free. */
    assert(0 && "TODO: implement dll_free (README M1)");
}

/* ------------------------------------------------------------------ M2 ---- */

size_t dll_size(const DoublyLinkedList *l) {
    (void)l;
    assert(0 && "TODO: implement dll_size (README M2)");
    return 0;
}

int dll_is_empty(const DoublyLinkedList *l) {
    (void)l;
    assert(0 && "TODO: implement dll_is_empty (README M2)");
    return 0;
}

/* ------------------------------------------------------------------ M3 ---- */

void dll_add_first(DoublyLinkedList *l, dll_elem item) {
    (void)l; (void)item;
    /* TODO (M3): new node; node->prev = NULL; node->next = l->head.
     * If head existed: l->head->prev = node. Else: l->tail = node.
     * Then l->head = node; size++. (Notice the pair: next AND prev.) */
    assert(0 && "TODO: implement dll_add_first (README M3)");
}

/* ------------------------------------------------------------------ M4 ---- */

void dll_add_last(DoublyLinkedList *l, dll_elem item) {
    (void)l; (void)item;
    /* TODO (M4): mirror of add_first. node->next = NULL; node->prev = l->tail.
     * If tail existed: l->tail->next = node. Else: l->head = node.
     * Then l->tail = node; size++. */
    assert(0 && "TODO: implement dll_add_last (README M4)");
}

/* ------------------------------------------------------------------ M5 ---- */

dll_elem dll_get(const DoublyLinkedList *l, size_t index) {
    (void)l; (void)index;
    /* TODO (M5): bounds-check (index >= size). Walk from head `index` hops.
     * STRETCH: if index > size/2, walk BACKWARD from tail — halves the average. */
    assert(0 && "TODO: implement dll_get (README M5)");
    return 0;
}

/* ------------------------------------------------------------------ M6 ---- */

dll_elem dll_remove_first(DoublyLinkedList *l) {
    (void)l;
    /* TODO (M6): empty-list policy. Else: victim = l->head; value = victim->value;
     * l->head = victim->next; if head now non-NULL set head->prev = NULL, else
     * l->tail = NULL (list emptied). free(victim); size--. */
    assert(0 && "TODO: implement dll_remove_first (README M6)");
    return 0;
}

/* ------------------------------------------------------------------ M7 ---- */

dll_elem dll_remove_last(DoublyLinkedList *l) {
    (void)l;
    /* TODO (M7): the payoff of `prev`. victim = l->tail; l->tail = victim->prev;
     * if tail now non-NULL set tail->next = NULL, else l->head = NULL.
     * free(victim); size--. This was O(n) in the singly list; here it's O(1). */
    assert(0 && "TODO: implement dll_remove_last (README M7)");
    return 0;
}

/* ------------------------------------------------------------------ M8 ---- */

int dll_remove(DoublyLinkedList *l, dll_elem item) {
    (void)l; (void)item;
    /* TODO (M8): find the first node with value == item. To unlink node n:
     *     if (n->prev) n->prev->next = n->next;  else l->head = n->next;
     *     if (n->next) n->next->prev = n->prev;  else l->tail = n->prev;
     * free(n); size--; return 1. Return 0 if not found. No `prev`-tracking walk
     * needed — every node already knows its predecessor. */
    assert(0 && "TODO: implement dll_remove (README M8)");
    return 0;
}

/* ------------------------------------------------------------------ M9 ---- */

int dll_contains(const DoublyLinkedList *l, dll_elem item) {
    (void)l; (void)item;
    /* TODO (M9): linear scan from head. */
    assert(0 && "TODO: implement dll_contains (README M9)");
    return 0;
}

/* ----------------------------------------------------------------- M10 ---- */

void dll_reverse(DoublyLinkedList *l) {
    (void)l;
    /* TODO (M10): for each node, swap its prev and next pointers; then swap
     * l->head and l->tail. Draw a 3-node list and confirm both directions still
     * satisfy the invariant afterward. */
    assert(0 && "TODO: implement dll_reverse (README M10)");
}

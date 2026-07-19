/*
 * doubly_linked_list.h — the CONTRACT for your doubly linked list.
 *
 * Same idea as Lesson 2, but every node now points BOTH ways:
 *
 *   NULL◀──┐  ┌────────┐  ┌────────┐  ┌────────┐  ┌──▶NULL
 *          prev        prev        prev        prev
 *        [10]────────▶[20]────────▶[30]────────▶[40]
 *          next        next        next        next
 *
 * The payoff of the extra `prev` pointer:
 *   - remove_last is O(1) (in the singly list it was O(n): no way back from tail)
 *   - given a pointer to ANY node you can unlink it in O(1)
 *   - you can traverse backward
 * The price: TWO links to fix on every insert/remove. Forget one and the chain
 * is silently corrupted in one direction — often invisible until you walk back.
 */
#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include <stddef.h>

typedef int dll_elem;

typedef struct DNode {
    dll_elem      value;
    struct DNode *prev;
    struct DNode *next;
} DNode;

typedef struct {
    DNode  *head;
    DNode  *tail;
    size_t  size;
} DoublyLinkedList;

/* ---- lifecycle ---------------------------------------------------------- */
DoublyLinkedList *dll_new(void);
void              dll_free(DoublyLinkedList *l);

/* ---- adding ------------------------------------------------------------- */
void dll_add_first(DoublyLinkedList *l, dll_elem item);   /* O(1) */
void dll_add_last (DoublyLinkedList *l, dll_elem item);   /* O(1) */

/* ---- reading ------------------------------------------------------------ */
dll_elem dll_get     (const DoublyLinkedList *l, size_t index); /* O(n) walk */
int      dll_contains(const DoublyLinkedList *l, dll_elem item);
size_t   dll_size    (const DoublyLinkedList *l);
int      dll_is_empty(const DoublyLinkedList *l);

/* ---- removing ----------------------------------------------------------- */
dll_elem dll_remove_first(DoublyLinkedList *l);            /* O(1) */
dll_elem dll_remove_last (DoublyLinkedList *l);            /* O(1) — the new win */
int      dll_remove      (DoublyLinkedList *l, dll_elem item); /* first match  */

/* ---- rearranging -------------------------------------------------------- */
void dll_reverse(DoublyLinkedList *l);  /* swap prev/next everywhere + head/tail */

#endif /* DOUBLY_LINKED_LIST_H */

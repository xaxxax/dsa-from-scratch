/*
 * queue.h — the CONTRACT for your FIFO queue.
 *
 * Composition again — but this time the array is the WRONG backing store, and
 * discovering why is the lesson. A queue adds at one end (back) and removes from
 * the OTHER end (front). On an array, removing the front is O(n) (shift all left).
 * On a singly linked list with head + tail, BOTH ends are O(1):
 *
 *     dequeue (remove_first)                 enqueue (add_last)
 *          ▼                                        ▼
 *        head ─▶ [A] ─▶ [B] ─▶ [C] ─▶ [D] ─▶ NULL   tail ─▶ D
 *        front                          back
 *
 * So the queue HAS-A LinkedList (your Lesson 2). enqueue delegates to add_last,
 * dequeue to remove_first. The stretch goal — a circular array buffer — shows how
 * an array CAN do O(1) at both ends by not shifting at all.
 */
#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>
#include "linked_list.h"   /* found via -I../02-singly-linked-list/src */

typedef ll_elem queue_elem;

typedef struct {
    LinkedList *backing;   /* HAS-A: front = list head, back = list tail */
} Queue;

Queue     *queue_new(void);
void       queue_free(Queue *q);

void       queue_enqueue(Queue *q, queue_elem item);  /* add at back  */
queue_elem queue_dequeue(Queue *q);                   /* remove from front */
queue_elem queue_peek   (const Queue *q);             /* look at front */

size_t     queue_size    (const Queue *q);
int        queue_is_empty(const Queue *q);

#endif /* QUEUE_H */

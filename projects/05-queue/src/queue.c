/*
 * queue.c — YOUR queue, composing the Lesson 2 singly linked list.
 *
 * SCAFFOLD (README §4). Short bodies — the insight is WHICH end maps to front vs.
 * back, and why the linked list beats the array here. Get ownership right:
 * queue_free frees the backing list, then the Queue struct.
 *
 * Mapping:  front  == list head  (dequeue = ll_remove_first)
 *           back   == list tail  (enqueue = ll_add_last)
 * Both are O(1) on your linked list — the tail pointer is what makes enqueue O(1).
 */
#include "queue.h"

#include <assert.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ M1 ---- */

Queue *queue_new(void) {
    /* TODO (M1): malloc Queue; q->backing = ll_new(); check both for NULL. */
    assert(0 && "TODO: implement queue_new (README M1)");
    return NULL;
}

void queue_free(Queue *q) {
    (void)q;
    /* TODO (M1): if NULL nothing; else ll_free(q->backing) THEN free(q). */
    assert(0 && "TODO: implement queue_free (README M1)");
}

/* ------------------------------------------------------------------ M2 ---- */

size_t queue_size(const Queue *q) {
    (void)q;
    /* TODO (M2): delegate to ll_size(q->backing). */
    assert(0 && "TODO: implement queue_size (README M2)");
    return 0;
}

int queue_is_empty(const Queue *q) {
    (void)q;
    /* TODO (M2): size == 0 (or ll_is_empty). */
    assert(0 && "TODO: implement queue_is_empty (README M2)");
    return 0;
}

/* ------------------------------------------------------------------ M3 ---- */

void queue_enqueue(Queue *q, queue_elem item) {
    (void)q; (void)item;
    /* TODO (M3): add at the BACK — ll_add_last. This is O(1) BECAUSE the linked
     * list keeps a tail pointer. (An array here would be O(1) at the back too —
     * the problem is the FRONT; see dequeue.) */
    assert(0 && "TODO: implement queue_enqueue (README M3)");
}

/* ------------------------------------------------------------------ M4 ---- */

queue_elem queue_peek(const Queue *q) {
    (void)q;
    /* TODO (M4): empty policy first. Otherwise the front is ll_get(backing, 0). */
    assert(0 && "TODO: implement queue_peek (README M4)");
    return 0;
}

/* ------------------------------------------------------------------ M5 ---- */

queue_elem queue_dequeue(Queue *q) {
    (void)q;
    /* TODO (M5): empty check first. Remove from the FRONT — ll_remove_first,
     * which returns the value. O(1). THIS is why the array is the wrong backing:
     * an array's remove-at-0 is O(n); the list's remove-first is O(1). */
    assert(0 && "TODO: implement queue_dequeue (README M5)");
    return 0;
}

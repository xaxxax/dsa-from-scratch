/*
 * test_queue.c — YOUR test harness for Project 5.
 *
 * Run:        make test
 * Leak audit: make && valgrind --leak-check=full --error-exitcode=1 ./out_test
 *
 * Exercises your Lesson 2 linked list THROUGH the queue. TODO-asserts from
 * linked_list.c mean: finish Lesson 2 first.
 */
#include "test_utils.h"
#include "queue.h"

int main(void) {

    Queue *q = queue_new();
    CHECK_INT(0, queue_size(q), "new queue size 0");
    CHECK(queue_is_empty(q), "new queue empty");

    queue_enqueue(q, 1);
    queue_enqueue(q, 2);
    queue_enqueue(q, 3);   /* front [1,2,3] back */
    CHECK_INT(3, queue_size(q), "size 3 after three enqueues");

    CHECK_INT(1, queue_peek(q), "peek sees the FRONT (1), not the back");
    CHECK_INT(1, queue_dequeue(q), "dequeue returns 1 (FIFO — oldest first)");
    CHECK_INT(2, queue_dequeue(q), "dequeue returns 2");
    CHECK_INT(1, queue_size(q), "size 1 after two dequeues");

    /* interleave to prove front/back independence */
    queue_enqueue(q, 4);   /* front [3,4] back */
    CHECK_INT(3, queue_dequeue(q), "3 still ahead of 4 (order preserved)");
    CHECK_INT(4, queue_dequeue(q), "then 4");
    CHECK(queue_is_empty(q), "empty after draining");

    /* reuse after empty */
    queue_enqueue(q, 99);
    CHECK_INT(99, queue_peek(q), "reusable after empty");

    queue_free(q);   /* frees backing list AND handle — zero leaks */

    /* ====================================================================== */
    /* TODO: enqueue/dequeue 100+ items and confirm strict FIFO order; test    */
    /* your empty-dequeue / empty-peek policy; queue_free on a fresh queue.     */
    /* ====================================================================== */

    return test_summary();
}

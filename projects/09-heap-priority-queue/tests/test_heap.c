/*
 * test_heap.c — YOUR test harness for Project 9.
 *
 * Run:        make test
 * Leak audit: make && valgrind --leak-check=full --error-exitcode=1 ./out_test
 *
 * The defining test of a min-heap: no matter what order you push, popping
 * repeatedly yields ascending order. (That's also heapsort.)
 */
#include "test_utils.h"
#include "heap.h"

int main(void) {
    MinHeap *h = heap_new();
    CHECK_INT(0, (int)heap_size(h), "new heap size 0");
    CHECK(heap_is_empty(h), "new heap empty");

    /* push in a jumbled order */
    int in[] = {5, 3, 8, 1, 9, 2, 7};
    for (size_t i = 0; i < sizeof in / sizeof in[0]; i++) heap_push(h, in[i]);
    CHECK_INT(7, (int)heap_size(h), "size 7 after 7 pushes");
    CHECK(!heap_is_empty(h), "not empty");

    CHECK_INT(1, heap_peek(h), "peek is the minimum (1)");
    CHECK_INT(7, (int)heap_size(h), "peek did not remove");

    /* popping must yield ascending order regardless of insertion order */
    int expected[] = {1, 2, 3, 5, 7, 8, 9};
    int ok = 1;
    for (size_t i = 0; i < sizeof expected / sizeof expected[0]; i++) {
        if (heap_pop(h) != expected[i]) { ok = 0; break; }
    }
    CHECK(ok, "pops come out sorted ascending (heap property holds)");
    CHECK(heap_is_empty(h), "empty after popping all");

    /* reuse + force a grow past initial capacity */
    for (int i = 100; i >= 1; i--) heap_push(h, i);   /* 100 down to 1 */
    CHECK_INT(100, (int)heap_size(h), "100 elements survive grows");
    CHECK_INT(1, heap_peek(h), "min is 1 after pushing 100..1");
    CHECK_INT(1, heap_pop(h), "pop 1");
    CHECK_INT(2, heap_pop(h), "pop 2");

    heap_free(h);   /* zero leaks under ASan/valgrind */

    /* ====================================================================== */
    /* TODO: push duplicates (heap must handle equal keys); pop down to empty  */
    /* then push again; single-element push/pop; your empty-pop/peek policy;   */
    /* heap_free on a fresh heap.                                              */
    /* ====================================================================== */

    return test_summary();
}

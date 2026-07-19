/*
 * test_linked_list.c — YOUR test harness for Project 2.
 *
 * Run:        make test      (from projects/02-singly-linked-list/)
 * Leak audit: make && valgrind --leak-check=full --error-exitcode=1 ./out_test
 *
 * The leak audit matters MORE here than in Lesson 1: every node is its own
 * malloc, so a broken ll_free or a dropped pointer leaks many blocks, not one.
 * Write tests as you go, one milestone at a time.
 */
#include "test_utils.h"     /* -I../../docs */
#include "linked_list.h"    /* -Isrc */

int main(void) {

    /* --- M1/M2: construct + empty ------------------------------------------ */
    LinkedList *l = ll_new();
    CHECK_INT(0, ll_size(l), "new list has size 0");
    CHECK(ll_is_empty(l), "new list is empty");

    /* --- M3/M5: add_first + get -------------------------------------------- */
    ll_add_first(l, 30);
    ll_add_first(l, 20);
    ll_add_first(l, 10);   /* list is now 10 -> 20 -> 30 */
    CHECK_INT(3, ll_size(l), "size 3 after three add_first");
    CHECK_INT(10, ll_get(l, 0), "get(0) == 10 (last added is head)");
    CHECK_INT(30, ll_get(l, 2), "get(2) == 30");

    /* --- M4: add_last uses the tail ---------------------------------------- */
    ll_add_last(l, 40);    /* 10 -> 20 -> 30 -> 40 */
    CHECK_INT(4, ll_size(l), "size 4 after add_last");
    CHECK_INT(40, ll_get(l, 3), "get(3) == 40 appended at tail");

    /* --- M8: contains ------------------------------------------------------ */
    CHECK(ll_contains(l, 20), "contains(20) true");
    CHECK(!ll_contains(l, 999), "contains(999) false");

    /* --- M6: remove_first -------------------------------------------------- */
    CHECK_INT(10, ll_remove_first(l), "remove_first returns old head 10");
    CHECK_INT(20, ll_get(l, 0), "new head is 20");
    CHECK_INT(3, ll_size(l), "size 3 after remove_first");

    /* --- M7: remove by value (middle, then tail) --------------------------- */
    CHECK(ll_remove(l, 30), "remove(30) found");         /* 20 -> 40 */
    CHECK(!ll_contains(l, 30), "30 gone after remove");
    CHECK(ll_remove(l, 40), "remove(40) found — was the tail");
    CHECK_INT(20, ll_get(l, 0), "20 is the only element left");
    CHECK_INT(1, ll_size(l), "size 1 after two removes");

    /* --- M9: reverse ------------------------------------------------------- */
    ll_add_last(l, 50);
    ll_add_last(l, 60);    /* 20 -> 50 -> 60 */
    ll_reverse(l);         /* 60 -> 50 -> 20 */
    CHECK_INT(60, ll_get(l, 0), "after reverse, head is old tail 60");
    CHECK_INT(20, ll_get(l, 2), "after reverse, tail is old head 20");
    ll_add_last(l, 7);     /* proves tail was fixed by reverse: 60->50->20->7 */
    CHECK_INT(7, ll_get(l, 3), "add_last after reverse lands correctly (tail intact)");

    ll_free(l);   /* <-- must free EVERY node; zero leaks under ASan/valgrind */

    /* ====================================================================== */
    /* TODO: add — remove head via ll_remove, remove the only element (list    */
    /* back to empty then add again), ll_get(l, (size_t)-1) unsigned trap,     */
    /* reverse of a single-element and empty list, ll_free on a fresh list.    */
    /* ====================================================================== */

    return test_summary();
}

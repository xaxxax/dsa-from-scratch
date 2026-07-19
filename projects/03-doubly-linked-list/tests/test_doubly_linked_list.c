/*
 * test_doubly_linked_list.c — YOUR test harness for Project 3.
 *
 * Run:        make test
 * Leak audit: make && valgrind --leak-check=full --error-exitcode=1 ./out_test
 *
 * Extra vigilance here: a bug can corrupt ONE direction while the other still
 * looks fine. Test by walking forward (get) AND by exercising remove_last /
 * add-after-remove, which only work if `prev`/`tail` stayed consistent.
 */
#include "test_utils.h"
#include "doubly_linked_list.h"

int main(void) {

    DoublyLinkedList *l = dll_new();
    CHECK_INT(0, dll_size(l), "new list size 0");
    CHECK(dll_is_empty(l), "new list empty");

    /* build 10 <-> 20 <-> 30 <-> 40 using both ends */
    dll_add_last(l, 20);
    dll_add_last(l, 30);
    dll_add_first(l, 10);
    dll_add_last(l, 40);
    CHECK_INT(4, dll_size(l), "size 4");
    CHECK_INT(10, dll_get(l, 0), "get(0) == 10");
    CHECK_INT(20, dll_get(l, 1), "get(1) == 20");
    CHECK_INT(40, dll_get(l, 3), "get(3) == 40");

    CHECK(dll_contains(l, 30), "contains(30)");
    CHECK(!dll_contains(l, 99), "!contains(99)");

    /* remove_last — the O(1) win the singly list couldn't do */
    CHECK_INT(40, dll_remove_last(l), "remove_last returns 40");
    CHECK_INT(30, dll_get(l, dll_size(l) - 1), "new last is 30");
    /* prove tail/prev stayed consistent: append should land right after 30 */
    dll_add_last(l, 99);
    CHECK_INT(99, dll_get(l, dll_size(l) - 1), "add_last after remove_last is consistent");
    CHECK_INT(30, dll_get(l, dll_size(l) - 2), "30 still precedes the new tail");

    /* remove_first */
    CHECK_INT(10, dll_remove_first(l), "remove_first returns 10");
    CHECK_INT(20, dll_get(l, 0), "new head is 20");

    /* remove by value from the middle (list: 20 <-> 30 <-> 99) */
    CHECK(dll_remove(l, 30), "remove(30) found");
    CHECK(!dll_contains(l, 30), "30 gone");
    CHECK_INT(2, dll_size(l), "size 2");
    CHECK_INT(20, dll_get(l, 0), "20 first");
    CHECK_INT(99, dll_get(l, 1), "99 second (links rejoined)");

    /* reverse: 20 <-> 99  ==>  99 <-> 20 */
    dll_reverse(l);
    CHECK_INT(99, dll_get(l, 0), "after reverse head is 99");
    CHECK_INT(20, dll_get(l, 1), "after reverse tail is 20");
    dll_add_last(l, 7);   /* tail must be intact */
    CHECK_INT(7, dll_get(l, dll_size(l) - 1), "add_last after reverse consistent");

    dll_free(l);   /* zero leaks */

    /* ====================================================================== */
    /* TODO: empty the list via remove_last down to 0 then rebuild; remove the */
    /* head and tail via dll_remove; unsigned trap dll_get(l,(size_t)-1);      */
    /* reverse of 0/1-element lists; dll_free on a fresh list.                 */
    /* ====================================================================== */

    return test_summary();
}

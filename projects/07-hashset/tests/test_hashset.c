/*
 * test_hashset.c — YOUR test harness for Project 7.
 *
 * Run:        make test
 * Leak audit: make && valgrind --leak-check=full --error-exitcode=1 ./out_test
 *
 * Exercises your Lesson 6 map THROUGH the set. TODO-asserts from hashmap.c mean:
 * finish Lesson 6 first.
 */
#include "test_utils.h"
#include "hashset.h"

int main(void) {
    HashSet *s = hs_new();
    CHECK_INT(0, (int)hs_size(s), "new set size 0");
    CHECK(!hs_contains(s, "x"), "empty set contains nothing");

    hs_add(s, "apple");
    hs_add(s, "banana");
    hs_add(s, "cherry");
    CHECK_INT(3, (int)hs_size(s), "size 3 after three distinct adds");
    CHECK(hs_contains(s, "banana"), "contains(banana)");
    CHECK(!hs_contains(s, "durian"), "!contains(durian)");

    /* idempotency: adding an existing member does not grow the set */
    hs_add(s, "apple");
    hs_add(s, "apple");
    CHECK_INT(3, (int)hs_size(s), "size still 3 after re-adding apple (idempotent)");

    /* remove */
    CHECK(hs_remove(s, "banana"), "remove(banana) returns 1");
    CHECK(!hs_contains(s, "banana"), "banana gone");
    CHECK_INT(2, (int)hs_size(s), "size 2 after remove");
    CHECK(!hs_remove(s, "banana"), "remove(banana) again returns 0");

    hs_free(s);   /* delegates to hm_free — zero leaks */

    /* ====================================================================== */
    /* TODO: add 1000 distinct keys (forces the backing map to resize) and     */
    /* confirm all present + size == 1000; add duplicates in bulk and confirm  */
    /* size counts distinct only; hs_free on a fresh set.                      */
    /* ====================================================================== */

    return test_summary();
}

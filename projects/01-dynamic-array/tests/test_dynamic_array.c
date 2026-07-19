/*
 * test_dynamic_array.c — YOUR test harness for Project 1.
 *
 * Run it with:   make test        (from projects/01-dynamic-array/)
 * Leak audit:    make && valgrind --leak-check=full --error-exitcode=1 ./out_test
 *
 * Write tests AS YOU GO, one milestone at a time — not all at the end. A few
 * starters show the rhythm; the rest are TODOs mapped to the milestones and edge
 * cases in README §5. Make them all pass, AND come back clean under the sanitizer.
 *
 * NOTE: because C has no exceptions, you can't unit-test an assert()-based
 * bounds abort (it kills the whole binary). Test the reachable happy/edge values;
 * verify abort paths by hand, OR switch to return-code error signaling and test
 * the codes with CHECK_INT.
 */
#include "test_utils.h"      /* found via the Makefile's -I../../docs */
#include "dynamic_array.h"   /* found via the Makefile's -Isrc */

int main(void) {

    /* --- M1/M2: construction, size, is_empty, and a clean free -------------- */
    DynamicArray *a = da_new();
    CHECK_INT(0, da_size(a), "new list has size 0");
    CHECK(da_is_empty(a), "new list is empty");

    /* --- M3/M4: add + get --------------------------------------------------- */
    da_add(a, 10);
    da_add(a, 20);
    da_add(a, 30);
    CHECK_INT(3, da_size(a), "size is 3 after three adds");
    CHECK_INT(10, da_get(a, 0), "get(0) == 10");
    CHECK_INT(30, da_get(a, 2), "get(2) == 30");
    CHECK(!da_is_empty(a), "non-empty after adds");

    /* --- M6: set ------------------------------------------------------------ */
    CHECK_INT(20, da_set(a, 1, 99), "set(1,99) returns old value 20");
    CHECK_INT(99, da_get(a, 1), "get(1) == 99 after set");

    /* --- M7: remove --------------------------------------------------------- */
    CHECK_INT(10, da_remove(a, 0), "remove(0) returns removed value 10");
    CHECK_INT(99, da_get(a, 0), "element shifted left: get(0) == 99");
    CHECK_INT(2, da_size(a), "size is 2 after remove");

    /* --- M8: contains ------------------------------------------------------- */
    CHECK(da_contains(a, 30), "contains(30) is true");
    CHECK(!da_contains(a, 12345), "contains(12345) is false");

    da_free(a);   /* <-- must leave ASan/valgrind with ZERO leaks */

    /* --- M5: growth across the capacity boundary ---------------------------- */
    DynamicArray *g = da_with_capacity(2);
    for (int i = 0; i < 100; i++) da_add(g, i);
    CHECK_INT(100, da_size(g), "100 elements survive many grows");
    CHECK_INT(0,  da_get(g, 0),  "first element intact after grows");
    CHECK_INT(50, da_get(g, 50), "middle element intact after grows");
    CHECK_INT(99, da_get(g, 99), "last element intact after grows");
    da_free(g);

    /* ====================================================================== */
    /* TODO: add the rest — remove-from-middle, remove-last, remove-to-empty-  */
    /* then-add, the unsigned index trap (da_get(a, (size_t)-1)), capacity     */
    /* sequence 4->8->16->32, and da_free on a fresh list. See README §5.      */
    /* ====================================================================== */

    return test_summary();
}

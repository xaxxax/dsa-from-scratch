/*
 * test_union_find.c — YOUR test harness for Project 14.
 *
 * Run:        make test
 * Leak audit: make && valgrind --leak-check=full --error-exitcode=1 ./out_test
 *
 * Two arrays plus a handle — the leak audit is simple, but DO run it: a failed
 * uf_new that leaks the first malloc, or a uf_free that forgets rank[], is
 * exactly the kind of bug valgrind names instantly.
 */
#include "test_utils.h"
#include "union_find.h"

int main(void) {
    /* 10 singletons: {0} {1} ... {9} */
    UnionFind *uf = uf_new(10);
    CHECK_INT(10, (int)uf_count(uf), "10 singletons at start");
    CHECK(!uf_connected(uf, 1, 2), "1 and 2 start disconnected");
    CHECK(uf_connected(uf, 3, 3), "every element is connected to itself");

    /* build the set {1,2,3} via two unions */
    CHECK(uf_union(uf, 1, 2), "union(1,2) merges two sets -> 1");
    CHECK(uf_union(uf, 2, 3), "union(2,3) merges two sets -> 1");
    CHECK_INT(8, (int)uf_count(uf), "two merges: 10 -> 8 sets");

    CHECK(uf_connected(uf, 1, 3), "1 and 3 now connected (transitively)");
    CHECK(!uf_connected(uf, 1, 4), "1 and 4 still separate");

    /* a union of two elements ALREADY in the same set changes nothing */
    CHECK(!uf_union(uf, 1, 3), "union(1,3) already together -> 0");
    CHECK_INT(8, (int)uf_count(uf), "count unchanged after redundant union");

    /* find is consistent: every member of {1,2,3} reports the same root */
    size_t r1 = uf_find(uf, 1);
    CHECK_INT((int)r1, (int)uf_find(uf, 2), "1 and 2 share a root");
    CHECK_INT((int)r1, (int)uf_find(uf, 3), "1 and 3 share a root");

    /* build a second set {4,5}, then merge the two big sets into one */
    uf_union(uf, 4, 5);
    CHECK(!uf_connected(uf, 3, 5), "{1,2,3} and {4,5} still distinct");
    CHECK(uf_union(uf, 3, 4), "bridging the two components merges them");
    CHECK(uf_connected(uf, 1, 5), "1 reaches 5 after the bridge");
    CHECK_INT(6, (int)uf_count(uf), "count now 6");

    /* stress: union a chain 0-1-2-...-9 forces path compression to matter */
    UnionFind *chain = uf_new(1000);
    for (size_t i = 0; i + 1 < 1000; i++) uf_union(chain, i, i + 1);
    CHECK_INT(1, (int)uf_count(chain), "chaining 1000 elements -> a single set");
    CHECK(uf_connected(chain, 0, 999), "0 and 999 connected through the chain");
    uf_free(chain);

    uf_free(uf);   /* frees parent[], rank[], and the handle — zero leaks */

    /* ====================================================================== */
    /* TODO: add these edge cases yourself:                                    */
    /*   - uf_new(1): a single element; count 1; connected(0,0) true.          */
    /*   - the unsigned trap: uf_find(uf,(size_t)-1) — what's your policy?      */
    /*   - uf_free(NULL) is a no-op (must not crash).                          */
    /*   - cycle detection: on a set of edges, count how many uf_union return   */
    /*     0 (each 0 is an edge that closed a cycle).                          */
    /*   - after many unions, assert uf_count equals the true number of roots. */
    /* ====================================================================== */

    return test_summary();
}

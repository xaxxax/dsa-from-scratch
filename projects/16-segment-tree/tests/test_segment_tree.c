/*
 * test_segment_tree.c — YOUR test harness for Project 16.
 *
 * Run:        make test
 * Leak audit: make && valgrind --leak-check=full --error-exitcode=1 ./out_test
 *
 * The defining property: a query after an update reflects the change, and both are
 * far cheaper than rescanning. Cross-check every query against a brute-force sum.
 */
#include "test_utils.h"
#include "segment_tree.h"

int main(void) {
    int vals[] = {5, 2, 7, 1, 9, 3};   /* n = 6 */
    size_t n = sizeof vals / sizeof vals[0];

    SegmentTree *t = seg_new(vals, n);
    CHECK_INT(6, (int)seg_size(t), "size == n");

    /* range sums against hand-computed answers */
    CHECK_INT(5,  (int)seg_query(t, 0, 0), "query[0,0] = 5");
    CHECK_INT(7,  (int)seg_query(t, 0, 1), "query[0,1] = 5+2 = 7");
    CHECK_INT(10, (int)seg_query(t, 1, 3), "query[1,3] = 2+7+1 = 10");
    CHECK_INT(27, (int)seg_query(t, 0, 5), "query[0,5] = whole array = 27");
    CHECK_INT(12, (int)seg_query(t, 3, 5), "query[3,5] = 1+9+3 = 12");

    /* point update, then re-query the affected ranges */
    seg_update(t, 2, 10);   /* index 2: 7 -> 10  (array now 5,2,10,1,9,3) */
    CHECK_INT(13, (int)seg_query(t, 1, 3), "after update, query[1,3] = 2+10+1 = 13");
    CHECK_INT(30, (int)seg_query(t, 0, 5), "after update, whole sum = 30");
    CHECK_INT(10, (int)seg_query(t, 2, 2), "single point reflects the update");

    /* another update, elsewhere */
    seg_update(t, 5, 0);    /* index 5: 3 -> 0  (array now 5,2,10,1,9,0) */
    CHECK_INT(27, (int)seg_query(t, 0, 5), "whole sum after 2nd update = 27");
    CHECK_INT(10, (int)seg_query(t, 3, 5), "query[3,5] = 1+9+0 = 10");

    seg_free(t);   /* zero leaks */

    /* ====================================================================== */
    /* TODO: single-element tree (n=1); update the same index twice; query the */
    /* full range vs. sum of two half-range queries (should match); brute-force */
    /* random test: for random updates/queries, compare against a plain loop.   */
    /* ====================================================================== */

    return test_summary();
}

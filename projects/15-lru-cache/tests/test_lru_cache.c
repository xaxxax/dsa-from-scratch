/*
 * test_lru_cache.c — YOUR test harness for Project 15 (mirrors LeetCode 146).
 *
 * Run:        make test
 * Leak audit: make && valgrind --leak-check=full --error-exitcode=1 ./out_test
 *
 * The leak audit is essential: eviction must free the victim AND remove it from
 * both the list and the hash map. A node freed from one structure but still
 * referenced by the other is a use-after-free valgrind will catch.
 */
#include "test_utils.h"
#include "lru_cache.h"

int main(void) {
    LRUCache *c = lru_new(2);   /* capacity 2 */
    int out = -1;

    CHECK_INT(0, (int)lru_size(c), "new cache size 0");
    CHECK(!lru_get(c, 1, &out), "miss on empty cache");

    lru_put(c, 1, 10);          /* cache: {1=10}                 MRU→ 1 */
    lru_put(c, 2, 20);          /* cache: {1=10, 2=20}           MRU→ 2,1 */
    CHECK_INT(2, (int)lru_size(c), "size 2 after two puts");

    CHECK(lru_get(c, 1, &out), "get(1) hit");
    CHECK_INT(10, out, "get(1) == 10 (and 1 is now MRU)");   /* order: 1,2 */

    lru_put(c, 3, 30);          /* over capacity → evict LRU (2). cache: {1,3} */
    CHECK_INT(2, (int)lru_size(c), "size stays at capacity 2");
    CHECK(!lru_get(c, 2, &out), "2 was evicted (it was least-recently used)");
    CHECK(lru_get(c, 1, &out), "1 survived (it was used more recently)");
    CHECK_INT(10, out, "1 still maps to 10");
    CHECK(lru_get(c, 3, &out), "3 present");
    CHECK_INT(30, out, "3 == 30");

    /* update existing key: value changes, no eviction, becomes MRU */
    lru_put(c, 1, 111);         /* order now: 1,3 */
    CHECK_INT(2, (int)lru_size(c), "update does not change size");
    CHECK(lru_get(c, 1, &out), "get(1) after update");
    CHECK_INT(111, out, "1 == 111 (updated in place)");

    lru_put(c, 4, 40);          /* evict LRU (3). cache: {1,4} */
    CHECK(!lru_get(c, 3, &out), "3 evicted after 1 was refreshed");
    CHECK(lru_get(c, 4, &out), "4 present");

    lru_free(c);   /* frees every node + buckets — ZERO leaks */

    /* ====================================================================== */
    /* TODO: capacity 1 (every put evicts); put the SAME key repeatedly (no    */
    /* growth, no eviction); interleave get/put so the LRU victim changes;     */
    /* fill, evict all, refill; lru_free on a fresh cache.                     */
    /* ====================================================================== */

    return test_summary();
}

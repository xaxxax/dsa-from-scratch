/*
 * test_hashmap.c — YOUR test harness for Project 6.
 *
 * Run:        make test
 * Leak audit: make && valgrind --leak-check=full --error-exitcode=1 ./out_test
 *
 * The leak audit is CRITICAL here — three layers of allocation (buckets, entries,
 * keys). A common bug is freeing entries but not their strdup'd keys; valgrind
 * catches exactly that.
 */
#include "test_utils.h"
#include "hashmap.h"

int main(void) {
    HashMap *m = hm_new();
    CHECK_INT(0, (int)hm_size(m), "new map size 0");

    hm_value out = -1;
    CHECK(!hm_get(m, "missing", &out), "get on empty returns 0 (not found)");

    /* --- put + get --------------------------------------------------------- */
    hm_put(m, "cat", 1);
    hm_put(m, "dog", 2);
    hm_put(m, "fox", 9);
    CHECK_INT(3, (int)hm_size(m), "size 3 after three puts");

    CHECK(hm_get(m, "cat", &out), "get(cat) found");
    CHECK_INT(1, out, "cat -> 1");
    CHECK(hm_get(m, "fox", &out), "get(fox) found");
    CHECK_INT(9, out, "fox -> 9");
    CHECK(hm_contains(m, "dog"), "contains(dog)");
    CHECK(!hm_contains(m, "cow"), "!contains(cow)");

    /* --- put on existing key UPDATES, does not duplicate ------------------- */
    hm_put(m, "cat", 100);
    CHECK_INT(3, (int)hm_size(m), "size still 3 after updating cat");
    CHECK(hm_get(m, "cat", &out), "get(cat) after update");
    CHECK_INT(100, out, "cat -> 100 (updated in place)");

    /* --- remove ------------------------------------------------------------ */
    CHECK(hm_remove(m, "dog"), "remove(dog) returns 1");
    CHECK(!hm_contains(m, "dog"), "dog gone");
    CHECK_INT(2, (int)hm_size(m), "size 2 after remove");
    CHECK(!hm_remove(m, "dog"), "remove(dog) again returns 0");

    hm_free(m);   /* frees buckets + entries + every key — ZERO leaks */

    /* --- resize / rehash under load --------------------------------------- */
    HashMap *big = hm_with_capacity(4);   /* tiny, to force resizes */
    char key[16];
    for (int i = 0; i < 1000; i++) {
        snprintf(key, sizeof key, "k%d", i);
        hm_put(big, key, i);
    }
    CHECK_INT(1000, (int)hm_size(big), "1000 entries survive many resizes");
    int all_ok = 1;
    for (int i = 0; i < 1000; i++) {
        snprintf(key, sizeof key, "k%d", i);
        hm_value v;
        if (!hm_get(big, key, &v) || v != i) { all_ok = 0; break; }
    }
    CHECK(all_ok, "every one of 1000 keys retrievable with correct value after rehash");
    CHECK(hm_load_factor(big) <= 0.75, "load factor stayed under threshold via resize");
    hm_free(big);

    /* ====================================================================== */
    /* TODO: force a collision on purpose (with capacity 1, all keys chain);   */
    /* remove the head vs. a middle-of-chain entry; put/get empty-string key;  */
    /* update then remove; hm_free on a fresh map.                             */
    /* ====================================================================== */

    return test_summary();
}

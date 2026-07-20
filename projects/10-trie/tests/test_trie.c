/*
 * test_trie.c — YOUR test harness for Project 10.
 *
 * Run:        make test
 * Leak audit: make && valgrind --leak-check=full --error-exitcode=1 ./out_test
 *
 * The leak audit matters: a trie is many nodes, and trie_free must recurse into
 * all 26 children of every node. A forgotten branch leaks silently.
 */
#include "test_utils.h"
#include "trie.h"

int main(void) {
    Trie *t = trie_new();
    CHECK_INT(0, (int)trie_size(t), "new trie has 0 words");
    CHECK(!trie_contains(t, "cat"), "empty trie contains nothing");
    CHECK(!trie_starts_with(t, "c"), "empty trie has no prefixes");

    trie_insert(t, "cat");
    trie_insert(t, "car");
    trie_insert(t, "dog");
    CHECK_INT(3, (int)trie_size(t), "3 words after 3 inserts");

    /* full-word membership */
    CHECK(trie_contains(t, "cat"), "contains(cat)");
    CHECK(trie_contains(t, "car"), "contains(car)");
    CHECK(trie_contains(t, "dog"), "contains(dog)");
    CHECK(!trie_contains(t, "ca"), "contains(ca) FALSE — a prefix is not a word");
    CHECK(!trie_contains(t, "cats"), "contains(cats) FALSE — longer than stored");
    CHECK(!trie_contains(t, "cow"), "contains(cow) FALSE — never inserted");

    /* prefix search — the trie's signature */
    CHECK(trie_starts_with(t, "ca"), "starts_with(ca) TRUE (cat, car)");
    CHECK(trie_starts_with(t, "d"),  "starts_with(d) TRUE (dog)");
    CHECK(trie_starts_with(t, "cat"), "starts_with(cat) TRUE (a full word is its own prefix)");
    CHECK(!trie_starts_with(t, "z"), "starts_with(z) FALSE");
    CHECK(!trie_starts_with(t, "cad"), "starts_with(cad) FALSE");

    /* idempotent insert */
    trie_insert(t, "cat");
    CHECK_INT(3, (int)trie_size(t), "re-inserting cat keeps size 3");

    trie_free(t);   /* recurse-free every node — ZERO leaks */

    /* ====================================================================== */
    /* TODO: insert a word that is a prefix of another ("car" then "care") and */
    /* both a word ("car") — contains both; insert where one word is a prefix  */
    /* of an existing one; your empty-string policy; trie_free on a fresh trie. */
    /* ====================================================================== */

    return test_summary();
}

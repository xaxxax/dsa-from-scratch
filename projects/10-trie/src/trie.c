/*
 * trie.c — YOUR trie (prefix tree). SCAFFOLD (README §4).
 *
 * Invariants:
 *   - the root represents the empty prefix and holds no letter of its own
 *   - a node reached by spelling S has is_end_of_word == 1 iff S was inserted
 *   - children[c] is non-NULL iff some inserted word has the next letter c here
 *   - size == number of distinct words (nodes with is_end_of_word set)
 *
 * Map a character to a slot with (c - 'a'); this scaffold assumes lowercase a-z.
 * Deciding what to do with out-of-range characters is one of your design calls.
 */
#include "trie.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>   /* strlen if you want it */

/* Allocate one all-NULL-children node (calloc is perfect). Returns NULL on OOM.
 * Unused until trie_new/insert call it — expected "unused" warning until M1. */
static TrieNode *node_new(void) {
    /* TODO (M1): calloc a TrieNode (calloc zeroes children[] to NULL and
     * is_end_of_word to 0). Return it; check for NULL. */
    assert(0 && "TODO: implement node_new (README M1)");
    return NULL;
}

/* Recursively free a node and its whole subtree. Post-order: free all 26 children
 * FIRST, then the node itself. Unused until trie_free calls it (M1). */
static void node_free(TrieNode *n) {
    (void)n;
    /* TODO (M1): if n is NULL return. For each of the 26 children, node_free it.
     * THEN free(n). (Freeing n first would strand its children — a leak.) */
    assert(0 && "TODO: implement node_free (README M1)");
}

/* ------------------------------------------------------------------ M1 ---- */

Trie *trie_new(void) {
    /* TODO (M1): malloc the Trie; t->root = node_new(); size = 0. Check both. */
    assert(0 && "TODO: implement trie_new (README M1)");
    return NULL;
}

void trie_free(Trie *t) {
    (void)t;
    /* TODO (M1): if NULL nothing; else node_free(t->root); then free(t). */
    assert(0 && "TODO: implement trie_free (README M1)");
}

/* ------------------------------------------------------------------ M2 ---- */

void trie_insert(Trie *t, const char *word) {
    (void)t; (void)word;
    /* TODO (M2): walk from the root, one character at a time. For each char c,
     * let idx = c - 'a'; if children[idx] is NULL, create it (node_new). Descend.
     * After the last char, if that node's is_end_of_word was already 1 the word
     * existed (don't double-count); otherwise set it to 1 and size++.
     * Decide: empty string? non-lowercase chars? Document your policy. */
    assert(0 && "TODO: implement trie_insert (README M2)");
}

/* ------------------------------------------------------------------ M3 ---- */

int trie_contains(const Trie *t, const char *word) {
    (void)t; (void)word;
    /* TODO (M3): walk the word char by char; if any child is missing, return 0.
     * If you reach the end, return the final node's is_end_of_word (NOT just
     * "the path exists" — "ca" path exists for "cat" but "ca" isn't a word). */
    assert(0 && "TODO: implement trie_contains (README M3)");
    return 0;
}

/* ------------------------------------------------------------------ M4 ---- */

int trie_starts_with(const Trie *t, const char *prefix) {
    (void)t; (void)prefix;
    /* TODO (M4): identical walk to contains, but the answer is just "did the path
     * survive to the end of the prefix?" — return 1 if you never hit a NULL child,
     * regardless of is_end_of_word. This is the trie's signature operation. */
    assert(0 && "TODO: implement trie_starts_with (README M4)");
    return 0;
}

/* ------------------------------------------------------------------ M5 ---- */

size_t trie_size(const Trie *t) {
    (void)t;
    /* TODO (M5): return the maintained word count. */
    assert(0 && "TODO: implement trie_size (README M5)");
    return 0;
}

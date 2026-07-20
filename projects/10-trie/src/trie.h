/*
 * trie.h — the CONTRACT for your trie (prefix tree) over lowercase a-z.
 *
 * A trie stores strings by their CHARACTERS, not by hashing. Each node has one
 * child slot per possible next letter; a path from the root spells a prefix, and
 * a flag marks nodes where a complete word ends.
 *
 *   words {"cat","car","dog"}:
 *
 *            (root)
 *           /      \
 *          c        d
 *          |        |
 *          a        o
 *         / \       |
 *        t*  r*     g*         (* = is_end_of_word)
 *
 *   "ca" is a valid PREFIX (starts_with) but not a word (contains) — the node at
 *   'a' has no end-flag. That prefix/word distinction is the whole point.
 *
 * WHY IT'S FAST: every operation is O(L) in the WORD LENGTH, completely
 * independent of how many words are stored. A million words don't slow down a
 * 4-letter lookup — you only ever walk 4 nodes.
 *
 * THE C COST: each node carries 26 child pointers (mostly NULL). That's a lot of
 * memory, and trie_free must recurse into ALL 26 children of every node before
 * freeing it — the deepest recursive free in the course.
 */
#ifndef TRIE_H
#define TRIE_H

#include <stddef.h>

#define TRIE_ALPHABET 26   /* lowercase 'a'..'z'; map with c - 'a' */

typedef struct TrieNode {
    struct TrieNode *children[TRIE_ALPHABET];
    int              is_end_of_word;   /* 1 iff a word ends exactly here */
} TrieNode;

typedef struct {
    TrieNode *root;   /* the empty-prefix node; never holds a letter itself */
    size_t    size;   /* number of distinct words inserted */
} Trie;

/* ---- lifecycle ---------------------------------------------------------- */
Trie *trie_new(void);
void  trie_free(Trie *t);   /* recursively free every node */

/* ---- core operations ---------------------------------------------------- */
void trie_insert     (Trie *t, const char *word);    /* idempotent for a repeat */
int  trie_contains   (const Trie *t, const char *word);   /* full word present? */
int  trie_starts_with(const Trie *t, const char *prefix); /* any word with prefix? */

size_t trie_size(const Trie *t);   /* number of distinct words */

#endif /* TRIE_H */

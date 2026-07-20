# Project 10 — Trie (prefix tree)

> **You are building:** a tree keyed by the **characters** of strings — the
> structure behind autocomplete, spell-checkers, IP routing tables, and word games.
> It answers "is this a word?" and "does any word start with this prefix?" in time
> proportional to the **word length**, not the number of words.
>
> **Builds on:** tree/recursion fluency (Lesson 8) and per-node allocation
> (Lesson 2). No hashing — a completely different way to index strings than
> Lesson 6.
>
> **The C leap:** each node holds a fixed array of **26 child pointers** (mostly
> NULL), and `trie_free` is the deepest recursive free yet — you must descend into
> all 26 children of every node before freeing it. Miss one branch and you leak a
> whole subtree.
>
> **Mentor rule:** diagrams and questions here, not code.

---

## 1. Motivation

### The problem

You have a dictionary of hundreds of thousands of words and need **autocomplete**:
given "ca", instantly list/confirm words that start that way. A hash set (Lesson 7)
gives O(1) *exact* membership but knows nothing about *prefixes* — hashing
deliberately scatters "cat" and "car" to unrelated buckets. You need a structure
where shared prefixes are shared *physically*.

### The idea

Store words by walking their letters. Each **node** has one slot per possible next
character; the path from the root spells a prefix; a flag marks nodes where a
complete word ends. Words with a common prefix **share the same nodes** for that
prefix — "cat" and "car" share `c→a`, then branch at `t` vs `r`.

Now:
- **contains(word):** walk the letters; the word exists iff you reach its final node
  *and that node is flagged as a word end*.
- **starts_with(prefix):** walk the letters; a word with that prefix exists iff the
  path survives to the end (end-flag irrelevant).

### The trade-off

Operations are **O(L)** in the word length, *independent of how many words are
stored* — a million-word trie answers a 4-letter query in 4 steps. The price is
**memory**: every node reserves 26 pointers whether or not they're used, so a trie
can be far larger than a hash set for the same words. Speed and prefix power for
space — the recurring theme.

---

## 2. Visualization

```
 insert "cat", "car", "dog":

            (root)                 * = is_end_of_word
           /      \
          c        d
          |        |
          a        o
         / \       |
        t*  r*     g*

 contains("cat")  → walk c,a,t → node t has * → TRUE
 contains("ca")   → walk c,a   → node a has NO * → FALSE (prefix, not a word)
 starts_with("ca")→ walk c,a   → path survived  → TRUE
 starts_with("cz")→ walk c, then z child is NULL → FALSE
```

Now insert `"car"` after `"care"` exists, or `"care"` after `"car"`: the end-flag
lives on the node where each word *ends*, so a word can be a prefix of another and
both are independently "words." **Draw that case** — it's the one people get wrong.

---

## 3. Design

```c
#define TRIE_ALPHABET 26

typedef struct TrieNode {
    struct TrieNode *children[TRIE_ALPHABET];   /* index = c - 'a' */
    int              is_end_of_word;
} TrieNode;

typedef struct { TrieNode *root; size_t size; } Trie;
```

### Design questions

1. **Path vs. word.** A path existing (`starts_with`) is *not* the same as a word
   existing (`contains`). The `is_end_of_word` flag is the entire difference —
   articulate why you need it and where it lives.
2. **Character → index.** `c - 'a'` maps 'a'..'z' to 0..25. What do you do with an
   uppercase letter, a digit, an apostrophe, or the empty string? Decide a policy
   (reject? lowercase? widen the alphabet?) and document it.
3. **`calloc` for nodes.** A node's 26 children must all start NULL. `calloc` zeroes
   them in one shot — why is that both correct and faster than a 26-iteration loop?
4. **The recursive free.** `trie_free` → `node_free(root)`, and `node_free` must
   recurse into all 26 children *before* freeing the node (post-order), exactly like
   the BST free but 26-way.
5. **Alphabet size trade-off.** 26 keeps it simple; a 256-way byte trie handles any
   text but 10× the pointer memory; a *compressed* (radix) trie collapses single-
   child chains to save space. Note these; the simple version first.

---

## 4. Milestones

Scaffold: [`src/trie.c`](src/trie.c) / [`src/trie.h`](src/trie.h).

```
[ ] M0  Build: header + compiling .c with TODO-asserts; make test aborts at trie_new.
        "defined but not used" warnings for node_new/node_free are EXPECTED until M1.
[ ] M1  node_new (calloc) + node_free (recursive, post-order); trie_new / trie_free.
        new-then-free on an empty trie: ZERO leaks under ASan.
[ ] M2  trie_insert — walk/create nodes; set is_end_of_word at the last char; count
        distinct words (don't double-count a re-insert).
[ ] M3  trie_contains — walk; TRUE only if the final node is flagged a word end.
[ ] M4  trie_starts_with — walk; TRUE if the prefix path survives (flag irrelevant).
[ ] M5  trie_size.
[ ] M6  (Stretch) trie_delete(word) — unset the flag AND prune now-empty nodes (the
        tricky part); collect_with_prefix (autocomplete list); a compressed/radix
        trie; a 256-way byte trie.
[ ] M7  Full suite green under ASan; valgrind clean. Request review.
```

---

## 5. Testing

```bash
cd projects/10-trie
make test
make && valgrind --leak-check=full --error-exitcode=1 ./out_test
```

Starter: [`tests/test_trie.c`](tests/test_trie.c).

### Edge cases (test all)

- **Prefix vs. word:** `contains("ca")` is FALSE while `starts_with("ca")` is TRUE.
- **A word that is a prefix of another:** insert "car" and "care"; both `contains`
  true. Insert in both orders — the end-flags must be independent.
- **Longer than stored:** `contains("cats")` FALSE when only "cat" exists.
- **Idempotent insert:** re-inserting "cat" doesn't change `size`.
- **Missing branch:** `starts_with("cz")` FALSE (NULL child mid-walk).
- **Empty string / non-lowercase:** exercise your documented policy.
- **`trie_free` on a fresh trie**, and after many inserts — valgrind zero leaks.

---

## 6. Complexity Analysis

Derive first. L = length of the word/prefix; ALPHABET = 26.

| Operation | Time | Space |
|-----------|:----:|:-----:|
| `insert` | ? | ? |
| `contains` | ? | ? |
| `starts_with` | ? | ? |

<details>
<summary>Answers.</summary>

- All three: **O(L)** time — you touch one node per character, *independent of the
  number of stored words*. That independence is the trie's superpower.
- Space: **O(total characters × ALPHABET)** worst case — every node reserves 26
  pointers. With shared prefixes the real cost is lower, but a trie is typically far
  larger than a hash set for the same words. That memory buys prefix queries a hash
  set can't do at all.
- Compare a hash set: O(L) to *hash* the string too (you read all its chars), O(1)
  average lookup — but **zero** prefix capability. Different tools for different jobs.
</details>

---

## 7. Real-World Usage

- **Autocomplete / typeahead** — the canonical use; `starts_with` + collecting the
  subtree gives suggestions.
- **Spell-checkers & fuzzy search** — tries support edit-distance walks.
- **IP routing (longest-prefix match)** — routers use bitwise tries (Patricia/radix)
  to pick the most specific matching route at line rate.
- **Word games / Scrabble / Boggle solvers** — prune impossible prefixes instantly.
- **Predictive text, T9, DNA k-mer indexing, URL routers** (path-prefix matching).

---

## 8. LeetCode

**No solutions here.**

1. **[Medium] 208. Implement Trie (Prefix Tree)** — literally this project.
2. **[Medium] 211. Design Add and Search Words** — trie + `.` wildcard (DFS).
3. **[Medium] 648. Replace Words** — dictionary of roots via a trie.
4. **[Medium] 677. Map Sum Pairs** — trie storing values, prefix-sum.
5. **[Hard] 212. Word Search II** — a trie over a DFS grid; a beautiful combo.

---

## Exercises

1. **`trie_count_words_with_prefix(prefix)`** — walk to the prefix node, then count
   the flagged nodes in its subtree (recursion).
2. **`trie_collect(prefix, out[])`** — autocomplete: gather every word under a
   prefix. Who allocates/frees the strings?
3. **Longest common prefix** of all stored words — walk down while there's exactly
   one child and no word ends.

## Challenge problems

1. **`trie_delete(word)`** — unset the end-flag, then prune nodes that are now
   childless and non-terminal, bottom-up. Why can't you just free the leaf? (Other
   words may share the path.) This is trickier than it looks — the classic trie hard
   part.
2. **Compressed (radix) trie** — collapse chains of single-child nodes into one edge
   labeled with a substring. Big memory win; harder inserts/splits.
3. **256-way byte trie** — handle arbitrary bytes/UTF-8. Compare memory to the 26-way
   version on the same words.

## Stretch goals

- **Ternary search trie** — a space-saving middle ground between a BST and a trie;
  each node has lo/eq/hi children. Great for large alphabets.
- **Autocomplete server** — load a word list, expose prefix queries, and measure how
  query time stays flat as the dictionary grows (the trie's whole promise).

## Common mistakes

- **Returning "path exists" for `contains`** — you must check `is_end_of_word`, or
  "ca" reports as a word just because "cat" is stored.
- **Freeing a node before its children** — recurse first (post-order), or you strand
  and leak the subtree. valgrind will list every leaked node.
- **Not using `calloc`** — a node with garbage child pointers dereferences wild
  memory. Zero the children.
- **Double-counting `size`** — a re-insert of an existing word must not increment.
- **Assuming input is clean** — an out-of-range `c - 'a'` indexes outside
  `children[26]` (buffer overflow — ASan catches it). Validate or document.

## Debugging tips

- Write a `trie_print` that DFS-dumps words (accumulate the path string, print at
  flagged nodes). Seeing the stored words is the fastest sanity check.
- Test `contains` vs. `starts_with` on the *same* prefix side by side — the
  difference localizes end-flag bugs.
- valgrind after inserting many words; the recursive free is easy to get almost right.
- Reproduce with two words that share a prefix and diverge — the minimal interesting
  trie.

## Frequently asked questions

**Q: Why use a trie instead of a hash set?**
A: A hash set gives O(1) *exact* membership but no notion of prefixes — it
deliberately scatters similar strings. A trie makes prefix queries (`starts_with`,
autocomplete, longest-prefix-match) natural and O(L), because shared prefixes share
nodes. If you never need prefixes, a hash set is smaller and simpler; the moment you
do, the trie is in a different league.

**Q: 26 pointers per node sounds wasteful — is it?**
A: It can be, for sparse data. That's exactly why radix/compressed tries (collapse
single-child chains) and ternary search tries exist — they trade some simplicity for
big memory savings. Build the plain 26-way version to understand the idea, then
appreciate why the compressed variants were invented.

**Q: How does IP routing use this?**
A: Routers store network prefixes in a bitwise trie and, for each packet, walk the
destination address bit by bit to find the **longest matching prefix** (the most
specific route). It's this structure, over bits instead of letters, running at
enormous speed.

---

## Definition of done

- [ ] Milestones M1–M5 implemented (M6–M7 recommended; delete is great practice).
- [ ] Full suite passes, especially prefix-vs-word and word-is-a-prefix-of-another.
- [ ] **`make test` clean under ASan/UBSan; `valgrind` zero leaks** across the
      recursive free.
- [ ] §6 table filled from your reasoning; you can explain why operations are O(L)
      independent of word count, and the space trade-off vs. a hash set.
- [ ] `PROGRESS.md` written. Code review requested.

→ Next: [`../11-graph/`](../11-graph/) — from trees (one parent, no cycles) to the
fully general structure: vertices connected by arbitrary edges, the substrate for
DFS, BFS, and shortest paths.

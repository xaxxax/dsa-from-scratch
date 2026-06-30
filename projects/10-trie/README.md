# Project 10 — Trie (Prefix Tree)

> **Status:** lesson outline (full eight-section treatment follows the Project 1
> template).

**Builds on:** Project 6 (each node maps a character to a child — your HashMap,
or a fixed array of 26).

## The core question
You can store words in a HashSet and check membership in O(1). But how do you
answer **"which stored words start with `pre`?"** — autocomplete, spell-check,
IP routing? A hash set throws away the *structure* of the keys. We want to exploit
shared **prefixes**.

## The idea
A **trie** stores strings character by character along paths from the root.
Shared prefixes share nodes. A boolean "end of word" flag marks complete words.

```
root
 ├─ c ─ a ─ t*          words: cat, car, card, dog
 │       └─ r* ─ d*     (* = end-of-word)
 └─ d ─ o ─ g*

"ca" is a prefix of cat, car, card → all reachable by walking c→a then exploring.
```

## What you'll implement
- A `TrieNode` (children map/array + `isEndOfWord`).
- `insert(word)`, `search(word)`, `startsWith(prefix)`.
- **Stretch:** `autocomplete(prefix)` returning all completions; deletion (subtle
  — when can you prune a node?).

## Key questions
- Why is every operation O(L) in the **word length**, *independent of how many
  words are stored*? When is that a win over a hash set's O(1)?
- Children as a fixed `[26]` array vs. a `HashMap<Character, TrieNode>`: what's
  the space/speed/Unicode trade-off?
- What's the difference between "this prefix exists" and "this whole word exists"?
- How does this generalize to routing tables and T9 predictive text?

## LeetCode (in order)
1. [Medium] 208. Implement Trie (Prefix Tree) — literally this project
2. [Medium] 211. Design Add and Search Words (wildcards → DFS over the trie)
3. [Medium] 648. Replace Words
4. [Hard] 212. Word Search II (trie + grid DFS — combines with Projects 11/12)

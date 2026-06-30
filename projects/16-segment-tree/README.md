# Project 16 — Segment Tree

> **Status:** lesson outline (full eight-section treatment follows the Project 1
> template). The most advanced core project — a fitting finale.

**Builds on:** Project 1 (array-backed, like the heap) and tree/recursion
intuition from Project 8.

## The core question
Given an array, you want to answer **range queries** — "sum of elements 3..9",
"minimum in 2..7" — *and* support **updates** to individual elements. A plain
array makes queries O(n) (scan the range). Precomputed prefix sums make queries
O(1) but updates O(n). Can we get **both** query *and* update in O(log n)?

## The idea
A **segment tree** is a binary tree where each node stores an aggregate (sum, min,
max, gcd...) over a *segment* of the array. The root covers the whole array; each
node splits its range in half between its two children; leaves are single
elements. A range query combines O(log n) nodes; an update touches O(log n) nodes.

```
array: [2, 1, 5, 3]              sum segment tree:
                                          [0..3]=11
                                         /         \
                                  [0..1]=3        [2..3]=8
                                   /    \           /    \
                              [0]=2  [1]=1     [2]=5   [3]=3

query sum(1..3): combine [1]=1 and [2..3]=8 → 9   (a few nodes, not all)
```

## What you'll implement
- An array-backed segment tree: `build`, `query(l, r)`, `update(i, value)`.
- Pick an aggregate (start with **sum**, then generalize to min/max).
- **Stretch:** **lazy propagation** for efficient *range* updates (a deep,
  rewarding topic); compare with the **Fenwick/BIT** tree on the bonus shelf.

## Key questions
- Why are both query and update O(log n)? (Each recurses down a tree of height
  log n, touching O(log n) nodes.)
- Why does a range query touch O(log n) nodes and not O(n)? (Whole-segment nodes
  short-circuit the recursion.)
- How much space does the backing array need, and why is `4n` a safe size?
- When is a Fenwick tree (simpler, less memory) enough, and when do you truly need
  a segment tree (arbitrary associative ops, lazy range updates)?

## LeetCode (in order)
1. [Medium] 303. Range Sum Query - Immutable (prefix sums — motivates the need)
2. [Medium] 307. Range Sum Query - Mutable (the canonical segment-tree problem)
3. [Hard] 315. Count of Smaller Numbers After Self
4. [Hard] 218. The Skyline Problem (advanced; segment-tree thinking)

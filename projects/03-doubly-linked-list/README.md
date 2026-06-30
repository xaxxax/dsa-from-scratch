# Project 3 — Doubly Linked List

> **Status:** lesson outline (full eight-section treatment follows the Project 1
> template). Implements the **same `List<E>` interface**.

**Builds on:** Project 2. This is the structure you'll later fuse with your
HashMap to build the LRU Cache (Project 15) — so build it well.

## The core question
A singly linked list can only walk forward, so removing a node requires knowing
its *predecessor*. What if you want **O(1) removal given just the node itself**,
and **O(1) operations at *both* ends**?

## The idea
Each node carries **two** pointers: `prev` and `next`. Add a `tail` pointer (and
often a pair of sentinel/dummy nodes) so both ends are symmetric and edge cases
mostly vanish.

```
head                                              tail
 │                                                  │
 ▼                                                  ▼
[/|A|·]⇄[·|B|·]⇄[·|C|·]⇄[·|D|/]      (each node: prev | value | next)
```

## What you'll implement
- `Node<E>` with `prev`, `next`, `value`.
- `addFirst`, `addLast`, `removeFirst`, `removeLast`, `remove(node)`.
- **Sentinel nodes** (dummy head & tail) — implement once *without* them, feel the
  null-checking pain, then *with* them, and appreciate why they're standard.

## Key questions
- Why does the second `prev` pointer enable O(1) `remove(node)`?
- What's the memory cost of the extra pointer per node? When is it worth it?
- How do sentinels eliminate "is this the first/last node?" special cases?
- Why is this the right backbone for an LRU cache's recency ordering?

## LeetCode (in order)
1. [Easy] 206. Reverse Linked List (re-do thinking in both directions)
2. [Medium] 146. LRU Cache (you'll fully build this in Project 15 — peek now)
3. [Medium] 430. Flatten a Multilevel Doubly Linked List
4. [Medium] 1472. Design Browser History (doubly linked list shines)

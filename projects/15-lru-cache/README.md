# Project 15 — LRU Cache (compose HashMap + Doubly Linked List)

> **Status:** lesson outline (full eight-section treatment follows the Project 1
> template). This is the **capstone of composition** — it fuses two structures
> you built from scratch.

**Builds on:** Project 6 (HashMap) and Project 3 (Doubly Linked List). You will
use *your own* implementations of both.

## The core question
A cache has limited space. When it's full and a new item arrives, **which item do
we evict?** A good policy: evict the **L**east **R**ecently **U**sed. The
challenge: make `get` *and* `put` both **O(1)**, *including* the bookkeeping that
tracks recency and the eviction. No single structure does it — but two together do.

## The idea
- A **HashMap** gives O(1) lookup from key → node.
- A **doubly linked list** maintains recency order: most-recently-used at the
  front, least at the back. Touching an item moves its node to the front in O(1)
  (which is *why* you needed the doubly-linked `remove(node)` from Project 3).
- On overflow, evict the node at the back and remove its key from the map.

```
map:  key ──▶ node                      recency (doubly linked):
                                  front [MRU] ⇄ ... ⇄ [LRU] back
get(k): map finds node → move node to front.
put(k,v): insert at front; if over capacity, drop the back node + its map entry.
```

## What you'll implement
- `LRUCache(int capacity)` with `get(key)` and `put(key, value)`, both O(1).
- Internally: your `HashMap<K, Node>` + your doubly linked list with sentinels
  (the sentinels from Project 3 pay off here — no null edge cases on eviction).

## Key questions
- *Why* does each operation need O(1)? Where would a single structure force O(n)?
- Why a **doubly** (not singly) linked list? (You must unlink an arbitrary node in
  O(1) — the `prev` pointer is essential.)
- Why store the *node* (not just the value) in the map?
- How do sentinel head/tail nodes eliminate eviction edge cases?
- How does this relate to real CPU caches, page replacement, and Redis/Memcached
  eviction policies (LRU, LFU, ARC)?

## LeetCode (in order)
1. [Medium] 146. LRU Cache — *exactly* this project
2. [Medium] 460. LFU Cache (a harder eviction policy; great stretch)
3. [Hard] 432. All O`one Data Structure (composition pushed further)

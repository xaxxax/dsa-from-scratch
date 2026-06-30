# Project 9 — Heap & Priority Queue

> **Status:** lesson outline (full eight-section treatment follows the Project 1
> template).

**Builds on:** Project 1 (a heap is *array-backed* — a beautiful surprise). Used
later for graph algorithms (Dijkstra) and many LeetCode "top-K" problems.

## The core question
A queue gives FIFO order. But what if we always want the **smallest (or largest)
element next**, regardless of insertion order — and we want *both* "insert" and
"remove-min" to be fast? Sorting on every insert is O(n log n) repeatedly; a
sorted list makes insert O(n). Can we do better?

## The idea
A **binary heap** is a *complete* binary tree (every level full except possibly
the last, filled left-to-right) with the **heap property**: every parent is ≤ (min-heap)
its children. Because it's complete, we store it in a **plain array** with simple
index arithmetic — no node objects, no pointers.

```
array:  [10, 15, 30, 40, 50, 60]      as a tree:        (10)
index:    0   1   2   3   4   5                         /    \
                                                     (15)    (30)
 parent(i) = (i-1)/2                                 /  \     /
 left(i)   = 2i+1                                 (40)(50) (60)
 right(i)  = 2i+2
```

## What you'll implement
- An array-backed `MinHeap<E extends Comparable<E>>`.
- `insert` → **sift-up**; `extractMin` → swap root with last, shrink, **sift-down**.
- `peek`, `size`; **stretch:** `heapify` an array in O(n) (not O(n log n) — why?),
  and a generic `PriorityQueue` taking a `Comparator`.

## Key questions
- Why does a *complete* tree let us use an array with no gaps?
- Derive the parent/child index formulas and prove they're correct.
- Why are insert and extract O(log n) (tree height) but peek O(1)?
- Why is bottom-up `heapify` O(n) while inserting n items one-by-one is
  O(n log n)? (A genuinely counterintuitive, beautiful result.)
- A heap is *not* a search structure — why is `contains` still O(n)?

## LeetCode (in order)
1. [Easy] 703. Kth Largest Element in a Stream
2. [Medium] 215. Kth Largest Element in an Array
3. [Medium] 347. Top K Frequent Elements (heap + your HashMap!)
4. [Medium] 973. K Closest Points to Origin
5. [Hard] 23. Merge k Sorted Lists (heap of list heads)

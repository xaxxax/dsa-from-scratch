# Project 9 — Heap / Priority Queue

> **You are building:** a **priority queue** — a structure that always hands you the
> smallest (or largest) item next — backed by a **binary heap** stored in a plain
> array. This is C++'s `priority_queue`, Java's `PriorityQueue`, Python's `heapq`.
>
> **Builds on:** Lesson 1 (the array grows exactly the same way) and the complete-
> binary-tree idea from Lesson 8 — but with a twist that removes the pointers.
>
> **The C leap:** a tree with **no nodes and no pointers.** A complete binary tree
> maps perfectly onto an array via index arithmetic (`parent = (i-1)/2`, `children
> = 2i+1, 2i+2`). After the pointer-heavy trees and lists, the heap is a palate
> cleanser — the "tree" is a mental model over a flat buffer.
>
> **Mentor rule:** diagrams and questions here, not code.

---

## 1. Motivation

### The problem

You repeatedly need "the most important thing next": the shortest pending job, the
nearest frontier node in Dijkstra, the top-k largest, the next event by timestamp.
A sorted array gives O(1) min but O(n) insert (shift to keep sorted). An unsorted
array gives O(1) insert but O(n) to find the min. Can we get **O(log n) insert AND
O(log n) remove-min, with O(1) peek-min**?

### The idea

Keep the data only *partially* ordered — just enough to know the minimum instantly.
A **min-heap** is a complete binary tree where **every parent ≤ its children**. The
root is therefore the global minimum (peek is O(1)). It is *not* fully sorted —
siblings have no order — which is exactly why maintaining it is cheap: a change only
has to be repaired along **one root-to-leaf path** (O(log n)), not across the whole
structure.

### The array trick

Because the tree is **complete** (filled left to right, no gaps), you can store it
in an array with no pointers at all:

```
parent(i) = (i-1)/2      left(i) = 2i+1      right(i) = 2i+2
```

Navigation is arithmetic. This is denser and far more cache-friendly than a
pointer tree — one reason heaps are so fast in practice.

---

## 2. Visualization

```
 min-heap:            2                array:  [ 2 | 4 | 7 | 9 | 5 | 8 ]
                    /   \               index:    0   1   2   3   4   5
                  4       7             parent(4@i1)=i0 ✓   children(i1)=i3,i4
                 / \     /              parent(9@i3)=i1 ✓   → 9,5 are 4's kids
                9   5   8

 NOTE: the array is NOT sorted. A heap only guarantees parent <= children.
```

### `push(1)` — sift UP

```
 append 1 at the next free leaf (index 6, a child of index 2=7):
        2                 1 < its parent 7 → swap;  1 < new parent 2 → swap;
      /   \               1 is now the root.
     4     7   →  ... →   the new min bubbled up along ONE path (O(log n)).
    /\    / \
   9  5  8   1
```

### `pop()` — remove min, sift DOWN

```
 min = root (2). Move the LAST leaf (8) to the root, shrink, then sink it:
        8            8 vs smaller child (4) → swap;
      /   \          8 vs smaller child (5) → swap; 8 now has no smaller child.
     4     7   →     Root is 4 (the new min). One root-to-leaf path again.
    /\
   9  5
```

**Draw push and pop on a 6-element heap, in BOTH the tree and array views.** Seeing
how a swap in the tree is a swap of two array slots is the whole lesson.

---

## 3. Design

```c
typedef struct {
    heap_elem *data;
    size_t     size;
    size_t     capacity;
} MinHeap;
```

It is literally "a dynamic array kept in heap order," so it grows the same way
(double + realloc, Lesson 1). All the tree logic is index arithmetic on `data`.

### Design questions

1. **Min or max?** Pick one (this scaffold is a min-heap). To flip to a max-heap you
   change *only the comparisons* in sift-up/down. Note that; it's a one-line
   conceptual switch, not a rewrite.
2. **The unsigned trap in `parent`.** `parent(0) = (0-1)/2` underflows `size_t` to a
   giant number. The root has no parent — your sift-up loop must **stop at `i == 0`**
   and never call `parent(0)`.
3. **Sift-down's "smaller child."** At each step compare against the *smaller* of the
   two children (for a min-heap), and mind that a node may have only a left child or
   none — guard `left(i) >= size` / `right(i) >= size`.
4. **Empty policy.** `peek`/`pop` on an empty heap — assert or status? Consistent
   with earlier lessons.
5. **Build-heap.** Inserting n items one-by-one is O(n log n). Floyd's *build-heap*
   (sift-down from the last internal node backward) heapifies an existing array in
   **O(n)** — a beautiful result and a great milestone (M6).

---

## 4. Milestones

Scaffold: [`src/heap.c`](src/heap.c) / [`src/heap.h`](src/heap.h).

```
[ ] M0  Build: header + compiling .c with TODO-asserts; make test aborts at heap_new.
        Several "defined but not used" warnings (parent_of/right_of/swap/heap_grow)
        are EXPECTED — they're static helpers nothing calls yet; they vanish as you
        implement push/pop.
[ ] M1  heap_new / heap_free (free data, then handle). new-then-free: ZERO leaks.
[ ] M2  heap_grow + heap_push with SIFT UP. Test that peek is the min after pushes.
[ ] M3  heap_peek — data[0], O(1). Guard empty first.
[ ] M4  heap_pop with SIFT DOWN — move last leaf to root, shrink, sink. The key
        test: repeated pops come out sorted ascending (that's heapsort).
[ ] M5  heap_size / heap_is_empty.
[ ] M6  (Stretch) heap_build(array, n) in O(n) (Floyd); heapsort; a max-heap flag or
        a comparator; decrease_key (needs positions — used by Dijkstra).
[ ] M7  Full suite green under ASan; valgrind clean. Request review.
```

---

## 5. Testing

```bash
cd projects/09-heap-priority-queue
make test
make && valgrind --leak-check=full --error-exitcode=1 ./out_test
```

Starter: [`tests/test_heap.c`](tests/test_heap.c).

### Edge cases (test all)

- **The defining test:** push a jumble, pop everything — output is ascending. If not,
  your sift is wrong.
- **Duplicates:** equal keys must be handled (heap allows them).
- **Grow boundary:** push well past initial capacity; the min stays correct.
- **Down to empty, then push again** — the structure recovers.
- **Single element:** push one, peek/pop it.
- **The unsigned trap:** never call `parent(0)`; sift-up stops at the root.
- **`heap_free` on a fresh heap** — zero leaks.

---

## 6. Complexity Analysis

Derive first.

| Operation | Time | Why |
|-----------|:----:|-----|
| `heap_peek` | ? | ? |
| `heap_push` | ? | ? |
| `heap_pop` | ? | ? |
| build-heap (n items) | ? | ? |

<details>
<summary>Answers.</summary>

- `peek`: **O(1)** — the min is always `data[0]`.
- `push`: **O(log n)** — sift up traverses at most the tree height; amortized with
  the occasional O(n) grow, still O(log n).
- `pop`: **O(log n)** — sift down traverses at most the height.
- **Not** a search structure: finding an arbitrary element is O(n) — a heap only
  makes the *extremum* cheap.
- Build-heap one-by-one: O(n log n). Floyd's bottom-up build: **O(n)** — the
  surprising result (most nodes are near the bottom and sift down very little).
- Space: **O(n)** (plus the array's slack). Heapsort uses the heap to sort in-place
  in O(n log n).
</details>

---

## 7. Real-World Usage

- **Dijkstra / A\* / Prim's MST:** a priority queue of frontier nodes by distance —
  the heap is what makes these efficient (you'll want `decrease_key`).
- **Schedulers & event simulation:** next task/event by priority or timestamp.
- **Top-k / streaming:** keep a size-k heap to track the k largest of a huge stream
  in O(n log k).
- **Heapsort:** an in-place O(n log n) sort built directly on this structure.
- **`priority_queue` (C++), `PriorityQueue` (Java), `heapq` (Python), Go
  `container/heap`** — all binary heaps in arrays, exactly like this.

---

## 8. LeetCode

**No solutions here.**

1. **[Easy] 703. Kth Largest Element in a Stream** — a running size-k heap.
2. **[Medium] 215. Kth Largest Element in an Array** — heap (or quickselect).
3. **[Medium] 347. Top K Frequent Elements** — heap keyed by frequency (uses your
   Lesson 6 map to count first).
4. **[Medium] 973. K Closest Points to Origin** — heap by distance.
5. **[Hard] 23. Merge k Sorted Lists** — a heap of list heads; the classic heap win.

---

## Exercises

1. **`heap_build(const int *a, size_t n)`** in O(n) (Floyd) — then compare timing to
   n pushes.
2. **Heapsort** — build a heap, pop n times into an output array; prove it's sorted.
3. **Max-heap variant** — flip the comparisons; verify pops descend.

## Challenge problems

1. **`decrease_key(i, newval)`** — lower an element's key and sift up. Requires
   tracking each element's current index (a map from element → position). This is
   the primitive Dijkstra needs; it's why library heaps expose handles.
2. **k-ary heap** — children at `k*i+1 .. k*i+k`. Shallower tree (faster push),
   costlier pop (k children to compare). When does k=4 beat k=2? (Cache effects.)
3. **Median of a stream** — two heaps (a max-heap of the low half, a min-heap of the
   high half) balanced in size. O(log n) insert, O(1) median.

## Stretch goals

- **Generic comparator** — take a `int (*less)(heap_elem, heap_elem)` so the same
  heap serves min, max, or custom priority (like `qsort`).
- **Pairing / Fibonacci heap** (advanced) — better amortized `decrease_key`; read
  about why theory prefers them and practice often doesn't (constants, cache).

## Common mistakes

- **Calling `parent(0)`** — underflows `size_t`; sift-up must stop at index 0.
- **Sift-down picking the wrong child** — for a min-heap you descend into the
  *smaller* child; comparing against the left child only is a classic bug.
- **Forgetting a node may have one or zero children** — guard `left(i) >= size` and
  `right(i) >= size` before indexing.
- **Treating the array as sorted** — it isn't; only the root is guaranteed the min.
- **Shrinking before saving the root**, or reading `data[size]` after decrementing.

## Debugging tips

- Print the array after every push/pop and mentally overlay the tree (index i's
  children are 2i+1, 2i+2). Most bugs are visible in one print.
- Verify the heap property directly: a helper that checks `data[i] <= data[2i+1]`
  and `data[i] <= data[2i+2]` for all valid i catches a broken sift immediately.
- Reproduce with a tiny heap (capacity 1–2) so a single grow + a single sift expose
  the bug.

## Frequently asked questions

**Q: Why an array and not a node tree like the BST?**
A: A heap is a *complete* tree (no gaps), which maps 1-to-1 onto array indices — so
you get the tree for free with zero pointer overhead and perfect cache locality. A
BST can be any shape, so it needs real nodes. The heap's rigidity is exactly what
lets it be an array.

**Q: If a heap isn't sorted, how does heapsort work?**
A: Repeatedly pop the min (O(log n) each) into the output; n pops give sorted order
in O(n log n). Done in-place (pop swaps the min to the end of the same array), it's
a classic O(n log n), O(1)-extra-space sort.

**Q: Why does Dijkstra want `decrease_key` specifically?**
A: When a shorter path to a frontier node is found, its priority drops; `decrease_key`
updates it in O(log n) instead of inserting a duplicate. Without it you can still do
"lazy deletion" (push duplicates, skip stale pops) — a common practical shortcut.

---

## Definition of done

- [ ] Milestones M1–M5 implemented (M6–M7 recommended; build-heap + heapsort are
      excellent).
- [ ] Full suite passes, especially "pops come out ascending" and the grow boundary.
- [ ] **`make test` clean under ASan/UBSan; `valgrind` zero leaks.**
- [ ] §6 table filled from your reasoning; you can explain why peek is O(1), push/pop
      O(log n), and why build-heap is O(n).
- [ ] `PROGRESS.md` written. Code review requested.

→ Next: [`../10-trie/`](../10-trie/) — a very different tree, keyed by the characters
of a string, giving prefix search in O(length) independent of how many words you've
stored.

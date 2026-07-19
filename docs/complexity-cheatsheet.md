# Complexity Cheat Sheet

> **Read this *after* you can derive these yourself.** It's a reference to check
> your reasoning against — not a thing to memorize. If a cell surprises you, go
> back to that structure and figure out *why* before you trust the number.

---

## How to read Big-O (the 60-second version)

Big-O describes how the cost of an operation **grows as the input grows**, in
the worst case, ignoring constants. `O(1)` = doesn't grow with `n`. `O(n)` =
grows in proportion to `n`. `O(log n)` = grows very slowly (doubling `n` adds
one step). `O(n²)` = grows brutally.

Two words you must be precise about:

- **Worst case** — the most expensive a single operation can ever be.
- **Amortized** — the *average* cost per operation across a long sequence, even
  if a few individual operations are expensive. (Dynamic-array `add` is the
  classic: usually O(1), occasionally O(n) to resize, but **amortized O(1)**.)

These are different from **average case** (expected cost under random input).
Don't conflate amortized (a guarantee over a sequence) with average (a
probability over inputs).

---

## The structures in this course

Legend: ✅ = the operation this structure is *good* at. n = number of elements.

| Structure | Access | Search | Insert | Delete | Notes |
|-----------|:------:|:------:|:------:|:------:|-------|
| **Dynamic Array** | O(1) ✅ | O(n) | O(1) amortized at end / O(n) middle | O(n) | Resizing is the amortized story |
| **Singly Linked List** | O(n) | O(n) | O(1) at head ✅ / O(n) elsewhere | O(1) at head / O(n) elsewhere | No random access |
| **Doubly Linked List** | O(n) | O(n) | O(1) at both ends ✅ | O(1) at both ends ✅ / O(1) given the node | Backward links cost memory |
| **Stack** (on array) | — | — | O(1) push ✅ | O(1) pop ✅ | LIFO only |
| **Queue** (on linked list) | — | — | O(1) enqueue ✅ | O(1) dequeue ✅ | FIFO only |
| **HashMap** | — | O(1) avg ✅ / O(n) worst | O(1) avg ✅ / O(n) worst | O(1) avg ✅ / O(n) worst | Worst case if everything collides |
| **HashSet** | — | O(1) avg ✅ | O(1) avg ✅ | O(1) avg ✅ | Same costs as its backing map |
| **Binary Search Tree** (unbalanced) | — | O(log n) avg / O(n) worst | O(log n) avg / O(n) worst | O(log n) avg / O(n) worst | Degenerates to a list if sorted input |
| **Balanced BST** (AVL/RB) | — | O(log n) ✅ | O(log n) ✅ | O(log n) ✅ | Balancing guarantees the log |
| **Binary Heap** | O(1) peek-min/max ✅ | O(n) | O(log n) ✅ | O(log n) extract ✅ | Not for searching arbitrary elements |
| **Trie** | — | O(L) ✅ | O(L) ✅ | O(L) | L = key length, *independent of n* |
| **Graph** (adj. list) | — | — | O(1) add edge | O(degree) | Space O(V + E) |
| **Graph** (adj. matrix) | O(1) edge lookup ✅ | — | O(1) | O(1) | Space O(V²) |
| **DFS / BFS** | — | — | — | — | O(V + E) time, O(V) space |
| **Union-Find** (path compression + rank) | — | — | — | — | ~O(α(n)) per op — effectively constant |
| **LRU Cache** | O(1) get ✅ | O(1) ✅ | O(1) put ✅ | O(1) evict ✅ | HashMap + doubly linked list |
| **Segment Tree** | — | O(log n) range query ✅ | O(log n) point update ✅ | — | Space O(n) |

---

## Space complexity reminders

- An array of `n` elements is `O(n)` space — but a *dynamic* array may hold up to
  ~2× that in spare capacity. That slack is the price of amortized O(1) appends.
- A linked list of `n` nodes is `O(n)`, but with a **larger constant**: every
  element also stores one (singly) or two (doubly) pointers plus per-node
  allocator overhead (each `malloc` block carries bookkeeping). And the nodes are
  scattered across the heap, so pointer-chasing stalls the CPU cache — this is why
  arrays are often faster *and* smaller in practice, even at the same Big-O.
- Recursion costs stack space: a recursive traversal of a tree of height `h`
  uses `O(h)` stack frames. For a balanced tree that's `O(log n)`; for a
  degenerate one it's `O(n)` — and can blow the call stack.

---

## The growth-rate ladder (memorize the *order*, derive the rest)

```
O(1) < O(log n) < O(n) < O(n log n) < O(n²) < O(2ⁿ) < O(n!)
 ↑                                              ↑
 ideal                                  avoid for large n
```

A useful gut check at n = 1,000,000:
- `O(log n)` ≈ 20 steps
- `O(n)` ≈ 1,000,000 steps
- `O(n log n)` ≈ 20,000,000 steps
- `O(n²)` ≈ 1,000,000,000,000 steps (a trillion — not happening)

This is *why* we care: the difference between O(n) and O(n²) is the difference
between "instant" and "never finishes."

---

## What `α(n)` means (Union-Find)

`α` is the **inverse Ackermann function**. It grows so absurdly slowly that for
any input you could physically store, `α(n) ≤ 5`. So we treat path-compressed,
union-by-rank Union-Find as *effectively constant time per operation* — but the
honest bound is "nearly constant," not "constant." Knowing the difference is the
mark of someone who actually understands it.

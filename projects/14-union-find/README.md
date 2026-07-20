# Project 14 — Union-Find (Disjoint Set Union)

> **You are building:** the structure that answers "are these two things connected?"
> and "merge these two groups" in **near-constant time** — the engine behind
> Kruskal's minimum spanning tree, connected-components, and cycle detection in
> undirected graphs.
>
> **Builds on:** just an array and a clear head. It's the most *code-light*
> structure in the course — two `int` arrays and a few lines per operation — but the
> reasoning about *why* it's near-O(1) is some of the deepest.
>
> **The C leap:** almost none, and that's the lesson. No pointers, no per-node
> `malloc` — the entire forest lives in a flat `size_t parent[]`. After the pointer
> gymnastics of trees and graphs, Union-Find shows that a brilliant data structure
> can be *array indices and nothing else*.
>
> **Mentor rule:** diagrams and questions here, not code.

---

## 1. Motivation

### The problem

You're given a stream of "a and b are in the same group" facts and must answer, at
any point, "are x and y in the same group?" Think: pixels forming a blob, cities
joined by roads, accounts merged by a dedupe pass, nodes in a network partition. You
need two operations, both fast:

- **union(x, y)** — declare x and y connected (merge their groups).
- **connected(x, y)** — are x and y in the same group?

A naive approach (a list per group, scanned and merged) degrades to O(n) per
operation. Union-Find gets both to *effectively* O(1).

### The idea

Represent each group as a **tree**, and identify the group by its **root**. Store
the forest in a single array: `parent[i]` is the node i points at; a **root** points
at itself (`parent[r] == r`). Then:

- `find(x)` follows parents up to the root — the group's name.
- `connected(x, y)` is just `find(x) == find(y)`.
- `union(x, y)` points one root at the other.

### Keeping it near-O(1): two optimizations

Done naively, a bad sequence of unions builds an n-deep chain and `find` becomes
O(n) — a linked list wearing an array's clothes. Two cheap tricks prevent that:

1. **Union by rank/size** — always attach the *shorter* tree under the taller one,
   so trees never get needlessly deep.
2. **Path compression** — during `find(x)`, re-point every node on the path straight
   at the root, flattening the tree for next time.

Together they give an amortized cost of **α(n)** (the inverse Ackermann function),
which is `< 5` for any `n` you could physically store — indistinguishable from
constant.

---

## 2. Visualization

```
 elements:   0   1   2   3   4
 parent[]:   0   0   1   3   3      (parent[i] == i  ⇒  i is a root)

 as a forest:      (0)          (3)
                    │           /
                   (1)        (4)
                    │
                   (2)

 set {0,1,2} has root 0.   set {3,4} has root 3.
```

### `find(2)` with path compression

```
 walk: 2 → parent 1 → parent 0 → parent 0 (root!)   returns 0
 compress: re-point 2 (and 1) directly at 0:

 before:  (0)          after:   (0)
           │                    /   \
          (1)                 (1)   (2)      ← both now one hop from the root
           │
          (2)
```

### `union(2, 4)` by rank

```
 find(2) = 0 (rank 2),  find(4) = 3 (rank 1).
 attach the SHORTER tree (root 3) under the taller (root 0):
 parent[3] = 0.   count drops 2 → 1.   Everything is now one set.
```

**Draw a find-with-compression and a union-by-rank on paper.** Once you see the
tree flatten, the α(n) magic stops being mysterious.

---

## 3. Design

```c
typedef struct {
    size_t *parent;   /* parent[i] = i's parent; parent[i]==i iff i is a root */
    size_t *rank;     /* upper bound on the tree height rooted at i           */
    size_t  n;        /* universe size: valid elements are 0..n-1             */
    size_t  count;    /* number of disjoint sets right now                    */
} UnionFind;
```

### Design questions

1. **Why does `find` take a *non-const* `UnionFind*`?** Because path compression
   *mutates* the array during a read. A `find` that flattens the tree is why the
   *next* `find` is fast. Sit with the idea of a read that writes.
2. **Why cache `count`?** So `uf_count` is O(1). Maintain it in `union` (decrement
   only on a real merge). Counting roots by scanning would be O(n).
3. **Union by rank vs. by size.** Rank ≈ tree height; size = element count. Both
   keep trees shallow; both work. Pick one and be consistent. (With path
   compression, `rank` becomes an *upper bound*, not exact — that's fine.)
4. **What is element 0..n-1?** Union-Find is defined over a fixed universe chosen at
   construction. Real keys (strings, coordinates) get mapped to `0..n-1` first —
   a job for your Lesson 6 hash map — then run this underneath.
5. **The unsigned trap** returns: bounds-check `x < n` in `find`; a "negative" index
   arrives as a huge `size_t`.

---

## 4. Milestones

Scaffold: [`src/union_find.c`](src/union_find.c) / [`src/union_find.h`](src/union_find.h).

```
[ ] M0  Build: header + compiling .c with TODO-asserts; make test aborts at uf_new.
[ ] M1  uf_new (n singletons: parent[i]=i, rank[i]=0, count=n) / uf_free (free the
        arrays, then the handle). new-then-free: ZERO leaks under ASan.
[ ] M2  uf_find — FIRST version, NO compression: bounds-check, then walk parent[]
        to the root. Simple and correct before it's fast.
[ ] M3  uf_union — by rank: find both roots; if equal return 0; else attach shorter
        under taller, decrement count, return 1.
[ ] M4  Add PATH COMPRESSION to uf_find (two-pass or path-halving). Now finds
        flatten the forest as a side effect.
[ ] M5  uf_connected (find(x)==find(y)) and uf_count (return the cached counter).
[ ] M6  (Stretch) Union by SIZE with an exposed set_size(x); cycle detection on an
        edge list; connected-components count; Kruskal's MST if you have weights.
[ ] M7  Full suite green under ASan; valgrind clean. Request review.
```

---

## 5. Testing

```bash
cd projects/14-union-find
make test
make && valgrind --leak-check=full --error-exitcode=1 ./out_test
```

Starter: [`tests/test_union_find.c`](tests/test_union_find.c).

### Edge cases (test all)

- **Reflexive/transitive:** after `union(0,1)` and `union(1,2)`, `connected(0,2)` is
  true (transitivity through the shared root).
- **Redundant union:** `union(0,1)` when already connected returns 0 and does *not*
  change `count`.
- **`count` accuracy:** start n; each real merge drops it by exactly 1; unions
  within a set don't.
- **Every element with itself:** `connected(x, x)` is always true.
- **The unsigned trap:** `find(uf, (size_t)-1)` must be rejected by `x < n`.
- **Scale + compression:** union a long chain `0-1, 1-2, …, 999-1000` then do
  repeated finds — with compression the trees flatten (hard to see directly; assert
  correctness and trust the amortized bound). `uf_free` on a fresh structure.

---

## 6. Complexity Analysis

Derive first.

| Operation | Naive | + union by rank | + path compression too |
|-----------|:-----:|:---------------:|:----------------------:|
| `find` | ? | ? | ? |
| `union` | ? | ? | ? |
| `connected` | ? | ? | ? |
| `count` | ? | ? | ? |

<details>
<summary>Answers.</summary>

- Naive `find`/`union`: **O(n)** worst (a degenerate chain).
- Union by rank alone: **O(log n)** — trees stay balanced-ish.
- Union by rank **+** path compression: **O(α(n)) amortized** — inverse Ackermann,
  `< 5` for all practical n, i.e. *effectively constant*. This is one of the most
  beautiful results in algorithms: two trivial tweaks collapse O(n) to ~O(1).
- `connected`: same cost as `find` (it calls it twice).
- `count`: **O(1)** — cached, maintained by `union`.
- Space: **O(n)** — two arrays.
</details>

---

## 7. Real-World Usage

- **Kruskal's MST:** sort edges, add each if its endpoints aren't already connected
  (Union-Find is the "already connected?" oracle) — this is the classic use.
- **Connected components / flood fill:** group pixels, network nodes, friend
  clusters; `count` gives the number of components instantly.
- **Cycle detection in undirected graphs:** an edge whose endpoints are already in
  the same set closes a cycle — exactly `uf_union` returning 0.
- **Dynamic connectivity, percolation, image segmentation, compiler equivalence
  (type unification), account/entity dedupe.**

---

## 8. LeetCode

**No solutions here.**

1. **[Medium] 547. Number of Provinces** — connected components, textbook DSU.
2. **[Medium] 200. Number of Islands** — DSU (or DFS/BFS) on a grid.
3. **[Medium] 684. Redundant Connection** — the edge that forms a cycle = union
   returning 0.
4. **[Medium] 1319. Number of Operations to Make Network Connected** — components
   and spare edges.
5. **[Hard] 305. Number of Islands II** — online DSU as cells are added.

---

## Exercises

1. **`uf_set_size(x)`** — track and expose the size of x's set (switch to union by
   size, or keep a parallel `size[]` updated on merges).
2. **Connected-components report** — given `n` and an edge list, return the number
   of components (just `uf_count` after unioning every edge).
3. **Cycle detector** — given an undirected edge list, return whether it contains a
   cycle, using only `uf_union`'s return value.

## Challenge problems

1. **Kruskal's MST.** Given weighted edges, sort them ascending and add each unless
   it forms a cycle. Union-Find makes the cycle test near-O(1). Prove the result is
   a minimum spanning tree.
2. **Path halving vs. two-pass compression.** Implement both; reason about why
   halving is one-pass and nearly as flat. (Benchmarking the difference is subtle —
   both are α(n).)
3. **Union by size + weighted quick-union without compression** — implement and
   confirm it's O(log n); then add compression and feel the drop.

## Stretch goals

- **Rollback / persistent DSU** (no path compression, union by rank with an undo
  stack) — supports "undo the last union," needed in offline dynamic-connectivity
  and some competitive problems.
- **Map arbitrary keys** through your Lesson 6 hash map (string → 0..n-1) and run
  DSU underneath — a real end-to-end connected-components tool.

## Common mistakes

- **Forgetting union by rank** — always hanging x under y builds an O(n) chain;
  finds crawl. Attach shorter under taller.
- **`find` as `const`** — path compression mutates; the signature must be non-const.
  If you make it const you can't compress.
- **`count` drift** — decrement only when `union` actually merges two *different*
  sets (return 1), never on a redundant union.
- **Comparing elements instead of roots** in `connected` — you must compare
  `find(x)` and `find(y)`, not `x` and `y`.
- **Off-by-one / unsigned index** — bounds-check `x < n`.

## Debugging tips

- Print `parent[]` and `rank[]` as arrays after each op; the whole state is two
  short rows — the easiest structure in the course to eyeball.
- To check the forest is valid, verify every `find(i)` terminates (no infinite loop
  = no cycle except roots).
- Test correctness with compression OFF first (M2/M3), then turn it on (M4) and
  confirm answers are unchanged — compression must never change *what* set an
  element is in, only the tree shape.

## Frequently asked questions

**Q: How can two one-line tricks turn O(n) into effectively O(1)?**
A: Union by rank bounds tree height at O(log n); path compression then flattens the
paths you actually touch, so repeated operations amortize to α(n). The amortized
analysis (Tarjan's) is genuinely deep — but the *code* is trivial, which is the
delightful part. You don't need to reprove α(n); you need to trust it and know why
each tweak helps.

**Q: Can I delete an element or split a set?**
A: Not with plain Union-Find — it only *merges*. Splitting/deletion needs a
different structure (e.g., Euler-tour trees or link-cut trees). This one-way nature
is the price of its speed; recognizing that limit is part of understanding it.

**Q: rank vs. size — does it matter which?**
A: Both keep trees shallow and give the same asymptotics. Size is sometimes handier
because you often *want* the set size anyway. With path compression, rank stops being
the exact height (it's an upper bound) — which is fine; it's still a good merge
heuristic.

---

## Definition of done

- [ ] Milestones M1–M5 implemented (M6–M7 recommended; Kruskal is superb practice).
- [ ] Full suite passes, every §5 edge case (redundant union, count accuracy, the
      unsigned trap).
- [ ] **`make test` clean under ASan/UBSan; `valgrind` zero leaks.**
- [ ] §6 table filled from your reasoning; you can explain why rank + compression
      give α(n) and what each tweak does on its own.
- [ ] `PROGRESS.md` written. Code review requested.

→ Next: [`../15-lru-cache/`](../15-lru-cache/) — the capstone that combines two of
your structures (a hash map for O(1) lookup and a doubly linked list for O(1)
recency ordering) into one cache that evicts the least-recently-used entry.

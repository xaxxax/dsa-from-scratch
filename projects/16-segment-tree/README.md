# Project 16 — Segment Tree

> **You are building:** a structure that answers **range queries** (sum/min/max over
> `[l, r]`) *and* **point updates** both in **O(log n)** — the tool competitive
> programmers reach for constantly, and the idea behind range-aggregate indexes.
>
> **Builds on:** the array-as-tree trick from Lesson 9 (heap) and the recursive
> divide-and-conquer thinking from Lesson 8 (BST). The final core lesson.
>
> **The C leap:** another pointerless tree in a flat array — but *not* a complete
> one, so you allocate `~4n` slots and navigate by index arithmetic. The recursion
> carries an explicit "this node covers range `[lo, hi]`," a pattern that recurs
> across advanced algorithms.
>
> **Mentor rule:** diagrams and questions here, not code.

---

## 1. Motivation

### The problem

You have an array and must answer many **"sum of `[l, r]`"** queries *while values
keep changing*. Two obvious tools each fail half the job:

- A **prefix-sum array**: O(1) range sum, but any update forces rebuilding the sums
  behind it — **O(n) per update**.
- The **raw array**: O(1) update, but each range sum loops the range — **O(n) per
  query**.

With many interleaved updates and queries, both are too slow. You want *both*
operations sub-linear.

### The idea

Build a binary tree where **each node stores the aggregate (sum) of a contiguous
range**. The root covers the whole array `[0, n-1]`; each node splits its range in
half between two children; leaves are single elements. Then:

- A **query** for `[l, r]` combines only the O(log n) nodes whose ranges exactly
  tile `[l, r]` — never the whole array.
- An **update** changes one leaf and refixes only the O(log n) ancestors on its
  root-to-leaf path.

Both O(log n). The "combine" operation is **sum** here, but swap it for min/max/gcd
and the identical structure answers a different range question — that generality is
part of why segment trees are beloved.

---

## 2. Visualization

```
 values [5, 2, 7, 1]:

                       [0..3] = 15
                      /            \
                 [0..1] = 7      [2..3] = 8
                 /      \         /      \
              [0]=5   [1]=2    [2]=7   [3]=1     ← leaves = single elements

 query(1, 2):                            update(2, 10):
   [0..1] partial → recurse               leaf [2] = 10
     [0] no overlap → 0                    [2..3] = 10 + 1 = 11
     [1] full → 2                          [0..3] = 7 + 11 = 18
   [2..3] partial → recurse               (only the path 2 → [2..3] → root changed)
     [2] full → 7
     [3] no overlap → 0
   combine → 2 + 7 = 9
```

Each node index in the flat array: root = 1, and node `k`'s children are `2k` and
`2k+1` (like the heap). Because the tree isn't *complete*, indices can reach ~4n —
**that's why you allocate `4n` slots.** **Draw the tree for a 4- or 8-element array
and trace one query + one update**, labeling each node's `[lo,hi]`.

---

## 3. Design

```c
typedef struct {
    long   *tree;   /* ~4n node aggregates */
    size_t  n;      /* number of leaves = original array length */
} SegmentTree;
```

Every recursive helper carries `(node, lo, hi)`: the array index of the current node
and the range it covers. Children split at `mid = (lo+hi)/2`: left `2*node` covers
`[lo, mid]`, right `2*node+1` covers `[mid+1, hi]`.

### Design questions

1. **Why `4n` and not `2n`?** The tree is complete only when `n` is a power of two;
   otherwise the recursion can reach node indices up to just under `4n`. Allocating
   `4n` is the simple safe bound. (An exact bound is `2 * 2^ceil(log2 n)`; `4n` is
   the no-thinking-required version.) Convince yourself with a non-power-of-two `n`.
2. **The three query cases.** At each node: **no overlap** → contribute the identity
   (0 for sum); **full overlap** (node's range ⊆ query) → return the stored
   aggregate; **partial** → recurse both children and combine. Getting these three
   exactly right is the whole query.
3. **The identity element.** For sum it's 0; for min it's +∞, for max −∞, for gcd 0.
   The identity is what a "no overlap" branch returns so it doesn't affect the
   combine. Note it changes with the operation.
4. **`combine` is the only operation-specific code.** Sum today; changing `combine`
   (and the identity) to `min` turns this into a range-minimum tree with zero other
   edits. Keep it a single function so that's true.
5. **Point vs. range update.** This lesson does *point* updates (one index). *Range*
   updates ("add 3 to all of `[l,r]`") in O(log n) need **lazy propagation** — a
   deferred-update flag per node — which is the headline stretch (M5).

---

## 4. Milestones

Scaffold: [`src/segment_tree.c`](src/segment_tree.c) / [`src/segment_tree.h`](src/segment_tree.h).

```
[ ] M0  Build: header + compiling .c with TODO-asserts; make test aborts at seg_new.
        "unused" warnings for combine/build/query/update are expected until wired in.
[ ] M1  seg_new (malloc ~4n, recursive build in O(n)) / seg_free. new-then-free:
        ZERO leaks under ASan.
[ ] M2  seg_query — the three-case recursion. Test many ranges against hand sums.
[ ] M3  seg_update — walk to the leaf, set it, recombine ancestors. Re-query to
        confirm the change propagated.
[ ] M4  seg_size. Then a brute-force cross-check test (below).
[ ] M5  (Stretch) LAZY PROPAGATION for O(log n) RANGE updates; swap combine to
        min/max/gcd; an iterative (bottom-up) segment tree; persistent segment tree.
[ ] M6  Full suite green under ASan; valgrind clean. Request review.
```

---

## 5. Testing

```bash
cd projects/16-segment-tree
make test
make && valgrind --leak-check=full --error-exitcode=1 ./out_test
```

Starter: [`tests/test_segment_tree.c`](tests/test_segment_tree.c).

### Edge cases (test all)

- **Whole-array query** equals the total; **single-element query** equals that
  element.
- **Query after update** reflects the new value on every range that contains the
  index (and no change on ranges that don't).
- **Split equivalence:** `query(l, r)` == `query(l, m) + query(m+1, r)` for any split
  m — a great invariant.
- **n = 1** (a one-node tree); **update the same index twice.**
- **Brute-force fuzz:** keep a plain shadow array; for random updates and queries,
  assert the tree matches a simple loop sum. This catches subtle range bugs fast.
- **`seg_free` on a fresh tree** — zero leaks.

---

## 6. Complexity Analysis

Derive first.

| Operation | Time | Space |
|-----------|:----:|:-----:|
| build (`seg_new`) | ? | ? |
| `seg_query` | ? | — |
| `seg_update` | ? | — |

<details>
<summary>Answers.</summary>

- Build: **O(n)** — each of the ~2n nodes is computed once from its children.
- Query: **O(log n)** — the recursion visits O(log n) nodes because at each level at
  most a constant number of ranges are "partial"; the rest are full-overlap (return
  immediately) or no-overlap (prune).
- Update: **O(log n)** — one root-to-leaf path plus recombining its ancestors.
- Space: **O(n)** — the `~4n` array (still linear).
- Versus prefix sums (O(1) query / O(n) update) and raw array (O(n) query / O(1)
  update), the segment tree is the balanced choice when both operations are frequent.
</details>

---

## 7. Real-World Usage

- **Competitive programming** — the default for range-sum/min/max with updates; also
  range GCD, range XOR, count-of-inversions, and more via different `combine`s.
- **Databases & analytics** — range-aggregate queries; interval and window
  aggregates.
- **Computational geometry** — sweep-line algorithms use segment trees over
  coordinates (their namesake use).
- **Graphics & scheduling** — interval stabbing, resource-over-time aggregates.
- **Fenwick tree (BIT)** — a lighter cousin for prefix sums; worth comparing (less
  memory, only prefix/point ops without the full flexibility).

---

## 8. LeetCode

**No solutions here.**

1. **[Medium] 307. Range Sum Query - Mutable** — literally this project.
2. **[Hard] 315. Count of Smaller Numbers After Self** — segment tree / BIT.
3. **[Hard] 218. The Skyline Problem** — segment-tree / sweep-line thinking.
4. **[Medium] 303. Range Sum Query - Immutable** — the prefix-sum baseline (see why
   updates motivate the tree).
5. **[Hard] 699. Falling Squares** — range max with updates (lazy propagation).

---

## Exercises

1. **Range minimum** — change `combine` to `min` and the identity to a large value;
   verify with hand queries. Same structure, new question.
2. **`seg_query` split check** — assert `query(l,r) == query(l,m)+query(m+1,r)` in a
   test; a quick correctness oracle.
3. **Rebuild helper** — `seg_assign(values, n)` that rebuilds in O(n) (useful when
   many values change at once — cheaper than n point updates).

## Challenge problems

1. **Lazy propagation.** Support **range updates** ("add v to all of `[l,r]`") in
   O(log n) by storing a pending "add" on internal nodes and pushing it down only
   when needed. The trickiest, most rewarding segment-tree skill.
2. **Iterative segment tree.** The bottom-up array version — shorter, faster, no
   recursion. Derive the index math (leaves at `n..2n-1`).
3. **Persistent segment tree.** Each update creates O(log n) new nodes and returns a
   new root, so old versions remain queryable. Enables "query the array as it was at
   time t."

## Stretch goals

- **Fenwick tree (BIT).** Implement the lighter prefix-sum structure and compare
  memory/speed/flexibility with the segment tree.
- **2D segment tree** — range queries over a grid (segment tree of segment trees).

## Common mistakes

- **Allocating `2n` instead of `4n`** — for non-power-of-two `n` the recursion writes
  past the array (ASan will flag the overflow). Use `4n`.
- **Botching the three overlap cases** — especially returning the wrong identity for
  "no overlap," or treating "partial" as "full." Test with ranges that straddle
  `mid`.
- **Off-by-one in ranges** — decide inclusive `[l, r]` and be consistent everywhere;
  `mid = (lo+hi)/2`, right child starts at `mid+1`.
- **Forgetting to recombine after update** — the leaf changes but ancestors keep
  stale sums. Recompute each ancestor on the way back up.
- **The size_t unsigned trap** — `mid+1 > hi` or `r < lo` comparisons with unsigned
  indices; reason carefully so nothing underflows.

## Debugging tips

- Cross-check EVERYTHING against a brute-force loop sum on a shadow array — for a
  segment tree this fuzz test finds bugs almost instantly.
- Print the `tree[]` array and, for small n, hand-map indices to `[lo,hi]` ranges.
- Test on a power-of-two n first (clean complete tree), then a non-power-of-two to
  shake out the 4n/boundary bugs.
- Reproduce with n = 1 and n = 2 — the recursion base cases surface immediately.

## Frequently asked questions

**Q: Segment tree vs. prefix sums vs. Fenwick tree — which when?**
A: Prefix sums: unbeatable for static arrays (O(1) query, no updates). Fenwick/BIT:
prefix sums *with* point updates, less memory, simpler — but limited to invertible
aggregates and prefix queries. Segment tree: the most flexible — arbitrary range
queries, any associative `combine`, and (with lazy) range updates — at a bit more
memory and code. Pick the least powerful tool that covers your operations.

**Q: Why `4n` slots?**
A: The recursive tree is a full binary tree over `n` leaves but not necessarily
*complete*, so the largest node index used can approach `4n` for non-power-of-two
`n`. `4n` is a safe over-allocation; the tight bound is `2·2^⌈log₂ n⌉`. Under-
allocating causes out-of-bounds writes.

**Q: What does "lazy propagation" buy me?**
A: O(log n) *range* updates (e.g., "add 5 to all of `[l,r]`"). Without it, a range
update is n point updates = O(n log n). Lazy propagation defers the update on a
covering node (storing a pending value) and pushes it to children only when a later
query/update needs to descend — restoring O(log n). It's the segment tree's most
powerful extension.

---

## Definition of done

- [ ] Milestones M1–M4 implemented (M5–M6 recommended; lazy propagation is the prize).
- [ ] Full suite passes, including the brute-force cross-check and the split-equivalence
      invariant.
- [ ] **`make test` clean under ASan/UBSan; `valgrind` zero leaks.**
- [ ] §6 filled from your reasoning; you can explain why query/update are O(log n),
      why build is O(n), and why the array needs 4n slots.
- [ ] `PROGRESS.md` written. Code review requested.

→ Next: [`../bonus/`](../bonus/) — a curated shelf of what comes after the core 16:
self-balancing trees, advanced structures, and where to go deeper.

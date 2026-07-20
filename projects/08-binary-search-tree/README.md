# Project 8 — Binary Search Tree

> **You are building:** an ordered container that keeps keys sorted while giving
> **O(log n) search, insert, AND delete** — the idea behind C++'s `std::set` /
> `std::map`, Java's `TreeMap` / `TreeSet`, and database indexes. (Those use a
> *balanced* variant; you build the plain version first, then feel exactly why
> balancing was invented.)
>
> **Builds on:** recursion fluency and Lesson 2 (nodes with pointers). Lesson 5
> (your queue) reappears in the level-order stretch goal. Sets up Lesson 9 (heaps)
> and the bonus self-balancing trees (AVL / Red-Black).
>
> **The C leap:** recursion over a heap-allocated linked structure. Every core op
> is a recursive helper, and two things bite: **freeing must go post-order**
> (children before parent) or you leak whole subtrees, and **recursion depth ==
> tree height**, so a degenerate tree can overflow the real call stack.
>
> **Mentor rule:** diagrams and questions here, not code. Draw the delete cases on
> paper before you touch the keyboard.

---

## 1. Motivation

### The problem

You have two structures that each win half the battle:

- A **sorted array** gives O(log n) *search* (binary search) but O(n) *insert* —
  every insert shifts the tail to keep order.
- A **linked list** gives O(1) *insert* but O(n) *search* — no random access, you
  walk it.

Can one structure give **O(log n) search, insert, and delete** while keeping the
elements ordered? That's the binary search tree.

### The idea

Impose one invariant, at **every** node: all keys in the left subtree are smaller,
all keys in the right subtree are larger. Searching becomes a decision at each
node — "go left or go right" — that throws away half the remaining keys per step,
exactly like binary search, but now insert and delete are *also* just "walk to the
right spot and splice a node." No shifting.

### The trade-off (and the catch)

The O(log n) promise holds only when the tree is **balanced** (short and bushy).
Feed a plain BST already-sorted data and it degenerates into a **linked list** —
every node hangs off the right, height n, everything O(n). Fixing that is what
AVL and Red-Black trees do (the bonus lessons); building the plain BST first is
how you earn the right to appreciate them.

### The C-specific twist

In Java the garbage collector reclaimed unreachable nodes. In C, **you free every
node yourself**, and a tree makes ordering matter: you must free a node's children
*before* the node, because freeing the node throws away the pointers to them.
That's **post-order** traversal — one of the three traversals, doing real work.

---

## 2. Visualization

### The structure and its one invariant

```
                 (50)                     left < node < right, everywhere:
                /    \                       20,30,40  <  50  <  60,70,80
             (30)    (70)                    20        <  30  <  40
             /  \    /  \                    60        <  70  <  80
          (20)(40) (60)(80)
```

### `contains(60)` — halve at each step, O(height)

```
   at 50: 60 > 50 → go RIGHT
   at 70: 60 < 70 → go LEFT
   at 60: 60 == 60 → FOUND        (3 comparisons for 7 nodes)
```

### `insert(65)` — walk to a NULL, graft a leaf there

```
   at 50: 65 > 50 → right
   at 70: 65 < 70 → left
   at 60: 65 > 60 → right ... which is NULL → put the new leaf here:

             (60)
                \
                (65)   ← new leaf
```

### `delete` — the three cases (the famous hard part)

```
 (a) LEAF: just unlink & free.            (b) ONE CHILD: splice the child up.
     delete 20:                               delete 30 (only child 40):
          (30)              (30)                   (30)             (40)
          /       →           \                    /     →
       (20)                  (nothing)          (40)
                                              [30's parent now points at 40]

 (c) TWO CHILDREN: can't just remove — who takes its place?
     delete 50:
              (50)                         Find the IN-ORDER SUCCESSOR: the
             /    \                        smallest key in the RIGHT subtree
          (30)    (70)                     (walk right once, then left, left...).
                  /  \                     Here that's 60. COPY 60 up into the
               (60)  (80)                  node, then delete the old 60 (which
                                           is now a leaf or one-child case).
     result:  (60) sits where 50 was; the tree stays ordered because 60 is the
              smallest key still larger than everything on the left.
```

**Draw all three delete cases on paper.** Case (c) is where every bug lives — if
you can reproduce it from memory, you understand the structure.

### Why in-order yields sorted output

In-order = **recurse left, visit node, recurse right**. The invariant says
everything left of a node is smaller and everything right is larger, so "all of
left, then me, then all of right" emits keys in ascending order — recursively,
all the way down. This is the proof that a BST is an *ordered* container.

```
   in-order(50): [in-order(30)=20,30,40] , 50 , [in-order(70)=60,70,80]
              = 20 30 40 50 60 70 80        ← sorted!
```

---

## 3. Design

```c
typedef int bst_key;                 /* one name to change; C has no generics */

typedef struct BSTNode {
    bst_key          key;
    struct BSTNode  *left;
    struct BSTNode  *right;          /* NULL child = empty subtree = base case */
} BSTNode;

typedef struct {
    BSTNode *root;                   /* NULL when empty */
    size_t   size;                   /* cached count → bst_size is O(1) */
} BST;
```

### Design questions to settle before coding

1. **Duplicate keys — reject, count, or allow?** This BST models a **set of ints**:
   inserting an existing key is a **no-op** (size unchanged). Alternatives: keep a
   per-node count (a multiset), or push duplicates consistently to one side. Pick
   one; we reject. State why a consistent rule matters (an inconsistent one breaks
   search).
2. **Recursion or iteration?** Insert/search/delete are cleanest recursively via a
   helper that **takes a subtree root and returns the (possibly new) subtree root**
   (`node->left = insert_node(node->left, key)`). That return-the-root idiom wires
   a new/rewired child back into its parent without special-casing the tree root.
   The cost: depth = height (see the stack-overflow risk, §Common mistakes). An
   iterative version avoids that — a good stretch goal.
3. **Height of an EMPTY tree — the `size_t` trap.** Height is naturally measured in
   *edges*: a single leaf is 0, an empty tree is "−1 edges." But `bst_height`
   returns `size_t` (unsigned), and −1 wraps to `SIZE_MAX`. Decide: clamp empty to
   0, or change the return type to a signed `int`/`ptrdiff_t`? We clamp empty to 0
   and document it. Whatever you choose, TEST the leaf and empty cases explicitly.
4. **Error policy.** `bst_insert` on malloc failure, `bst_remove` of a missing key
   (we return 0/1 status), `bst_contains` (returns 0/1). There's no ambiguous
   "value" to return here (unlike the hashmap), so status ints are clean. Be
   consistent.
5. **Ownership of the size cache.** `size` is a promise: every insert that actually
   adds a node does `size++`, every remove that actually deletes does `size--`. A
   no-op insert must NOT bump it. Thread an `*inserted` / `*removed` flag out of
   the recursive helper so the public function knows whether the count changed.

> Don't code until you can state, in a sentence each: your duplicate policy, why
> in-order is sorted, and what the successor is in the two-children delete.

---

## 4. Milestones

Scaffold: [`src/bst.c`](src/bst.c) / [`src/bst.h`](src/bst.h). Implement **in
order**, compiling and testing under ASan after each.

```
[ ] M0  Build: header + compiling .c with TODO-asserts; `make test` builds and
        aborts at the first TODO (bst_new). EXPECTED warnings: several static
        helpers (node_new, insert_node, contains_node, free_node, height_node,
        inorder_node, find_min, remove_node) are "defined but not used" until the
        milestone that calls them — that's the compiler flagging not-yet-wired
        code, and it clears milestone by milestone.

[ ] M1  bst_new (malloc handle, root = NULL, size = 0) / bst_free. free_node must
        be POST-ORDER: free left, free right, THEN free node. bst_free(NULL) is a
        no-op; free the nodes before the handle. new-then-free: ZERO leaks (ASan).

[ ] M2  node_new + insert_node (recursive, returns subtree root) + bst_insert.
        Duplicate key = no-op; only bump size when a node was actually created.

[ ] M3  contains_node + bst_contains: the go-left-or-right search. Test present and
        absent keys, including the root.

[ ] M4  inorder_node + bst_inorder(out): fill a caller array left→node→right.
        Assert the output is strictly ascending — this PROVES the BST is ordered.

[ ] M5  bst_size (return the cache, O(1)) + height_node + bst_height. Mind the
        empty-tree convention (§3 Q3). Test a leaf (0) and the balanced tree (2).

[ ] M6  remove_node + bst_remove — the THREE CASES (leaf / one child / two
        children via in-order successor). After each delete, re-run bst_inorder
        and confirm it's still sorted (a great correctness invariant). Only
        decrement size when a node was actually removed.

[ ] M7  Full suite green under ASan; valgrind ZERO leaks. Request review.

Stretch (keep the core build self-contained — don't add these to the Makefile
unless you also add the queue module):
  - Iterative in-order traversal using an explicit stack (Lesson 4) — no recursion.
  - LEVEL-ORDER (breadth-first) traversal using your Lesson 5 queue of node
    pointers. This is why the "Builds on" mentions the queue.
  - Pre-order / post-order traversals.
  - Self-balancing (AVL rotations) so height stays O(log n) on sorted input.
```

**Why this order?** Free before insert (so every test can clean up and stay
leak-free), insert before search, search before the traversal that validates
ordering, and delete last because it's the hardest and leans on `find_min`.

---

## 5. Testing

```bash
cd projects/08-binary-search-tree
make test                 # builds with ASan+UBSan and runs the tests
make && valgrind --leak-check=full --error-exitcode=1 ./out_test
```

The leak audit is important: a BST is many separate mallocs, and the classic bug
is dropping a subtree by freeing a parent before its children. valgrind names the
exact `malloc` you failed to free.

Starter: [`tests/test_bst.c`](tests/test_bst.c).

### Manual test cases & expected outputs

| # | Action | Expected |
|---|--------|----------|
| 1 | `bst_new` then `bst_size` | `0`; `contains(x)` → false |
| 2 | insert `50,30,70,20,40,60,80`, `bst_size` | `7` |
| 3 | insert `50` again, `bst_size` | still `7` (no duplicates) |
| 4 | `bst_inorder` into an array | `20 30 40 50 60 70 80` (sorted) |
| 5 | `bst_height` | `2` (3 levels, edges) |
| 6 | `remove(20)` (leaf), `contains(20)`, size | `1`, false, `6` |
| 7 | `remove(30)` (one child 40), `contains(40)`, size | `1`, true, `5` |
| 8 | `remove(50)` (two children, root), size, in-order | `1`, `4`, still sorted |
| 9 | `bst_new` then `bst_free` | zero leaks (ASan/valgrind) |

### Edge cases (where the bugs hide — test all)

- **Empty tree:** `bst_contains`, `bst_inorder` (writes 0), `bst_height`, and
  `bst_free` all on a fresh tree — no crash, no leak.
- **Degenerate insert:** insert `1,2,3,4,5` *in order*. Height should be `4` — the
  tree is a linked list. This is the O(n) worst case; see §6.
- **Delete the root when it's the only node** → empty tree, size 0, root NULL.
- **Delete down to empty, then insert again** — the tree must recover.
- **Two-children delete where the successor has a right child** — make sure that
  child isn't lost when the successor is spliced out.
- **Delete a missing key** — returns 0, size unchanged, no crash.

---

## 6. Complexity Analysis

Fill this in **yourself** first. `h` = height; note where it's `log n` vs `n`.

| Operation | Balanced (avg) | Worst (degenerate) | Why |
|-----------|:--------------:|:------------------:|-----|
| `bst_contains` | ? | ? | ? |
| `bst_insert` | ? | ? | ? |
| `bst_remove` | ? | ? | ? |
| `bst_inorder` | ? | ? | ? |
| `bst_height` | ? | ? | ? |
| `bst_size` | ? | — | ? |

<details>
<summary>Answers + the average-vs-worst insight.</summary>

- `contains`/`insert`/`remove`: **O(h)** — each walks one root-to-leaf path. With a
  **balanced** tree h ≈ log n → **O(log n)**. With a **degenerate** tree (sorted
  input) h = n → **O(n)**.
- `bst_inorder`: **O(n)** always — it visits every node once (it must, to list them
  all). Space O(n) for the output array, plus O(h) recursion stack.
- `bst_height`: **O(n)** — it, too, visits every node (unless you cache height).
- `bst_size`: **O(1)** — we keep a cached counter instead of walking.

### Average vs. worst — the whole point

A BST built from **random** insertion order is balanced *in expectation* → height
Θ(log n). But insert **already-sorted** keys and every node hangs off the right:
height n, and the "tree" is a linked list — all ops O(n). Search structures can't
tolerate that, which is exactly why **self-balancing** trees (AVL, Red-Black)
exist: they do a little rotation work on each insert/delete to guarantee height
O(log n) *regardless* of input order. Build the plain BST, watch it degenerate,
and you'll understand why the balanced variants earn their complexity.

### One more C angle: recursion depth

Because depth == height, a degenerate tree of a few hundred thousand sorted keys
can **overflow the call stack** in the recursive traversal/free — a crash, not a
wrong answer. Balanced trees (log n depth) or iterative traversals with an
explicit stack avoid it.
</details>

---

## 7. Real-World Usage

- **C++ STL:** `std::set` / `std::map` / `std::multimap` are **Red-Black trees** —
  an ordered map with O(log n) ops and in-order iteration for free.
- **Java:** `TreeMap` / `TreeSet` — also Red-Black. `HashMap` beats them on raw
  lookup, but the tree keeps keys *sorted* and supports range queries
  (`headMap`, `tailMap`, `firstKey`).
- **Databases & filesystems:** indexes are **B-trees / B+-trees** — the BST idea
  widened to many keys per node so each node is one disk page. Same "ordered,
  logarithmic" DNA.
- **Java 8 `HashMap` treeification** (Lesson 6): a bucket whose chain grows past 8
  converts into a Red-Black tree, so a bad-hash worst case is O(log n), not O(n) —
  a direct marriage of the hashmap and this lesson.
- **Anywhere you need "sorted + fast insert/delete + range queries"** at once: a
  plain array or hashmap can't do all three; a balanced BST can.

---

## 8. LeetCode

Attempt **in order**. Solve them in **C** to keep the pointer/recursion practice
going. **No solutions here — that's the point.**

1. **[Easy] 700. Search in a Binary Search Tree** — your `contains`, returning the
   node. The clean recursive base case.
2. **[Easy] 701. Insert into a Binary Search Tree** — your `insert`, returning the
   (possibly new) root. Cements the return-the-subtree idiom.
3. **[Easy] 94. Binary Tree Inorder Traversal** — recursive first, then **iterative
   with an explicit stack** (your Lesson 4). Feel why in-order is sorted for a BST.
4. **[Medium] 98. Validate Binary Search Tree** — a subtle bounds-bug magnet:
   checking only immediate children is wrong; you must carry `(min, max)` bounds
   down. Great test of whether you truly grok the invariant.
5. **[Medium] 230. Kth Smallest Element in a BST** — in-order + a counter; stop
   early at the k-th. Builds directly on M4.

For each: which BST property does it lean on — the invariant, the go-left/right
search, or the sorted in-order? Name it before you start.

---

## Exercises

1. **`bst_min` / `bst_max`** — walk hard-left / hard-right. O(h). You already need
   `find_min` for delete; expose it and test it.
2. **`bst_clear`** without freeing the handle — reset to empty, reusable. (Reuse
   `free_node`, then `root = NULL; size = 0`.)
3. **Recursive `bst_size` vs. the cache.** Write an O(n) `count_nodes(root)` helper
   and assert it equals the cached `size` after a batch of inserts/removes — a
   cheap invariant check that catches a mis-maintained counter.
4. **`bst_contains` iteratively** — a `while (node)` loop, no recursion. Compare
   with the recursive version; note it can't overflow the stack.

## Challenge problems

1. **Iterative in-order** with an explicit stack (your Lesson 4 stack of
   `BSTNode*`). This is LeetCode 94's harder half and the key to O(1)-extra-space
   tree walking.
2. **Level-order (BFS)** with your Lesson 5 queue of node pointers: enqueue root,
   then loop — dequeue, visit, enqueue its non-NULL children. Prints the tree
   row by row. (Compose the queue module into the Makefile if you build this.)
3. **In-order predecessor delete** — redo case (c) using the predecessor (max of
   the left subtree) instead of the successor. Same correctness, mirror image.
4. **`bst_is_valid`** — verify the invariant with (min, max) bounds (LeetCode 98).
   Run it after every mutation in your tests as a paranoia check.

## Stretch goals

- **AVL balancing.** Track height (or a balance factor) per node and rotate on
  insert/delete to keep height O(log n). Re-run the "insert 1..n sorted" test and
  watch height stay ~log n. This is the payoff of the whole lesson.
- **Augment for order statistics.** Store a subtree-size per node → O(log n)
  "k-th smallest" and "rank of key." (This is how you'd *really* do LeetCode 230.)
- **Range query** `bst_keys_in(lo, hi)` — a pruned in-order that skips subtrees
  entirely out of range.

## Common mistakes

- **Freeing pre-order (parent first).** `free(node)` before recursing into its
  children drops `node->left`/`node->right` → the whole subtree leaks, and reading
  the freed node is UB. **Free post-order.** valgrind will name the lost mallocs.
- **The two-children delete.** Forgetting to actually remove the successor after
  copying its key (now you have it twice), or picking a successor that has a right
  child and losing that child. Draw it; step through it.
- **Bumping `size` on a duplicate insert** (or not decrementing on a real delete) —
  the cache drifts. Thread an `*inserted`/`*removed` flag out of the helper.
- **`size_t` height underflow.** Returning "−1 edges" for an empty subtree wraps to
  `SIZE_MAX`. Clamp empty to 0 (or return signed) and TEST it.
- **Not reassigning the returned subtree root** — writing `insert_node(node->left,
  ...)` without `node->left = ...` silently drops the new node. The return value is
  load-bearing.
- **Assuming O(log n).** It's O(h); on sorted input h = n. Don't promise logarithmic
  without balancing.

## Debugging tips

- Write a `bst_print` (in-order, or an indented sideways tree) early — seeing the
  shape makes invariant violations obvious.
- After every mutation in a test, call `bst_inorder` and check it's still sorted.
  Any bug in insert/delete shows up as an out-of-order pair immediately.
- Reproduce delete bugs on the **smallest** tree that triggers them (three nodes is
  enough for all three cases).
- Read the ASan report: it points at the exact use-after-free line when a delete
  mishandles a child pointer.
- Print `size` alongside the in-order count — they must always agree.

## Frequently asked questions

**Q: Why the in-order *successor* in the two-children delete, specifically?**
A: The successor is the smallest key still larger than the node being deleted —
the *next* key in sorted order. Dropping it into the node's slot keeps everything
on the left smaller and everything on the right larger, so the BST invariant
survives. The predecessor (largest key on the left) works identically by symmetry;
either is correct.

**Q: Why is in-order sorted but pre-order and post-order aren't?**
A: In-order visits *everything smaller* (left) before the node and *everything
larger* (right) after it — precisely ascending order, by the invariant, at every
level. Pre-order (node first) and post-order (node last) don't respect that
left<node<right ordering in their visit sequence, so they scramble it.

**Q: When does the recursion actually overflow the stack?**
A: When the tree is deep — and depth == height. A balanced tree of a billion nodes
is only ~30 deep (fine), but a *degenerate* tree (sorted input) of a few hundred
thousand nodes is that many frames deep and can crash. Balance the tree, or
traverse iteratively with an explicit heap-allocated stack.

**Q: Why keep a `size` cache instead of counting nodes on demand?**
A: Counting is O(n); the cache makes `bst_size` O(1). The price is a discipline:
every real insert/delete must update it. That "maintain the invariant everywhere"
habit is the same one the linked list's `tail` pointer taught.

---

## Definition of done

- [ ] Milestones M0–M6 implemented (M7 + stretches recommended).
- [ ] Full test suite passes, including every edge case in §5 — especially all
      three delete cases and the "still sorted after delete" invariant.
- [ ] **`make test` clean under ASan/UBSan; `valgrind` reports zero leaks** (a tree
      is many mallocs — this is the real test of your post-order free).
- [ ] You filled in the §6 table from your own reasoning and can explain, out loud,
      average-vs-worst and why sorted input degenerates the tree.
- [ ] `PROGRESS.md` written (copy from [`../../docs/progress-template.md`](../../docs/progress-template.md)).
- [ ] You requested and received a code review.

→ Next: [`../09-heap-priority-queue/`](../09-heap-priority-queue/) — another
tree, but with a shocking twist: it's *complete*, so it lives in a **plain array**
with no pointers at all, and index arithmetic replaces `left`/`right`.

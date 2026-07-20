# Bonus Shelf — Advanced & Specialized Structures

> **The stretch curriculum.** Tackle these once the 16 core projects feel solid.
> Each answers a question the core raised — *"what fixes a degenerate BST?", "how
> does a database index actually work?"* — and rewards the pointer-and-memory
> intuition you've now built in C.
>
> **How to build them (same contract as the core):** for any you implement, follow
> the repo conventions — a `src/<name>.{h,c}` with the struct + prototypes and
> TODO-milestone scaffold, a `tests/test_<name>.c` using `../../docs/test_utils.h`,
> and a `Makefile` copied from a core lesson (single-module, or the composition
> pattern from Lesson 4 if it builds on one). Compile under `-fsanitize=address,
> undefined` and keep valgrind clean — these structures have the richest pointer
> surgery in the whole course, so the sanitizers earn their keep.
>
> **Mentor rule still applies:** ask for hints, diagrams, and design questions —
> not the implementation. Each of these deserves the full eight-section treatment;
> ask when you reach it.

Suggested order is roughly easiest → hardest, but pick by curiosity.

---

## AVL Tree
**Answers:** *what fixes the O(n) worst case of a plain BST (Lesson 8)?*
A self-balancing BST that keeps subtree heights within 1 via **rotations** after
insert/delete, guaranteeing O(log n). The cleanest introduction to balancing.
**Key idea:** balance factor + four rotation cases (LL, RR, LR, RL).
**C angle:** rotations are pure pointer rewiring (Lesson 3 skills); the recursive
insert returns the possibly-new subtree root, exactly like your BST. Watch that
every rotation updates *both* child links and the cached height.

## Red-Black Tree
**Answers:** *what do C++ `std::map`/`std::set` and Java `TreeMap`/`TreeSet` use?*
A balancing scheme with looser invariants than AVL (fewer rotations on update,
slightly taller trees) — the standard-library workhorse. Also the structure Java 8+
HashMap buckets **treeify** into (callback to Lesson 6).
**Key idea:** color invariants + recoloring vs. rotating.
**C angle:** the fiddliest case analysis in the course; a `node_color` enum and a
disciplined set of helper functions keep it sane. A great test of writing small,
correct, well-named static helpers.

## B-Tree
**Answers:** *how do databases and filesystems index data on disk?*
A balanced tree where each node holds *many* keys and has *many* children, sized to
a disk page to minimize slow I/O. The reason a database index lookup hits disk only
a handful of times.
**Key idea:** high branching factor → very shallow tree → few disk reads.
**C angle:** each node is a small array of keys + an array of child pointers — the
dynamic-array and pointer lessons combined. Splitting a full node on insert is the
core operation to get right.

## B+ Tree
**Answers:** *how do real database indexes support range scans efficiently?*
A B-tree variant where all values live in the leaves, and leaves are linked for fast
in-order range scans. The actual structure behind most relational DB indexes
(SQLite, MySQL/InnoDB, PostgreSQL).
**Key idea:** internal nodes route; linked leaves enable range queries.
**C angle:** combines the B-tree's node arrays with a Lesson 2/3 linked list across
the leaves — a satisfying synthesis once both feel natural.

## Skip List
**Answers:** *can we get O(log n) search without the complexity of tree balancing?*
A probabilistic, layered linked list giving expected O(log n) operations using
randomness instead of rotations. Used in Redis sorted sets and Java's
`ConcurrentSkipListMap`.
**Key idea:** express lanes — randomized levels skip ahead.
**C angle:** each node has a *variable-length* array of forward pointers (so you
`malloc` a node whose size depends on its random level) — a neat flexible-array-
member exercise. Far simpler to implement correctly than a red-black tree.

## Fenwick Tree (Binary Indexed Tree)
**Answers:** *is there something simpler than a segment tree (Lesson 16) for prefix
sums?* Yes — a compact array giving O(log n) prefix-sum queries and updates with
strikingly little code, using clever bit manipulation.
**Key idea:** `i & (-i)` isolates the lowest set bit to navigate responsibility
ranges.
**C angle:** one `long` array, two ~5-line loops, zero pointers — the most
code-light structure here after Union-Find. A beautiful demonstration that a great
data structure can be almost entirely *arithmetic*. Compare its footprint to your
segment tree.

## Bloom Filter
**Answers:** *how do we test set membership using almost no memory, if we can
tolerate false positives?* A probabilistic structure (a bit array + several hash
functions) that says "definitely not present" or "probably present." Used in
databases (LSM-tree lookups, e.g. RocksDB/Cassandra), browsers (malicious-URL
checks), and caches.
**Key idea:** trade exactness for enormous space savings — no false negatives, a
tunable false-positive rate.
**C angle:** real **bit manipulation** — pack bits into a `uint8_t`/`uint64_t` array
and set/test individual bits with shifts and masks. Reuse the FNV-style hashing from
Lesson 6 (with different seeds) for the k hash functions. Then *measure* the
false-positive rate against the theoretical formula.

---

### How to study the bonus shelf
Same loop as always (see [`../../docs/how-to-study.md`](../../docs/how-to-study.md)):
draw it, design the struct and its memory lifecycle, build one function, test it,
break it under the sanitizers, explain it. For each, explicitly connect it back to
the core lesson it extends or fixes — that connection is where the deepest
understanding lives:

- **AVL / Red-Black** fix the degenerate **BST** (Lesson 8).
- **B-Tree / B+ Tree** scale trees to **disk** and power the databases your day job
  runs on.
- **Skip List** reaches O(log n) with randomness instead of the **BST**'s balancing.
- **Fenwick Tree** is the lean cousin of your **Segment Tree** (Lesson 16).
- **Bloom Filter** is the probabilistic cousin of your **Hash Set** (Lesson 7).

Pick one that genuinely intrigues you and give it the same care you gave the core
16. You now have every prerequisite — pointers, recursion, hashing, arrays-as-trees,
and (the thing C forced you to learn) full ownership of your own memory.

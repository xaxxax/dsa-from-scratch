# Bonus Shelf — Advanced & Specialized Structures

> **Status:** the stretch curriculum. Each gets the full eight-section treatment
> on request. Tackle these once the 16 core projects feel solid — they reward the
> intuition you've built, and several answer questions the core projects raised
> ("what fixes a degenerate BST?", "how does a database index actually work?").

Suggested order is roughly easiest → hardest, but pick by curiosity.

---

## AVL Tree
**Answers:** *what fixes the O(n) worst case of a plain BST (Project 8)?*
A self-balancing BST that keeps subtree heights within 1 via **rotations** after
insert/delete, guaranteeing O(log n). The cleanest introduction to balancing.
**Key idea:** balance factor + four rotation cases (LL, RR, LR, RL).

## Red-Black Tree
**Answers:** *what does `java.util.TreeMap`/`TreeSet` actually use?*
A balancing scheme with looser invariants than AVL (fewer rotations on update,
slightly taller trees) — the standard library workhorse. Also the structure Java 8+
HashMap buckets **treeify** into (callback to Project 6).
**Key idea:** color invariants + recoloring vs. rotating.

## B-Tree
**Answers:** *how do databases and filesystems index data on disk?*
A balanced tree where each node holds *many* keys and has *many* children, sized
to a disk page to minimize slow I/O. The reason a database index lookup hits disk
only a handful of times.
**Key idea:** high branching factor → very shallow tree → few disk reads.

## B+ Tree
**Answers:** *how do real database indexes support range scans efficiently?*
A B-tree variant where all values live in the leaves, and leaves are linked for
fast in-order range scans. The actual structure behind most relational DB indexes
(MySQL/InnoDB, PostgreSQL).
**Key idea:** internal nodes route; linked leaves enable range queries.

## Skip List
**Answers:** *can we get O(log n) search without the complexity of tree
balancing?* A probabilistic, layered linked list giving expected O(log n)
operations using randomness instead of rotations. Used in Redis sorted sets and
`java.util.concurrent.ConcurrentSkipListMap`.
**Key idea:** express lanes — randomized levels skip ahead.

## Fenwick Tree (Binary Indexed Tree)
**Answers:** *is there something simpler than a segment tree (Project 16) for
prefix sums?* Yes — a compact array structure giving O(log n) prefix-sum queries
and updates with strikingly little code, using clever bit manipulation.
**Key idea:** `i & (-i)` isolates the lowest set bit to navigate responsibility ranges.

## Bloom Filter
**Answers:** *how do we test set membership using almost no memory, if we can
tolerate false positives?* A probabilistic structure (a bit array + several hash
functions) that says "definitely not present" or "probably present." Used in
databases (LSM-tree lookups), browsers (malicious-URL checks), and caches.
**Key idea:** trade exactness for enormous space savings — no false negatives,
tunable false-positive rate.

---

### How to study the bonus shelf
Same loop as always: draw it, design it, build one method, test it, explain it.
For each, explicitly connect it back to the core project it extends or fixes —
that connection is where the deepest understanding lives.

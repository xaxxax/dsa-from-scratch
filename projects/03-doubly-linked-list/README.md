# Project 3 — Doubly Linked List

> **You are building:** a linked list where every node points **both** forward and
> backward — the structure behind C++'s `std::list`, Java's `LinkedList`, and the
> Linux kernel's `list_head`.
>
> **Builds on:** Lesson 2. You already own single-direction pointer surgery. Now
> you add a `prev` pointer to every node, which buys three things the singly list
> couldn't do — and forces you to keep **two** links consistent on every edit.
>
> **The C leap:** the failure mode here is subtler than a leak. If you fix
> `next` but forget `prev`, the list looks perfect walking forward and is
> *corrupt* walking backward. Bugs hide in the direction you're not testing — so
> you test both.
>
> **Mentor rule:** diagrams and questions here, not code.

---

## 1. Motivation

### What the singly list couldn't do

In Lesson 2, `remove_last` was **O(n)**: from the tail you had no way back to find
its predecessor, so you walked from the head. And removing an arbitrary node
required tracking `prev` as you searched. The one-way street was the limitation.

### The idea

Give every node a **second pointer, `prev`,** to the node before it. Now:

- **`remove_last` is O(1)** — `tail->prev` *is* the new tail.
- **Remove-given-a-node is O(1)** — the node already knows both neighbors, so you
  can splice it out without searching for its predecessor.
- **Backward traversal** is possible (iterate from tail to head).

### The trade-off

Two pointers per node instead of one: **more memory** (another 8 bytes/node) and
**more work per edit** — every insert and remove now fixes links on *both* sides.
The bookkeeping roughly doubles. Whether that's worth it depends entirely on
whether you need O(1) removal-from-either-end or backward iteration. (The LRU cache
in Lesson 15 needs exactly this.)

### The C-specific twist

The dangerous new bug is **asymmetric corruption**: set `a->next = b` but forget
`b->prev = a`, and forward walks succeed while backward walks (or a later
`remove_last`) dereference a stale pointer. The discipline that prevents it: treat
link updates as **pairs** — every `x->next = y` is shadowed by a `y->prev = x`.

---

## 2. Visualization

```
        NULL           node            node            node          NULL
         ▲    ┌── prev ──┐  ┌── prev ──┐  ┌── prev ──┐  ┌── prev ──┐   ▲
 head ───┼──▶ [ 10 ]     └▶ [ 20 ]     └▶ [ 30 ]     └▶ [ 40 ] ◀──────┼─── tail
              [    ] ── next ─▶[    ] ── next ─▶[    ] ── next ─▶[ / ]

 head->prev == NULL          tail->next == NULL
```

### `add_first(5)` — fix FOUR pointers

```
before:  head─▶[10]⇄[20]   (⇄ means a matched next/prev pair)

 new node 5:   5.prev = NULL
               5.next = old head (10)
               10.prev = 5          ← the easy-to-forget backward link
               head   = 5
after:   head─▶[ 5]⇄[10]⇄[20]
```

### `remove(20)` from the middle — O(1), no predecessor search

```
before:  [10]⇄[20]⇄[30]        (20 knows both neighbors)
          10.next = 30
          30.prev = 10
          free(20)
after:   [10]⇄[30]
```

Compare Lesson 2, where you had to *walk* to find the node before `20`. Here the
node carries that knowledge.

### `remove_last` — the headline O(1) win

```
before:  ...[30]⇄[40]   tail─▶40
         tail       = 40.prev = 30
         30.next    = NULL
         free(40)
after:   ...[30]        tail─▶30
```

**Draw `add_first`, `add_last`, a middle `remove`, and `remove_last` on paper,
labeling every pointer you touch.** Missing one is the whole bug catalog of this
lesson.

---

## 3. Design

```c
typedef struct DNode {
    dll_elem      value;
    struct DNode *prev;
    struct DNode *next;
} DNode;

typedef struct {
    DNode  *head;
    DNode  *tail;
    size_t  size;
} DoublyLinkedList;
```

### Design questions to settle

1. **The two-directional invariant.** For every non-head node `n`,
   `n->prev->next == n`; for every non-tail node, `n->next->prev == n`;
   `head->prev == NULL`, `tail->next == NULL`. Write it down. Every function
   preserves it.
2. **Sentinel nodes (very common here).** Real doubly linked lists often use a
   *pair* of dummy head/tail sentinels (or one circular sentinel) so that **no
   operation ever special-cases an end** — `head` and `tail` are always real
   neighbors. We use plain `head`/`tail` first so you feel the end cases; then try
   the sentinel version as a stretch and watch the special cases vanish.
3. **Bidirectional `get`.** Since you can walk from either end, `get(index)` can
   start from `tail` when `index > size/2`, halving the average walk. Worth it?
   (Still O(n), but a real constant-factor win. Implement it as a stretch.)
4. **Error policy.** Same choice as before — `assert` vs. status for empty-list
   removal. Be consistent with Lessons 1–2.

---

## 4. Milestones

Scaffold in [`src/doubly_linked_list.c`](src/doubly_linked_list.c) /
[`src/doubly_linked_list.h`](src/doubly_linked_list.h).

```
[ ] M0  Build: header + compiling .c with TODO-asserts; make test aborts at dll_new.
[ ] M1  dll_new / dll_free (walk forward freeing each node; free handle last).
[ ] M2  dll_size / dll_is_empty.
[ ] M3  dll_add_first — fix next AND prev; handle empty (tail = new node).
[ ] M4  dll_add_last — the mirror; handle empty (head = new node).
[ ] M5  dll_get — bounds-check, walk from head. (Stretch: walk from nearer end.)
[ ] M6  dll_remove_first — fix new head's prev to NULL; empty ⇒ tail = NULL.
[ ] M7  dll_remove_last — the O(1) win; fix new tail's next to NULL; empty ⇒ head = NULL.
[ ] M8  dll_remove(value) — O(1) unlink using the node's own prev/next.
[ ] M9  dll_contains.
[ ] M10 dll_reverse — swap each node's prev/next, then swap head/tail.
[ ] M11 (Stretch) sentinel-node version; bidirectional get; backward iterator.
[ ] M12 Full suite green under ASan; valgrind clean. Request review.
```

---

## 5. Testing

```bash
cd projects/03-doubly-linked-list
make test
make && valgrind --leak-check=full --error-exitcode=1 ./out_test
```

### Edge cases (test all — bugs love the asymmetry)

- **After `remove_last`, does `add_last` still land correctly?** Only if `tail` and
  the new tail's `next` were fixed. (The starter test checks this.)
- **After a middle `remove`, walk forward past the gap** — both neighbors rejoined?
- **Remove head, remove tail, remove sole element** — three distinct end cases.
- **Down to empty via `remove_last`, then rebuild** — do head *and* tail recover?
- **Unsigned trap:** `dll_get(l, (size_t)-1)` rejected.
- **Reverse of 0- and 1-element lists** — no-ops, no crash.

---

## 6. Complexity Analysis

Derive first.

| Operation | Singly (L2) | Doubly (L3) | Why the difference |
|-----------|:-----------:|:-----------:|--------------------|
| `add_first` / `add_last` | O(1) | ? | ? |
| `remove_first` | O(1) | ? | ? |
| `remove_last` | **O(n)** | ? | ? |
| remove-given-a-node | O(n) | ? | ? |
| `get(i)` | O(n) | ? | ? |

<details>
<summary>Answers.</summary>

- Adds and both end-removals: **O(1)**. `remove_last` dropping from O(n) to O(1) is
  the entire reason this structure exists.
- Remove-given-a-node: **O(1)** (no predecessor search — the node knows it).
- `get(i)`: still **O(n)** — two pointers don't give random access; they just let
  you choose which end to start from.
- Space: **O(n)**, with a *larger constant* than the singly list (two pointers per
  node). The bidirectional convenience is paid for in bytes and in per-edit work.
</details>

---

## 7. Real-World Usage

- **C++ STL `std::list`** is a doubly linked list — chosen when you need O(1)
  `splice` and iterators that stay valid across insertions/removals elsewhere.
- **Java `LinkedList`** implements `Deque` on a doubly linked list — O(1) at both
  ends.
- **Linux kernel `struct list_head`** is an *intrusive circular* doubly linked list
  with a sentinel — the backbone of countless kernel data structures.
- **LRU cache (Lesson 15):** doubly linked list + hash map = O(1) get, O(1)
  move-to-front, O(1) evict-tail. You'll build exactly this.
- **Text editors / undo stacks / browser history:** move both directions in O(1).

---

## 8. LeetCode

**No solutions here.**

1. **[Medium] 146. LRU Cache** — the marquee application; you'll build it in L15.
2. **[Medium] 430. Flatten a Multilevel Doubly Linked List** — prev/next surgery.
3. **[Medium] 707. Design Linked List** — implement the whole API from scratch.
4. **[Easy] 206. Reverse Linked List** — compare to your O(1)-space `dll_reverse`.
5. **[Hard] 432. All O`one Data Structure** — doubly linked list of buckets + map.

---

## Exercises

1. **`dll_get` from the nearer end** — start at `tail` when `index > size/2`.
2. **`dll_remove_node(DNode *n)`** — unlink a node you already hold, in O(1). (This
   is the primitive the LRU cache needs.)
3. **Backward iteration** — a `dll_print_reverse` that walks tail→head. It only
   works if every `prev` is correct — a great corruption detector.
4. **`dll_insert_before(DNode *n, value)`** — O(1) insert given a node.

## Challenge problems

1. **Sentinel rewrite.** Add dummy head/tail nodes so no operation special-cases an
   end. Re-derive `add_first`/`remove_last` — notice they collapse to a single
   uniform "insert between two known nodes" / "unlink one known node".
2. **Make it circular.** `tail->next = head` and `head->prev = tail`. One sentinel
   now suffices. This is the kernel's design — implement and test it.
3. **Consistency checker.** Write `dll_check(l)` that walks forward counting nodes,
   then backward, and asserts both counts equal `size` and every `n->next->prev ==
   n`. Run it after every mutation in a test — it catches asymmetric corruption
   instantly.

## Stretch goals

- **Intrusive list**, kernel-style: the node struct is *embedded inside* the
  element, and the list stores no values of its own. Look up `container_of`.
- **Benchmark** `remove_last` against your Lesson 2 singly list — watch O(n) vs.
  O(1) diverge as n grows.

## Common mistakes

- **Fixing `next` but not `prev` (or vice versa).** The list works one direction
  and is corrupt the other. Update links in pairs.
- **Forgetting the empty-list transitions.** Removing the last remaining node must
  set *both* `head` and `tail` to NULL; adding to an empty list must set both to
  the new node.
- **Not NULL-ing the new end's outward link.** After `remove_last`, the new tail's
  `next` must become NULL; after `remove_first`, the new head's `prev` must.
- **Dereferencing a neighbor without checking it exists.** `n->prev->next = ...`
  crashes when `n` is the head. Guard: `if (n->prev) ... else l->head = ...`.
- **Leaking on free** — same as L2: walk and free every node; free the handle last.

## Debugging tips

- Implement the §Challenge **`dll_check`** consistency walker early and call it
  liberally. It converts silent asymmetric corruption into a loud, located failure.
- Print forward *and* backward after each op; a mismatch localizes the broken link.
- Reduce to 1–2 nodes: every end case (empty, single, head, tail) reproduces small.
- Trust ASan — a forgotten `prev` fix usually surfaces as a use-after-free on the
  next `remove_last`.

## Frequently asked questions

**Q: When is a doubly linked list worth the extra pointer over a singly list?**
A: When you need O(1) removal from the tail, O(1) removal given a node without
searching for its predecessor, or backward traversal. If you only ever push/pop at
the front and iterate forward, the singly list is leaner. The LRU cache is the
canonical "yes, you need doubly" case.

**Q: Why do real implementations use sentinel nodes?**
A: To erase end cases. With a permanent dummy head and tail, *every* real node has
a real neighbor on both sides, so insert/remove is always "splice between two known
nodes" — no `if (n->prev == NULL)` branches. The cost is one or two nodes of
overhead. Once you've felt the end cases here, the sentinel version is a joy.

**Q: Is `get(i)` any faster than the singly list?**
A: Same Big-O (O(n)), but you can start from whichever end is closer, roughly
halving the average number of hops. Constant-factor, not asymptotic — random access
still isn't a thing linked lists do.

---

## Definition of done

- [ ] Milestones M1–M10 implemented (M11–M12 recommended).
- [ ] Full suite passes, every §5 edge case covered, including the asymmetry checks.
- [ ] **`make test` clean under ASan/UBSan; `valgrind` zero leaks.**
- [ ] §6 table filled from your own reasoning; you can explain why `remove_last`
      drops from O(n) to O(1).
- [ ] `PROGRESS.md` written.
- [ ] Code review requested and received.

→ Next: [`../04-stack/`](../04-stack/), where you'll discover that a stack is not a
new structure at all — it's a *restriction* of one you've already built, and the
restriction is exactly what makes it useful.

# Project 2 — Singly Linked List

> **You are building:** a chain of separately-allocated nodes — the structure
> behind C++'s `std::forward_list`, and the classic teaching vehicle for
> *pointer surgery*.
>
> **Builds on:** Lesson 1 gave you the array's trade-offs. This is the mirror
> image: **O(1) insert/remove at the front, no resizing, no wasted contiguous
> block** — bought by **giving up O(1) random access**.
>
> **The C leap over Lesson 1:** the dynamic array was *one* allocation. A linked
> list is *n* allocations — one `malloc` per node. That changes everything about
> memory: `ll_free` must **walk the whole chain**, and if you drop `head` for even
> one statement, the entire tail leaks. This is the lesson where pointers stop
> being an abstraction and become the thing you manipulate directly.
>
> **Mentor rule:** diagrams and questions here, not code. Write every line.

---

## 1. Motivation

### The problem with the array

The dynamic array is superb — until you need to insert at the *front*. Prepending
to an array means shifting all `n` elements right (O(n)), and growth occasionally
triggers an O(n) copy of the whole block. Some workloads insert and remove at the
ends constantly (queues, editor buffers, LRU lists). For those, copying is wasteful.

### The idea

Stop demanding that elements sit **contiguously** in memory. Instead, let each
element live in its own **node** somewhere on the heap, holding its value and a
**pointer to the next node**. The list is just a pointer to the first node
(`head`). Insert at the front? Allocate one node, point it at the old head, done —
**O(1)**, no shifting, no copying, ever.

### The trade-off you're buying

| | Dynamic array | Singly linked list |
|---|---|---|
| `get(i)` | **O(1)** address math | **O(n)** — walk i pointers |
| `add_first` | O(n) shift | **O(1)** |
| `add_last` | amortized O(1) | O(1) *with a tail pointer* |
| memory | one block, ~2× slack | one malloc **per node** + a pointer of overhead each |
| cache | contiguous, fast | scattered, pointer-chasing, slower in practice |

There is no "better" structure — there is the right trade-off for the access
pattern. Naming that trade-off out loud is the point of the whole course.

### The C-specific twist

In Java, a `Node` was an object the GC collected when unreachable. In C, **every
node is a `malloc` you personally must `free`.** Freeing the list is no longer
"free one buffer" — it's "walk `n` nodes, freeing each, in an order that never
reads a node after freeing it." Get that order wrong and ASan will show you a
use-after-free. This is the real content of Lesson 2.

---

## 2. Visualization

### The chain

```
 LinkedList handle          the heap (nodes scattered anywhere)
 ┌───────────┐
 │ head  ●───┼──────▶ [10|●]───▶ [20|●]───▶ [30|●]───▶ [40| / ]
 │ tail  ●───┼───────────────────────────────────────────┘ ▲
 │ size  4   │                                       tail points here
 └───────────┘                              ( / = NULL, marks the end )
```

`head` and `tail` are pointers *into* the heap. The nodes are **not** next to each
other in memory — following the list means hopping from address to address.

### `add_first(5)` — O(1)

```
before:  head─▶[10|●]─▶[20| / ]
step 1:  new node [5|●] whose next = old head (10)
step 2:  head = the new node
after:   head─▶[ 5|●]─▶[10|●]─▶[20| / ]
```

### `remove(20)` from the middle — rewire, don't shift

```
before:  head─▶[10|●]─▶[20|●]─▶[30| / ]
                     prev    target
step:    prev->next = target->next     (10 now points past 20 to 30)
         free(target)
after:   head─▶[10|●]─▶[30| / ]
```

You needed the node **before** the target to do this — a singly linked list only
points forward, so you either track `prev` as you walk, or walk with a
"pointer-to-pointer" (stretch goal).

### `reverse` — the three-pointer dance

```
prev=NULL  cur─▶[10|●]─▶[20|●]─▶[30| / ]

each step: save next; point cur back at prev; advance both.

after:     [10| / ]◀─[20|●]◀─[30|●]◀─head    (arrows all flipped; head=old tail)
```

**Draw all four on paper before coding.** If you cannot reproduce the `remove`
rewiring from memory, you *will* write a dangling pointer.

---

## 3. Design

Decide first, then compare.

### The node and the handle

```c
typedef struct Node {
    ll_elem      value;
    struct Node *next;    /* self-referential — the essence of a linked structure */
} Node;

typedef struct {
    Node   *head;
    Node   *tail;    /* optional — see below */
    size_t  size;
} LinkedList;
```

Note `struct Node` names itself so `next` can point to its own type — you can't
`typedef`-name it before it's complete, hence the tag.

### Design questions to settle

1. **Do you keep a `tail`?** With one, `add_last` is O(1) (no walk to the end).
   The cost: **every mutator must maintain it.** `remove` the last node? Update
   `tail`. `remove_first` down to empty? `tail` must become NULL. Forgetting this
   is the signature bug of this project. (We keep `tail` — the header commits to
   it. Know what it costs.)
2. **`head`/`tail` NULL contract.** Empty list ⇔ `head == NULL` ⇔ `tail == NULL` ⇔
   `size == 0`. Write these equivalences down; every function must preserve them.
3. **A dummy/sentinel head node?** Some designs use a fake first node so there's
   never a "removing the head is special" case. We use a plain `head` (no
   sentinel) so you *feel* the head-is-special edge case — then you'll appreciate
   why sentinels exist. (Try it as a stretch goal.)
4. **Empty-list error policy.** `ll_remove_first` on an empty list — `assert`
   (caller bug) or a status return? Same choice as Lesson 1; be consistent.

> Don't code until you can state why `tail` helps, and exactly which operations
> must touch it.

---

## 4. Milestones

Implement **in order**, compiling and running after each. A scaffold with
signatures + TODOs is in [`src/linked_list.c`](src/linked_list.c) /
[`src/linked_list.h`](src/linked_list.h).

```
[ ] M0  Build: header (Node, LinkedList, prototypes) + a .c that compiles with
        TODO-asserts. `make test` builds and aborts at the first TODO (ll_new).

[ ] M1  ll_new / ll_free. ll_free must WALK the chain freeing every node, then
        the handle — grabbing `next` before freeing each node. new-then-free on
        an empty list: ZERO leaks under ASan.

[ ] M2  ll_size / ll_is_empty. Trivial; write the tests now for a clean baseline.

[ ] M3  ll_add_first: O(1) prepend. Handle the empty-list case (tail = new node).

[ ] M4  ll_add_last: O(1) append via tail. Handle empty vs. non-empty.

[ ] M5  ll_get: bounds-check (index >= size), then walk index hops. Feel the O(n).

[ ] M6  ll_remove_first: unlink + free the head; fix tail if list becomes empty.

[ ] M7  ll_remove(value): find the node and its predecessor; rewire; free.
        Cover all three cases — head, middle, tail (update tail!).

[ ] M8  ll_contains: linear scan.

[ ] M9  ll_reverse: three-pointer in-place reversal; swap head/tail roles.

[ ] M10 (Stretch) ll_insert_at / ll_remove_at(index); a sentinel-head variant;
        Floyd's cycle detection (tortoise & hare).

[ ] M11 Full suite green under ASan; valgrind reports zero leaks. Request review.
```

---

## 5. Testing

`#include "test_utils.h"` (the Makefile adds `docs/` to the include path). Starter
rhythm in [`tests/test_linked_list.c`](tests/test_linked_list.c).

```bash
cd projects/02-singly-linked-list
make test                                                   # ASan + UBSan
make && valgrind --leak-check=full --error-exitcode=1 ./out_test   # leak audit
```

### Edge cases (where the bugs live — test all)

- **The tail after removals.** Remove the last element via `ll_remove` — did
  `tail` follow, or does a later `add_last` write through a freed pointer? (ASan
  will catch it if it did.)
- **Down to empty, then add again.** Remove every element, then `add_first` — do
  `head` *and* `tail` recover?
- **Removing the head** vs. a middle node vs. the tail — three distinct code paths.
- **The unsigned trap.** `ll_get(l, (size_t)-1)` is a huge index, not "-1" — must
  be rejected by `index >= size`.
- **Reverse of 0- and 1-element lists** — must be no-ops that don't crash.
- **`ll_free` on a fresh empty list** — zero leaks, no NULL deref.

---

## 6. Complexity Analysis

Derive it yourself first.

| Operation | Time | Space | Why |
|-----------|:----:|:-----:|-----|
| `ll_add_first` | ? | O(1) | ? |
| `ll_add_last` (with tail) | ? | O(1) | ? |
| `ll_get` / `ll_contains` | ? | O(1) | ? |
| `ll_remove_first` | ? | O(1) | ? |
| `ll_remove(value)` | ? | O(1) | ? |
| `ll_reverse` | ? | O(1) | ? |
| whole structure | — | ? | ? |

<details>
<summary>Answers.</summary>

- `add_first`, `add_last` (with tail), `remove_first`: **O(1)** — a fixed number
  of pointer writes.
- `get`, `contains`, `remove(value)`: **O(n)** — you must walk to find the node.
- `reverse`: **O(n)** time, **O(1)** extra space (three pointers, no new nodes).
- Space: **O(n)** — but note the *constant* is worse than the array: each element
  also pays for a `next` pointer (8 bytes on 64-bit) plus allocator per-node
  overhead. And the nodes are scattered, so **cache locality is poor** — in
  practice a linked list is often slower than an array even where both are O(n),
  because the CPU stalls chasing pointers. This is a crucial real-world nuance
  Big-O hides.
</details>

---

## 7. Real-World Usage

- **C++ STL:** `std::list` (doubly, Lesson 3) and `std::forward_list` (singly, this
  one) — explicitly for O(1) splice/insert without invalidating other elements.
- **Linux kernel:** `struct list_head` — an *intrusive* doubly linked list used
  everywhere (process lists, LRU page lists). Worth reading; it's a masterclass.
- **Hash maps (Lesson 6):** separate chaining stores collisions as linked lists.
- **LRU cache (Lesson 15):** a linked list gives O(1) move-to-front / evict-tail.
- **Allocators / free lists:** the heap itself often threads free blocks on a
  linked list.

The linked list's superpower is **O(1) structural edits given a pointer to the
spot** — no shifting, no reallocation, and existing node addresses stay valid.

---

## 8. LeetCode

Attempt in order; solve in C to keep the pointer practice going. **No solutions.**

1. **[Easy] 206. Reverse Linked List** — exactly your `ll_reverse`. Do it iterative
   *and* recursive.
2. **[Easy] 876. Middle of the Linked List** — slow/fast pointers; one pass.
3. **[Easy] 21. Merge Two Sorted Lists** — pointer splicing; a dummy head shines.
4. **[Easy] 141. Linked List Cycle** — Floyd's tortoise & hare.
5. **[Medium] 19. Remove Nth Node From End** — two pointers a fixed gap apart.

For each: which linked-list property does it exploit — O(1) rewiring, sequential
access, or the impossibility of going backward?

---

## Exercises

1. **`ll_get_first` / `ll_get_last`** in O(1) (tail makes last free).
2. **`ll_index_of(value)`** returning the index or a "not found" sentinel — same
   unsigned-`size_t` wrinkle as Lesson 1.
3. **`ll_clear`** — free all nodes but keep the (now-empty) handle reusable.
4. **`ll_to_array`** — copy into a freshly `malloc`'d `ll_elem[]`. Who frees it?

## Challenge problems

1. **Pointer-to-pointer removal.** Rewrite `ll_remove` using `Node **pp = &l->head;`
   walking with `pp = &(*pp)->next;`. It erases the "head is special" case
   entirely — one uniform path. Understand *why* it does.
2. **Sentinel head.** Add a dummy first node so no operation special-cases the
   head. Compare the code's simplicity against the memory cost.
3. **Detect + break a cycle** you deliberately create by pointing `tail->next` back
   at a middle node (Floyd's algorithm).

## Stretch goals

- **Make `ll_free` handle an intrusive design** where the list doesn't own the
  values (they're pointers to objects someone else owns). Who frees what?
- **Benchmark** 1M `add_first` vs. a dynamic array's `insert(0)`. The linked list
  should crush it — then benchmark 1M random `get(i)` and watch the array win big.

## Common mistakes

- **Losing the rest of the list.** `l->head = l->head->next` *before* saving the
  old head leaks (or, if you freed first, dangles). Save `next`, then advance.
- **Forgetting `tail`.** Remove the last node without updating `tail`, then
  `add_last` writes through a freed pointer. Classic, and ASan-visible.
- **`ll_free` frees the handle but not the nodes** (or frees a node then reads its
  `next`). Walk first, free the handle last.
- **Off-by-one in the walk.** `ll_get(l, index)` should take exactly `index` hops
  from head — test `get(0)` and `get(size-1)` explicitly.
- **Reversing without fixing head/tail** — the pointers flip but `head` still names
  the old first node (now the tail).

## Debugging tips

- Write an `ll_print` (`10 -> 20 -> 30 -> /`) *first*. Seeing the chain after each
  op turns invisible pointer bugs visible.
- When a mutation misbehaves, print `head`, `tail`, and `size` together — their
  relationship (per §3's contract) tells you which invariant broke.
- Reduce to the smallest failing case: most linked-list bugs reproduce at 1–2
  nodes (empty, single, head-vs-tail).
- Trust ASan's stack trace — it points at the exact use-after-free line and the
  `free` that invalidated it.

## Frequently asked questions

**Q: Why is `get(i)` O(n) here but O(1) for the array?**
A: The array stores elements contiguously, so element `i` is at `base + i*size` —
one calculation. Linked-list nodes are scattered; the only way to find the i-th is
to follow i `next` pointers from the head. There's no arithmetic shortcut because
the addresses are unrelated.

**Q: If linked lists are often slower in practice, why learn them?**
A: (1) O(1) structural edits and stable node addresses are genuinely needed
(kernels, allocators, LRU, hash-map chaining). (2) They're the gateway to trees
and graphs — every "arrow" ahead is a `struct * ` you allocate and free, exactly
like `next` here. Master pointer surgery now and the rest of the course is
mechanics you already own.

**Q: Singly vs. doubly — why not always keep a `prev` too?**
A: `prev` makes backward traversal and O(1) removal-given-a-node possible, at the
cost of another pointer per node and *two* links to fix on every edit. That's
Lesson 3 — build the cheaper one first so you feel exactly what `prev` buys.

---

## Definition of done

- [ ] Milestones M1–M9 implemented (M10–M11 recommended).
- [ ] Full suite passes, every §5 edge case covered.
- [ ] **`make test` clean under ASan/UBSan; `valgrind` reports zero leaks** —
      especially important here, with one allocation per node.
- [ ] §6 complexity table filled from your own reasoning; you can explain *why the
      linked list can be slower than the array despite matching Big-O*.
- [ ] `PROGRESS.md` written (copy [`../../docs/progress-template.md`](../../docs/progress-template.md)).
- [ ] Code review requested and received.

→ Next: [`../03-doubly-linked-list/`](../03-doubly-linked-list/), where a second
pointer (`prev`) makes backward traversal and O(1) removal-given-a-node possible —
and doubles the number of links you must keep consistent.

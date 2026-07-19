# Project 5 — Queue (composed from your Linked List)

> **You are building:** a FIFO queue — and, crucially, discovering *why the array
> is the wrong backing store here* and the linked list is the right one.
>
> **Builds on:** Lesson 2 (singly linked list with a `tail`). The queue HAS-A
> linked list: enqueue == `add_last`, dequeue == `remove_first`.
>
> **The C leap:** same multi-module build as Lesson 4, but the lesson is about
> *choosing the right primitive*. You'll feel, concretely, why removing the front
> of an array costs O(n) and why the linked list sidesteps it — then rebuild the
> queue as a **circular array buffer** that makes the array O(1) at both ends after
> all.
>
> **Mentor rule:** short bodies, real design decisions. Answer the questions.

---

## 1. Motivation

### The problem

Countless systems process work in arrival order — **first in, first out**: print
spoolers, task schedulers, network packet buffers, and (Lesson 13) breadth-first
search. You need fast add at the **back** and fast remove from the **front**.

### Why the array struggles

Your dynamic array is O(1) at *one* end. Enqueue at the back? Fine — `da_add` is
amortized O(1). But dequeue from the front means `da_remove(0)`, which **shifts
every remaining element left one slot — O(n)**. A queue that's O(n) per dequeue is
useless at scale. The array's single fast end isn't enough; a queue needs *two*.

### The idea

A singly linked list with **both `head` and `tail`** is O(1) at both ends:
`add_last` (enqueue) uses the tail; `remove_first` (dequeue) uses the head. So the
queue simply composes your Lesson 2 list.

### The circular-buffer twist (the real payoff)

An array *can* do O(1) at both ends — if it stops insisting the front lives at
index 0. A **circular (ring) buffer** keeps `front` and `back` indices that wrap
around modulo capacity, so dequeue just advances `front` (no shifting). That's how
`java.util.ArrayDeque` and most high-performance queues work, and it's this
lesson's headline stretch goal.

---

## 2. Visualization

### Linked-list queue

```
   dequeue (O(1), remove head)                 enqueue (O(1), add at tail)
        ▼                                              ▼
      head ─▶ [A] ─▶ [B] ─▶ [C] ─▶ [D] ─▶ NULL         tail ─▶ D
      front                          back

   enqueue(E):  tail->next = E; tail = E     → ...[D]->[E]
   dequeue():   x = head; head = head->next; free(x); return x's value  → A
```

### Why array dequeue is O(n)

```
   [A][B][C][D]   dequeue A →  shift:  [B][C][D][ ]   ← moved 3 elements
```

### Circular buffer (stretch) — O(1), no shifting

```
   capacity 6, front=2, count=3
   index:  0    1    2    3    4    5
         [ .  ][ .  ][ B ][ C ][ D ][ .  ]
                       ^front        ^back=(front+count)%cap = 5

   dequeue: front = (front+1) % cap; count--     (B leaves; nothing shifts)
   enqueue: arr[(front+count)%cap] = x; count++  (wraps past the end)
```

---

## 3. Design

```c
typedef struct {
    LinkedList *backing;   /* HAS-A: front = head, back = tail */
} Queue;
```

### Design questions

1. **Which list end is the front?** It must be the **head** — because
   `remove_first` (head) is O(1), while removing the tail of a *singly* list is
   O(n) (no way back). So: dequeue from head, enqueue at tail. Convince yourself why
   the reverse mapping (front = tail) would make dequeue O(n).
2. **Why does enqueue need the `tail` pointer?** Without it, `add_last` walks the
   whole list to find the end — O(n). The tail is exactly what makes a linked-list
   queue viable. (This is why Lesson 2 kept a tail.)
3. **Array vs. list vs. ring buffer.** Plain array: O(n) dequeue — no. Linked list:
   O(1) both ends, but a `malloc`/`free` per element and poor cache locality. Ring
   buffer: O(1) both ends, contiguous, cache-friendly, but fixed capacity (or a
   resize policy). Name when each wins.
4. **Empty policy.** `dequeue`/`peek` on empty — `assert` or status. Consistent
   with earlier lessons.

---

## 4. Milestones

Scaffold: [`src/queue.c`](src/queue.c) / [`src/queue.h`](src/queue.h). The
[`Makefile`](Makefile) compiles your Lesson 2 `linked_list.c` alongside this.

```
[ ] M0  Build: compiles both modules; make test aborts at the first TODO.
[ ] M1  queue_new (ll_new the backing) / queue_free (ll_free then free struct).
[ ] M2  queue_size / queue_is_empty — delegate.
[ ] M3  queue_enqueue — ll_add_last (O(1) via the tail).
[ ] M4  queue_peek — ll_get(backing, 0), guard empty first.
[ ] M5  queue_dequeue — ll_remove_first (O(1)), guard empty first.
[ ] M6  (Stretch) Circular-array queue: a SEPARATE implementation with a fixed
        int[] + front/count indices and modulo wrap. O(1) both ends, no list.
        Decide the "full" condition and a resize (or reject) policy.
[ ] M7  (Stretch) Deque: add_front + remove_back too (needs a doubly linked list,
        Lesson 3, or a ring buffer). Compare with java.util.ArrayDeque.
[ ] M8  Full suite green under ASan; valgrind clean. Request review.
```

---

## 5. Testing

```bash
cd projects/05-queue
make test
make && valgrind --leak-check=full --error-exitcode=1 ./out_test
```

Starter: [`tests/test_queue.c`](tests/test_queue.c). Aborts inside
`linked_list.c` ⇒ finish Lesson 2.

### Edge cases

- **Strict FIFO under load** — enqueue 1..100, dequeue all, verify exact order.
- **Interleave** enqueue/dequeue so the queue repeatedly nears empty — order and
  size stay correct?
- **Dequeue/peek empty** — your chosen policy fires.
- **Empty then reuse.** `queue_free` on a fresh queue — zero leaks.
- **(Ring buffer) wrap-around**: fill, dequeue a few, enqueue past the end so
  indices wrap; nothing is lost or overwritten. Test the exact "full" boundary.

---

## 6. Complexity Analysis

| Operation | Linked-list queue | Ring-buffer queue |
|-----------|:-----------------:|:-----------------:|
| `enqueue` | ? | ? |
| `dequeue` | ? | ? |
| `peek` / `size` / `is_empty` | ? | ? |
| space | ? | ? |

<details>
<summary>Answers.</summary>

- Linked list: enqueue/dequeue/peek/size all **O(1)**; space **O(n)** with a
  per-node pointer + allocator overhead, scattered in memory.
- Ring buffer: same **O(1)** operations, space **O(capacity)** contiguous
  (cache-friendly), but capacity is fixed unless you add a resize (amortized O(1)
  like the dynamic array). The ring buffer usually wins in practice on speed; the
  list wins on never needing a capacity decision.
</details>

---

## 7. Real-World Usage

- **OS schedulers** — run queues of ready processes/threads.
- **Networking** — packet buffers, socket backlogs are queues (often ring buffers).
- **BFS (Lesson 13)** — the frontier is a queue; that's *why* BFS finds shortest
  paths in unweighted graphs (it explores in order of distance).
- **Producer/consumer & message queues** — Kafka, RabbitMQ, `SharedArrayBuffer`
  ring buffers, lock-free SPSC queues in audio/embedded — all this structure.
- **`java.util.ArrayDeque`, C++ `std::deque`/`std::queue`** — ring-buffer-backed.

---

## 8. LeetCode

**No solutions here.**

1. **[Easy] 232. Implement Queue using Stacks** — two stacks make a queue; deepens
   both structures.
2. **[Easy] 933. Number of Recent Calls** — a sliding time window via a queue.
3. **[Medium] 622. Design Circular Queue** — build the ring buffer directly (M6).
4. **[Medium] 239. Sliding Window Maximum** — monotonic *deque*, advanced.

---

## Exercises

1. **`queue_to_array`** front→back into a fresh `int[]`. Who frees it?
2. **Bounded queue** — a max size; enqueue rejects (or overwrites oldest — a
   "ring/log" policy) when full. Which policy suits a metrics buffer?
3. **Stack-backed queue** (LeetCode 232) — two stacks, amortized O(1) dequeue.
   Explain the amortization.

## Challenge problems

1. **Circular buffer (M6).** Fixed `int[]`, `front`, `count`. `enqueue` writes at
   `(front+count)%cap`; `dequeue` advances `front`. Nail the full/empty conditions
   (why `count`, not comparing `front==back`?).
2. **Growable ring buffer.** When full, double capacity — but you must *linearize*
   the wrapped elements into the new array in order. Trickier than the dynamic
   array's straight copy; draw it.
3. **Lock-free SPSC queue** (advanced) — a single-producer/single-consumer ring
   buffer with atomic head/tail indices. A real taste of concurrency.

## Stretch goals

- **Deque** (M7): O(1) at both ends both directions — doubly linked list or ring
  buffer. This is what `ArrayDeque` gives you.
- **Benchmark** linked-list queue vs. ring buffer for 10M enqueue/dequeue. The ring
  buffer's cache locality should show.

## Common mistakes

- **Mapping front to the tail** — then dequeue is O(n) on a singly list. Front =
  head.
- **Enqueue that walks the list** because you didn't use the tail pointer — silently
  O(n). Use `ll_add_last`.
- **(Ring buffer) confusing full and empty.** With only `front`/`back` indices,
  `front == back` is *ambiguous* (both empty and full). Track a `count`, or leave
  one slot unused — pick one and be rigorous.
- **Ownership** — `queue_free` must free the backing list, then the struct.

## Debugging tips

- Print front→back after each op; FIFO bugs are obvious once you can see order.
- For the ring buffer, print `front`, `count`, and the raw array with the active
  window marked — wrap-around bugs jump out.
- Reduce to capacity 2–3 for the ring buffer; wrap happens immediately.

## Frequently asked questions

**Q: Why not just use the dynamic array for the queue?**
A: Dequeue = remove index 0 = shift everything left = O(n). A queue that's O(n) per
dequeue defeats the purpose. The array is a fine *stack* (one fast end) but a poor
*queue* (needs two) — unless you make it circular, which removes the shifting.

**Q: If the linked-list queue is O(1) everywhere, why bother with a ring buffer?**
A: Constant factors and memory. The list does a `malloc`/`free` per element and
scatters nodes across memory (cache-unfriendly). A ring buffer is one contiguous
block, no per-op allocation, and far better locality — usually meaningfully faster
even though both are O(1).

**Q: Why does BFS need a queue and DFS a stack?**
A: A queue processes nodes in the order discovered — so BFS expands outward level by
level (nearest first), which is exactly what finds shortest paths in unweighted
graphs. A stack processes most-recently-discovered first, driving DFS deep. Same
graph, different frontier structure, different traversal. You'll see it in L12/L13.

---

## Definition of done

- [ ] Milestones M1–M5 implemented (M6 circular buffer strongly recommended — it's
      the real lesson; M7–M8 optional).
- [ ] Full suite passes, every §5 edge case covered.
- [ ] **`make test` clean under ASan/UBSan; `valgrind` zero leaks** (both modules).
- [ ] §6 table filled from your reasoning; you can explain why the array is O(n) per
      dequeue and how the ring buffer fixes it without shifting.
- [ ] `PROGRESS.md` written. Code review requested.

→ Next: [`../06-hashmap/`](../06-hashmap/) — the first structure that isn't a
sequence. You'll turn a *key* into an array index with a hash function, handle
collisions with the linked lists you already built, and get to average-case O(1)
lookup by value.

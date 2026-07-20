# Project 15 — LRU Cache (HashMap + Doubly Linked List)

> **You are building:** the capstone — a fixed-capacity cache with **O(1) get and
> put** that evicts the **least-recently-used** entry when full. It's the design
> behind CPU caches, database buffer pools, and `functools.lru_cache`; it's also the
> single most-asked "design a data structure" interview question (LeetCode 146).
>
> **Builds on:** everything about *why* structures combine — a **hash map** (Lesson
> 6) for O(1) lookup and a **doubly linked list** (Lesson 3) for O(1) recency
> ordering. This is where you see two structures cooperate to beat what either can
> do alone.
>
> **The C leap:** the **intrusive** node — one `malloc`'d node that lives in *two*
> data structures simultaneously (the recency list via `prev`/`next`, its hash
> bucket via `hnext`). Every operation must keep both views in sync, and eviction
> must remove the victim from both and free it exactly once.
>
> **Mentor rule:** diagrams and questions here, not code.

---

## 1. Motivation

### The problem

Memory (or any fast store) is finite, but the data you might want is not. A **cache**
keeps a bounded number of entries and, when full, must throw one out. The best guess
for "what won't I need soon?" is often **least recently used** — evict whatever
hasn't been touched for the longest. The challenge: do `get` (with "mark as
recently used") and `put` (with "evict the LRU when full") both in **O(1)**.

### Why one structure isn't enough

- A **hash map** finds a key in O(1) — but has no concept of order, so it can't tell
  you which entry is oldest, and scanning for it is O(n).
- A **doubly linked list** in recency order makes "the oldest is the tail" O(1) and
  "move this to the front" O(1) — but finding a *specific key* in it is O(n).

### The idea: use both, pointing at the same nodes

Keep entries in a **doubly linked list** ordered by recency (most-recent at the
head, least-recent at the tail), **and** a **hash map** from key → the node that
holds it. Now:

- `get(key)`: map finds the node in O(1); move it to the head (O(1)); return value.
- `put(key,val)`: map check in O(1); on a new key, add a node at the head and to the
  map; if now over capacity, evict the **tail** (O(1)) — the map tells you nothing
  extra is needed because the tail node *is* the LRU.

Two structures, same nodes, each covering the other's blind spot. That cooperation
is the entire lesson.

---

## 2. Visualization

```
 capacity 2.  recency list (MRU ─────────────── LRU):

   head → [1|10] ⇄ [2|20] ← tail          hash map:  1 → node[1|10]
                                                      2 → node[2|20]

 get(1):  map finds node[1]; move it to front:
   head → [1|10] ⇄ [2|20] ← tail   (1 is now MRU; 2 is now the eviction victim)

 put(3,30):  new key, cache is full → evict tail (2), then insert 3 at head:
   head → [3|30] ⇄ [1|10] ← tail          hash map:  1 → node[1|10]
          (2 removed from BOTH list and map, then freed)         3 → node[3|30]
```

### The intrusive node — in two structures at once

```
   recency list:   ... ⇄  [ key | val ]  ⇄ ...     via prev / next
                              │  ▲
   hash bucket b:   buckets[b] ─┘  └── hnext ──▶ (next node colliding in bucket b)
```

**Draw a `get` (move-to-front) and a `put`-that-evicts, updating BOTH the list
pointers and the map, on paper.** Eviction touching both is where every bug hides.

---

## 3. Design

```c
typedef struct LRUNode {
    int key, value;
    struct LRUNode *prev, *next;   /* recency list */
    struct LRUNode *hnext;         /* hash bucket chain */
} LRUNode;

typedef struct {
    LRUNode **buckets; size_t num_buckets;
    LRUNode  *head, *tail;         /* MRU ... LRU */
    size_t    size, capacity;
} LRUCache;
```

### A note on the "real" design (be honest about the scaffold)

The textbook LRU is literally "compose a generic hash map (key → node pointer) with
a doubly linked list." Your Lesson 6 map stores `string → int` and your Lesson 3
list stores `int` — neither stores "pointer to a list node," so force-composing them
doesn't fit. Rather than hack around that, this scaffold is **self-contained**: it
builds its *own* small intrusive hash map and recency list, so you implement the
*coordination* cleanly. A production version would use a generic map holding node
pointers — which is exactly the "make Lesson 6 generic" stretch. The concept is
identical; only the plumbing differs.

### Design questions

1. **Why must the node be in both structures?** So that a hit found via the map can
   be repositioned in the list in O(1), and the tail found via the list can be
   removed from the map in O(1). If either lookup were O(n), the cache wouldn't be
   O(1). Trace why.
2. **`get` mutates.** Reading a key makes it most-recent, so `get` reorders the list
   (hence a non-const cache). Surprising, but essential to LRU semantics.
3. **Eviction removes from BOTH + frees once.** The tail node must be unlinked from
   the list, removed from its hash bucket, and freed — exactly once. Grab the tail
   pointer *before* unlinking. This is the #1 place to leak or double-free.
4. **Update vs. insert in `put`.** An existing key updates value and moves to front
   (no size change, no eviction). A new key inserts and may evict. Don't duplicate.
5. **Sizing the hash map.** For a fixed capacity you can size buckets once at
   construction (e.g., ≈ capacity); no resize needed since size never exceeds
   capacity. Why does that simplify things versus Lesson 6?

---

## 4. Milestones

Scaffold: [`src/lru_cache.c`](src/lru_cache.c) / [`src/lru_cache.h`](src/lru_cache.h).

```
[ ] M0  Build: header + compiling .c with TODO-asserts; make test aborts at lru_new.
        "unused" warnings for the static helpers are expected until M2/M3 wire them in.
[ ] M1  lru_new / lru_free (walk the recency list freeing every node — it reaches
        them all — then buckets, then handle). new-then-free: ZERO leaks.
[ ] M2  Recency-list primitives: list_unlink, list_push_front, move_to_front (your
        Lesson 3 skills). Test them in isolation if you like.
[ ] M3  Hash-map primitives: bucket_of, map_find, map_insert, map_remove (your
        Lesson 6 chaining, intrusive via hnext).
[ ] M4  lru_get: map_find → hit moves to front and returns value; miss returns 0.
[ ] M5  lru_put: update-existing OR insert-new; evict the tail when over capacity —
        removing it from BOTH structures and freeing it. The heart of the project.
[ ] M6  lru_size. Full LeetCode-146-style sequence passes.
[ ] M7  (Stretch) generic keys/values; an LFU cache (evict least-FREQUENTLY used —
        harder); a TTL/expiry policy; thread-safety discussion.
[ ] M8  Full suite green under ASan; valgrind ZERO leaks. Request review.
```

---

## 5. Testing

```bash
cd projects/15-lru-cache
make test
make && valgrind --leak-check=full --error-exitcode=1 ./out_test
```

Starter: [`tests/test_lru_cache.c`](tests/test_lru_cache.c) (mirrors LeetCode 146).

### Edge cases (test all)

- **Eviction picks the LRU:** after `get(1)`, a subsequent over-capacity `put` must
  evict 2 (now the oldest), not 1.
- **`get` refreshes recency:** touching a key saves it from the next eviction.
- **Update, don't duplicate:** `put` on an existing key changes the value, keeps size,
  moves it to front.
- **Capacity 1:** every `put` of a new key evicts the previous one.
- **Repeated same key:** no growth, no eviction.
- **Evict everything then refill** — structure recovers; valgrind clean.
- **`lru_free` on a fresh cache**, and after evictions — every node freed exactly once.

---

## 6. Complexity Analysis

Derive first.

| Operation | Time | Why |
|-----------|:----:|-----|
| `lru_get` | ? | ? |
| `lru_put` | ? | ? |
| eviction | ? | ? |

<details>
<summary>Answers.</summary>

- `get`: **O(1) average** — a hash lookup (O(1) avg) + a constant-work move-to-front.
- `put`: **O(1) average** — hash lookup + O(1) list insert + O(1) eviction.
- Eviction: **O(1)** — the tail *is* the LRU; unlink + map-remove + free, all
  constant. No search for "the oldest."
- Worst case is the hash map's O(n) (adversarial collisions) — same caveat as
  Lesson 6. Space: **O(capacity)**.
</details>

---

## 7. Real-World Usage

- **CPU & OS caches** — page replacement (approximated by LRU / clock algorithms).
- **Database buffer pools** — Postgres, MySQL keep hot pages with LRU-ish policies.
- **Application caches** — Redis (`maxmemory-policy allkeys-lru`), Memcached, HTTP
  and CDN caches.
- **Language stdlib** — Python's `functools.lru_cache`, Guava/Caffeine in Java.
- **Browsers** — the back/forward and resource caches.

Real systems often use *approximations* of exact LRU (segmented LRU, clock, LRU-K,
ARC) because perfect LRU's bookkeeping can be costly at scale — but they all start
from the idea you're building here.

---

## 8. LeetCode

**No solutions here.**

1. **[Medium] 146. LRU Cache** — literally this project. The canonical design
   question.
2. **[Hard] 460. LFU Cache** — evict least-*frequently* used; a natural, harder
   sequel (frequency buckets + recency within each).
3. **[Medium] 1472. Design Browser History** — a doubly linked list application.
4. **[Hard] 432. All O`one Data Structure** — map + doubly linked list of buckets.
5. **[Medium] 706. Design HashMap** — revisit the map underneath.

---

## Exercises

1. **`lru_contains(key)`** — membership *without* refreshing recency (a peek). Why
   is "look without touching" a meaningfully different operation for a cache?
2. **`lru_peek_lru()` / `lru_peek_mru()`** — report the current eviction victim and
   the freshest key, for observability.
3. **Hit/miss counters** — track and expose the hit rate; it's how you'd tune a real
   cache's capacity.

## Challenge problems

1. **LFU cache (LeetCode 460).** Evict the least-*frequently* used, breaking ties by
   recency. Requires per-frequency buckets, each an LRU list. Much subtler — a great
   test of everything you've built.
2. **Generic LRU.** Parameterize key/value types (void* + sizes, or a comparator/
   hash callback like `qsort`). Now it caches anything.
3. **TTL expiry.** Add per-entry time-to-live; expired entries are misses and get
   reclaimed. Combine recency eviction with time-based eviction.

## Stretch goals

- **Compose the *real* generic map.** Make Lesson 6 store `void*` values, then build
  the LRU by genuinely composing that map (key → node) with your Lesson 3 list —
  the production architecture.
- **Thread safety.** Discuss (and, if adventurous, implement) locking; note why a
  global lock kills scalability and how sharding/lock-striping helps.

## Common mistakes

- **Updating one structure but not the other.** Insert into the list but forget the
  map (or vice versa) → later lookups miss, or eviction dangles. Every mutation
  touches both.
- **Freeing the evicted node but leaving it in the map** (or list) → use-after-free
  on the next access. Remove from both, THEN free.
- **Evicting the head instead of the tail** — the tail is the LRU. Getting the ends
  backwards silently evicts the wrong entry.
- **`put` on an existing key inserting a duplicate node** — update in place and move
  to front instead.
- **Forgetting `get` reorders** — an LRU whose `get` doesn't refresh recency isn't an
  LRU; it'll evict entries you just used.

## Debugging tips

- Write `lru_debug_print` that walks the recency list head→tail printing `key:val`;
  seeing MRU→LRU order makes eviction bugs obvious.
- After each op, assert the invariant: list length == size == number of map entries.
  A mismatch localizes which structure you forgot to update.
- Test with capacity 1 and 2 first — eviction fires immediately and the whole
  lifecycle is visible in a few operations.
- valgrind after a sequence with several evictions; double-free/leak on the victim is
  the classic bug and it names the exact node.

## Frequently asked questions

**Q: Why exactly do we need BOTH a map and a list?**
A: The map answers "where is key k?" in O(1) but not "who's oldest?"; the list
answers "who's oldest?" (the tail) and "make k newest" (move to front) in O(1) but
not "where is k?". Each covers the other's blind spot, and because they point at the
*same* nodes, both stay O(1). Drop either and an operation becomes O(n).

**Q: Why is `get` allowed to mutate the cache?**
A: "Recently used" is the whole basis of eviction, and using an entry (reading it) is
exactly what should mark it recent. So `get` moves the entry to the front. A cache
whose reads didn't update recency would evict hot entries — defeating the point.

**Q: How is this different from LFU?**
A: LRU evicts by *recency* (time since last use); LFU evicts by *frequency* (how
often used). LFU handles "used a lot long ago" differently and needs frequency
counts plus recency tie-breaking — strictly more machinery. LRU is the simpler,
more common default; LFU (LeetCode 460) is the natural next challenge.

---

## Definition of done

- [ ] Milestones M1–M6 implemented (M7–M8 recommended; LFU is an excellent capstone-
      of-the-capstone).
- [ ] Full LeetCode-146-style sequence passes, plus every §5 edge case.
- [ ] **`make test` clean under ASan/UBSan; `valgrind` ZERO leaks** — every node
      freed exactly once, especially across evictions.
- [ ] §6 table filled from your reasoning; you can explain why both structures are
      required and why every operation is O(1).
- [ ] `PROGRESS.md` written. Code review requested.

→ Next: [`../16-segment-tree/`](../16-segment-tree/) — the last core lesson: answer
range queries (sum/min over `[l, r]`) and point updates both in O(log n), using a
tree stored in a flat array.

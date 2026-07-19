# Project 6 — HashMap (the crown jewel)

> **You are building:** the structure behind C++'s `unordered_map`, Java's
> `HashMap`, Python's `dict`, and JavaScript's objects — average-case **O(1)
> lookup by an arbitrary key.** Budget real time; this is the most important
> lesson in the course.
>
> **Builds on:** Lesson 1 (an array — of *buckets*) and Lesson 2 (linked-list
> *chaining* for collisions). Lessons 7 (HashSet) and 15 (LRU Cache) build directly
> on this.
>
> **The C leap:** three layers of memory (a bucket array, a chain of entries per
> bucket, a heap-copied key per entry) and a genuine **ownership contract** — the
> map owns copies of its keys. This is the lesson where you learn to free a
> non-trivial structure completely and correctly.
>
> **Mentor rule:** diagrams and questions, not code. This one rewards patience.

---

## 1. Motivation

### The problem

An array gives O(1) access — but only by an **integer index** in `0..n-1`. Real
programs look things up by *arbitrary* keys: a username → account, a word → count,
a URL → cached page. Scanning a list for the key is O(n). We want O(1) lookup by
*any* key.

### The idea

Two moves:
1. A **hash function** turns any key into an integer.
2. `integer % capacity` maps that into a slot in an array of **buckets**.

Now `put`/`get` compute the bucket directly — no scanning. The catch: two
different keys can hash to the same bucket (a **collision**), which is not a bug
but a certainty (pigeonhole). So each bucket holds a short **linked list** of
entries (**separate chaining**) — you already know how to build those.

### Keeping it O(1): load factor and resizing

If entries pile into few buckets, chains grow and lookup degrades toward O(n). The
fix: track the **load factor** = `size / capacity`. When it crosses ~0.75, **resize**
to a bigger array and **rehash** every entry. This keeps chains short (≈1 element
on average), so lookups stay O(1) amortized.

### The C-specific twist: who owns the key?

The caller hands you `const char *key`. If you store that pointer and the caller
later frees or reuses its buffer, your map now points at garbage. So the map must
store its **own heap copy** (`strdup`) and free it on `remove`/`free`. Values here
are `int` (nothing to own); if they were pointers to heap objects, the same
question would apply to them. Ownership isn't paperwork — it's the difference
between a correct map and a use-after-free.

---

## 2. Visualization

```
 hash("cat") = 6384967 ... % 8 = 0
 hash("dog") = 1170243 ... % 8 = 0   ← collision with cat → same bucket, chained
 hash("fox") =  998321 ... % 8 = 1

 buckets (capacity 8):
   [0] ●──▶ ["dog"|2|●]──▶ ["cat"|1| / ]     (newest pushed at head)
   [1] ●──▶ ["fox"|9| / ]
   [2] NULL
   [3] NULL
   ...
```

### `get("cat")`

```
 idx = hash("cat") % 8 = 0
 walk bucket[0]: "dog"? no.  "cat"? yes → return value 1
```

### Resize (capacity 8 → 16) rehashes everyone

```
 "cat" was in bucket 0 at cap 8 (6384967 % 8 == 0)
 at cap 16:  6384967 % 16 == 7   ← DIFFERENT bucket!
```

That's why you can't just copy buckets across on resize — the modulus changed, so
every entry must be re-placed. **Draw the collision + the chain + a resize on
paper.** If you can't, the code will confuse you.

---

## 3. Design

```c
typedef struct Entry {
    char         *key;    /* OWNED heap copy */
    hm_value      value;
    struct Entry *next;   /* chain within a bucket */
} Entry;

typedef struct {
    Entry  **buckets;     /* array of `capacity` chain-heads */
    size_t   capacity;
    size_t   size;
} HashMap;
```

### Design questions (the heart of the course)

1. **Why not raw byte-sum as the hash?** `"abc"` and `"cba"` would collide, and
   sequential keys (`k1, k2, k3…`) would cluster. A good hash (FNV-1a, djb2)
   *mixes* bits so similar keys scatter. Even spreading is what keeps chains ≈1
   long. Look up **FNV-1a**; it's ~4 lines.
2. **Why chaining and not one entry per slot?** Because collisions are guaranteed.
   Chaining absorbs them gracefully. (The alternative — **open addressing**, probing
   for the next empty slot — is a great stretch goal with different trade-offs.)
3. **Load factor threshold.** Why ~0.75? Too high → long chains, slow lookups. Too
   low → wasted memory and frequent resizes. 0.75 is the classic time/space
   compromise; derive why extremes hurt.
4. **Why must resize rehash, not copy?** Bucket = `hash % capacity`. Change
   capacity → change every bucket assignment. Copying buckets verbatim would put
   entries in wrong buckets and `get` would never find them.
5. **`get` returns a status + out-param, not the value.** Any `int` is a valid
   stored value, so there's no spare int to mean "absent." Returning `1/0` and
   writing `*out` cleanly separates "found" from "the value happened to be 0." This
   is the C fix for Java's ambiguous `null` return.
6. **Ownership.** `put` `strdup`s the key; `remove` and `free` must free those
   copies. Three allocation layers to release: keys, entries, bucket array.

> Don't code until you can explain, in a sentence each: why mixing matters, why
> chaining, why 0.75, why rehash, and who frees the keys.

---

## 4. Milestones

Scaffold: [`src/hashmap.c`](src/hashmap.c) / [`src/hashmap.h`](src/hashmap.h).

```
[ ] M0  Build: header + compiling .c with TODO-asserts; make test aborts at hm_new.
        Two "defined but not used" warnings (hm_hash, hm_strdup) are EXPECTED —
        they're static helpers nothing calls yet; they vanish at M2/M3.
[ ] M1  hm_strdup helper; hm_with_capacity / hm_new (calloc the buckets — why
        calloc?); hm_free (free keys, entries, bucket array, struct — all layers).
        new-then-free: ZERO leaks under ASan.
[ ] M2  hm_hash (FNV-1a) + bucket index. Print a few hashes; eyeball the spread.
[ ] M3  hm_put: insert new (strdup key, push on chain) AND update existing (strcmp
        match → overwrite value, size unchanged). No duplicate keys, ever.
[ ] M4  hm_get (out-param + status) / hm_contains.
[ ] M5  hm_remove: unlink from the chain, free key then entry, size--.
[ ] M6  hm_load_factor + hm_resize: when put pushes load factor past 0.75, double
        capacity and REHASH all entries (relink existing nodes; don't re-strdup).
        Test: insert 1000 keys into a capacity-4 map; all retrievable after resizes.
[ ] M7  hm_size (and confirm load factor observability).
[ ] M8  (Stretch) Open addressing (linear probing) as a second implementation;
        treeify a long chain (link to Lesson 8); generic values (void*).
[ ] M9  Full suite green under ASan; valgrind ZERO leaks. Request review.
```

---

## 5. Testing

```bash
cd projects/06-hashmap
make test
make && valgrind --leak-check=full --error-exitcode=1 ./out_test
```

Starter: [`tests/test_hashmap.c`](tests/test_hashmap.c).

### Edge cases (test all)

- **Update vs. insert:** `put("cat",1)` then `put("cat",2)` — size stays 1, value
  becomes 2. The #1 hashmap bug is duplicating instead of updating.
- **Collisions on purpose:** build with `hm_with_capacity(1)` so *every* key chains
  in one bucket — then get/remove/update must all still work. This tests your chain
  logic in isolation from hashing.
- **Remove the chain head vs. a middle entry vs. a tail** — three code paths.
- **Remove a missing key** — returns 0, no crash, no leak.
- **Rehash correctness:** 1000 keys through many resizes, every value intact.
- **Key ownership:** put a key from a *mutable* buffer, then overwrite the buffer,
  then get — the map must still find it (proves you copied, didn't alias).
- **`hm_free` on a fresh map**, and on a map after removing everything.

---

## 6. Complexity Analysis

Derive first.

| Operation | Average | Worst | Why worst? |
|-----------|:-------:|:-----:|-----------|
| `hm_put` | ? | ? | ? |
| `hm_get` / `hm_contains` | ? | ? | ? |
| `hm_remove` | ? | ? | ? |
| resize (amortized into put) | — | ? | ? |

<details>
<summary>Answers.</summary>

- `get`/`put`/`remove`: **O(1) average** — a hash + a walk of a chain that's ≈1
  long when the load factor is bounded.
- **O(n) worst case** — if every key hashes to one bucket (adversarial input or a
  terrible hash), the map degenerates to a single linked list. This is why hash
  quality and load-factor bounding matter, and why security-sensitive code uses
  *randomized* hashes to prevent deliberate collision attacks (HashDoS).
- `put` is **amortized O(1)** despite the occasional O(n) resize/rehash — same
  doubling argument as the dynamic array: rare O(n) grows paid for by many O(1)
  puts.
- Space: **O(n + capacity)**.
</details>

---

## 7. Real-World Usage

- **Every language's core map:** C++ `unordered_map`, Java `HashMap`, Python `dict`
  (which underlies objects, kwargs, modules), JS objects/`Map`, Go `map`, Rust
  `HashMap`. Python's `dict` is so central the interpreter is tuned around it.
- **Databases:** hash indexes, hash joins.
- **Caches:** the LRU cache (Lesson 15) is a hash map + doubly linked list.
- **Compilers/interpreters:** symbol tables mapping names → info.
- **Deduplication, counting, memoization, sets** (Lesson 7) — all hash maps
  underneath.

Java 8+ **treeifies** a bucket (converts a chain longer than 8 into a red-black
tree, Lesson 8) so even a bad-hash worst case is O(log n) not O(n) — a real-world
marriage of this lesson and the next.

---

## 8. LeetCode

**No solutions here.** The "just use a hashmap" insight is one of the highest-ROI
patterns in all of interviewing.

1. **[Easy] 1. Two Sum** — the canonical hashmap unlock (value → index).
2. **[Easy] 242. Valid Anagram** — counting with a map.
3. **[Easy] 217. Contains Duplicate** — a preview of HashSet.
4. **[Medium] 49. Group Anagrams** — map keyed by a derived signature.
5. **[Medium] 380. Insert Delete GetRandom O(1)** — hashmap + array combo.

---

## Exercises

1. **`hm_keys`** — return a freshly allocated array of all keys. Who frees it, and
   do the returned strings alias the map's copies or new copies? (Design decision.)
2. **`hm_get_or_default(m, key, fallback)`** — cleaner than checking the status
   everywhere. When is each style better?
3. **Histogram** — count word frequencies in a paragraph using your map. This is the
   "hello world" of hash maps and a great end-to-end test.

## Challenge problems

1. **Open addressing.** Reimplement with a single entry array and **linear probing**
   (on collision, try the next slot). No chains, better cache locality — but now
   deletion needs *tombstones*. Why? Implement and compare.
2. **Randomized hashing.** Seed your hash with a random value at construction to
   defend against HashDoS (adversarial keys all colliding). What does this cost?
3. **Shrink on remove.** Halve capacity when the load factor drops below ~0.2.
   Watch for thrashing (same lesson as the dynamic array's quarter-threshold).
4. **Generic keys.** Support keys that aren't strings via a caller-supplied
   `hash(key)` and `equals(a,b)` — exactly how `qsort` takes a comparator.

## Stretch goals

- **Treeify** a long chain into a balanced BST (after Lesson 8) — bound the worst
  case at O(log n).
- **Benchmark** your chaining map vs. an open-addressing version vs.
  `unordered_map` for 1M inserts + lookups. Where does the time go?

## Common mistakes

- **Duplicating on update.** `put` of an existing key must find-and-overwrite, not
  prepend a second entry. Test size after re-putting a key.
- **Freeing entries but not their keys** (or vice versa) — the classic three-layer
  leak. valgrind names the exact `strdup`.
- **Aliasing the caller's key** instead of copying — works in tests where the buffer
  outlives the map, dangles in real use. Always `strdup`.
- **Copying buckets on resize instead of rehashing** — entries land in wrong
  buckets; `get` silently fails to find them.
- **`hash % capacity` with capacity 0** — division by zero. Guard capacity ≥ 1.
- **Signed/overflow in the hash** — use `unsigned long long`; wrap-around is defined
  for unsigned and expected here.

## Debugging tips

- Write `hm_debug_print` that dumps each bucket and its chain, plus size/capacity/
  load factor. Seeing the distribution tells you instantly if your hash clusters.
- Test the chain logic with `hm_with_capacity(1)` (forces all collisions) *before*
  trusting your hash — it isolates the two concerns.
- valgrind after every milestone; the three-layer free is easy to get 90% right.
- If rehash "loses" entries, print size before/after — you're probably dropping the
  tail of a chain when relinking. Relink carefully (save `next` first, like L2).

## Frequently asked questions

**Q: Why is it O(1) *average* but O(n) *worst*?**
A: Average: a good hash spreads keys so each bucket holds ≈1 entry (given a bounded
load factor), so a lookup is a hash plus a tiny walk. Worst: if every key collides
into one bucket, you're walking a length-n chain — a plain linked list. Hash quality
+ resizing keep you in the average case.

**Q: Why must `equals` and `hashCode` agree (here: `strcmp` and `hm_hash`)?**
A: The map finds the bucket by hash, then confirms the key by equality. If two
"equal" keys hashed differently, you'd store duplicates and `get` might miss one. The
rule "equal keys must hash equal" is exactly what makes the two-step lookup sound.

**Q: Why resize at 0.75 and not, say, 0.95?**
A: At high load, collisions cascade and chains lengthen, so lookups slow toward
O(n). 0.75 keeps the average chain near 1 while not wasting too much memory. It's an
empirical sweet spot; measure it yourself by timing lookups at different loads.

**Q: Chaining vs. open addressing — which is "right"?**
A: Neither universally. Chaining is simple and degrades gracefully; open addressing
has better cache behavior and no per-node allocation but needs tombstones for
deletion and suffers more at high load. Real libraries pick based on their workload
(many use open addressing for speed). Build chaining first; try open addressing to
feel the trade.

---

## Definition of done

- [ ] Milestones M1–M7 implemented (M8–M9 recommended; open addressing is superb
      practice).
- [ ] Full suite passes, every §5 edge case — especially update-not-duplicate,
      forced collisions, and rehash correctness.
- [ ] **`make test` clean under ASan/UBSan; `valgrind` reports ZERO leaks** across
      all three memory layers.
- [ ] §6 table filled from your reasoning; you can explain average vs. worst, why
      rehash is required, and the load-factor trade-off out loud.
- [ ] `PROGRESS.md` written. Code review requested.

→ Next: [`../07-hashset/`](../07-hashset/) — a set is a map that only cares about
*presence*, not values. You'll build it in a few lines by composing this map, and
see how one good structure spawns another.

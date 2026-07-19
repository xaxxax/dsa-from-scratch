# Project 7 — HashSet (composed from your HashMap)

> **You are building:** a set — a collection of *distinct* members with O(1)
> average membership tests — the structure behind C++'s `unordered_set`, Java's
> `HashSet`, and Python's `set`.
>
> **Builds on:** Lesson 6. A set is a map that only cares about **presence**, not
> values. So you compose the map you just built and get a whole structure in a
> handful of lines.
>
> **The lesson:** how a single well-designed abstraction spawns others. The hard
> parts — hashing, chaining, resizing, key ownership — were solved in Lesson 6.
> Here you just *restrict and rename*.
>
> **Mentor rule:** short bodies; the value is in seeing the composition clearly.

---

## 1. Motivation

### The problem

Often you don't need to associate a value with a key — you just need to know
**"have I seen this before?"** Deduplicating a list, tracking visited nodes in a
graph traversal (Lessons 12–13), testing membership in a whitelist. You want O(1)
average `add`, `contains`, `remove`, with no duplicates.

### The idea

That is *exactly* a hash map with the values thrown away. Store each member as a
**key** with a throwaway value; "is it in the set?" becomes "does the map contain
the key?". So `HashSet` HAS-A `HashMap`, and every operation is a one-line
delegation. You inherit the map's average-O(1) performance, its collision handling,
its resizing, and — crucially — its **key ownership** (the map already `strdup`s and
frees keys, so the set needs no memory logic of its own).

### Why this matters

Recognizing "X is just Y with a restriction" is a core engineering skill. A set is
a map without values; a stack is an array touched only at the end; a queue is a
list used at both ends. Building on your own abstractions — instead of copy-pasting
their internals — is how real systems stay small and correct.

---

## 2. Visualization

```
   HashSet                         backing HashMap
  ┌─────────┐        add("cat")   [bucket] ─▶ ("cat" | PRESENT)
  │ backing ─┼──────────────────▶ [bucket] ─▶ ("dog" | PRESENT)
  └─────────┘                     [bucket] ─▶ ("fox" | PRESENT)

  contains("cat")  ==  hm_contains(backing, "cat")
  add("cat") again ==  hm_put(backing, "cat", PRESENT)  → updates, size unchanged
```

The set is a thin *view*: it publishes `add`/`contains`/`remove`/`size` and hides
the fact that a full key/value map is doing the work.

---

## 3. Design

```c
typedef struct {
    HashMap *backing;   /* HAS-A: members are keys; values are a sentinel */
} HashSet;
```

### Design questions

1. **Why is `add` idempotent for free?** Because `hm_put` *updates* an existing key
   rather than duplicating it. Adding `"cat"` twice touches the same entry, so
   `size` stays correct with zero extra code. Confirm you understand *why* that
   falls out of Lesson 6's put.
2. **What value do you store?** Anything — a constant sentinel (`1`). The set never
   reads it. (If you later wanted a *multiset*/bag that counts occurrences, the
   value becomes the count — a nice generalization.)
3. **Do you need any memory management here?** No — and that's the point. The map
   owns the key copies; `hs_free` just calls `hm_free`. Composition means you
   inherit correct ownership, not re-implement it.
4. **Could you instead have copy-pasted the map's guts and deleted the value
   field?** You could — and you'd now maintain two copies of the hard code. Why is
   composition strictly better here?

---

## 4. Milestones

Scaffold: [`src/hashset.c`](src/hashset.c) / [`src/hashset.h`](src/hashset.h). The
[`Makefile`](Makefile) compiles your Lesson 6 `hashmap.c` alongside this.

```
[ ] M0  Build: compiles both modules; make test aborts at hs_new.
[ ] M1  hs_new (hm_new the backing) / hs_free (hm_free then free struct).
[ ] M2  hs_add (hm_put with sentinel — idempotent) / hs_contains.
[ ] M3  hs_remove / hs_size.
[ ] M4  (Stretch) Set algebra: hs_union, hs_intersection, hs_difference over two
        sets. Decide whether they build a new set or mutate one in place.
[ ] M5  Full suite green under ASan; valgrind clean. Request review.
```

---

## 5. Testing

```bash
cd projects/07-hashset
make test
make && valgrind --leak-check=full --error-exitcode=1 ./out_test
```

Starter: [`tests/test_hashset.c`](tests/test_hashset.c). Aborts inside
`hashmap.c` ⇒ finish Lesson 6.

### Edge cases

- **Idempotent add:** add the same member 3×, size stays 1.
- **Remove a missing member** — returns 0, no crash.
- **Scale + resize:** 1000 distinct members all present, size == 1000 (forces the
  backing map to rehash).
- **Bulk duplicates:** add 1000 copies of 10 members → size 10.
- **`hs_free` on a fresh set** — zero leaks.

---

## 6. Complexity Analysis

The set inherits the map's characteristics exactly.

| Operation | Average | Worst |
|-----------|:-------:|:-----:|
| `add` | ? | ? |
| `contains` | ? | ? |
| `remove` | ? | ? |

<details>
<summary>Answers.</summary>

All **O(1) average, O(n) worst** — identical to the hash map, because each is a
single delegating call. `add` is amortized O(1) (the backing map may resize).
Space **O(n)**. Composition composes complexity, too.
</details>

---

## 7. Real-World Usage

- **Deduplication** — unique visitors, unique tokens, `SELECT DISTINCT`.
- **Visited-sets in graph traversal** (Lessons 12–13) — the single most common use:
  "have I already explored this node?"
- **Fast membership** — allow/deny lists, feature flags, stop-word filters.
- **Set algebra** — union/intersection/difference power tag systems, permissions,
  recommendation overlaps.
- Every language ships one: `unordered_set`, `HashSet`, Python `set` (with literal
  `{...}` syntax and `|`, `&`, `-` operators mapping to set algebra).

---

## 8. LeetCode

**No solutions here.**

1. **[Easy] 217. Contains Duplicate** — the set in one line.
2. **[Easy] 349. Intersection of Two Arrays** — set intersection.
3. **[Easy] 202. Happy Number** — a set to detect a cycle.
4. **[Medium] 128. Longest Consecutive Sequence** — a set enables O(n) instead of
   O(n log n); a beautiful "why a set" problem.
5. **[Medium] 36. Valid Sudoku** — sets for row/col/box membership.

---

## Exercises

1. **`hs_union(a, b)`** returning a new set with all members of both.
2. **`hs_intersection(a, b)`** — members in both. Iterate the smaller; why?
3. **`hs_difference(a, b)`** — in `a` but not `b`.
4. **`hs_to_array`** — snapshot the members into a fresh array.

## Challenge problems

1. **Multiset / bag.** Let the backing value be a *count*. `add` increments,
   `remove` decrements (and deletes at zero). Now `size` vs. distinct-count differ —
   expose both.
2. **Set from a stream** — feed a huge file of tokens; report the number of distinct
   tokens using bounded memory relative to distinct count.
3. **Bloom filter (advanced).** A probabilistic set: O(1) space per element far
   below storing keys, at the cost of false positives. Implement one and measure the
   false-positive rate vs. bits-per-element. A great bridge to real systems.

## Stretch goals

- **Generic members** once your Lesson 6 map supports generic keys.
- **Immutable/persistent set** operations that return new sets without mutating
  inputs — a taste of functional data structures.

## Common mistakes

- **Reimplementing the map** instead of composing it — duplicated hard code, two
  places for bugs. Delegate.
- **Assuming add is not idempotent** — it is, via `hm_put`'s update. Don't add
  guard logic the map already provides.
- **Ownership confusion** — you do *not* free keys here; the map does. Just
  `hm_free` the backing.

## Debugging tips

- Almost every set bug is really a map bug — reproduce it against the map directly.
- Test idempotency and membership before set algebra; the algebra builds on them.

## Frequently asked questions

**Q: Isn't storing a dummy value wasteful?**
A: Marginally — one `int` per entry. In exchange you reuse a battle-tested map with
zero new memory logic. If the waste mattered, a dedicated open-addressing set with a
1-bit occupancy flag would be leaner — a fine stretch, but premature for learning.

**Q: Why do graph traversals need a set specifically?**
A: To answer "have I visited this node?" in O(1) so you don't loop forever on cycles
or redo work. You'll wire exactly this into DFS/BFS (Lessons 12–13) — the visited
set is what makes them terminate and stay linear.

**Q: When is a hash set the wrong choice?**
A: When you need ordering (use a tree/`std::set` for sorted iteration), range queries
(BST), or when members are few and comparison is cheap (a small sorted array can win
on cache). "Set" the concept has several implementations; the *hash* set is the O(1)
unordered one.

---

## Definition of done

- [ ] Milestones M1–M3 implemented (M4 set algebra recommended).
- [ ] Full suite passes, every §5 edge case (especially idempotency + scale).
- [ ] **`make test` clean under ASan/UBSan; `valgrind` zero leaks** (both modules).
- [ ] You can explain why `add` is idempotent for free and why composition beats
      copy-pasting the map.
- [ ] `PROGRESS.md` written. Code review requested.

→ Next: [`../08-binary-search-tree/`](../08-binary-search-tree/) — leave hashing
behind for *ordering*. A BST keeps elements sorted, giving O(log n) search plus
something a hash map can't: in-order traversal and range queries.

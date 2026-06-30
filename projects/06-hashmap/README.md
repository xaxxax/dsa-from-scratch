# Project 6 — HashMap

> **Status:** lesson outline (full eight-section treatment follows the Project 1
> template). This is the **crown jewel** of the course — budget two weeks.

**Builds on:** Project 1 (array of buckets) and Project 2 (chaining with linked
lists). Project 7 (HashSet) and Project 15 (LRU Cache) build directly on it.

## The core question
Arrays give O(1) access *by integer index*. But we usually want to look things up
**by an arbitrary key** — a username, a word, an object. How do we get O(1)
average lookup by *any* key, not just `0..n-1`?

## The idea
A **hash function** turns a key into an integer; we map that integer into an array
of **buckets** (`hash(key) % capacity`). Different keys can land in the same
bucket (**collision**) — so each bucket holds a small list of entries (**separate
chaining**). When the table gets too full (**load factor** exceeds a threshold),
we **resize** and **rehash** everything into a bigger table.

```
buckets (capacity 8):
 0: ─▶ (key=cat, val=1) ─▶ (key=dog, val=2)     ← two keys collided here
 1: ─▶ (key=fox, val=9)
 2:    (empty)
 ...
hash("cat") = 348721  →  348721 % 8 = 0  → bucket 0
```

## What you'll implement
- An `Entry<K,V>` node (key, value, next) — chaining via your own list logic.
- `Map<K,V>` interface; `HashMap<K,V>` with `put`, `get`, `remove`, `containsKey`,
  `size`.
- A hash spreader (mix the bits of `key.hashCode()` — why isn't raw `hashCode()`
  enough?).
- **Load factor** tracking and **resize/rehash** when it's exceeded.

## Key questions (the heart of the course)
- Why is lookup O(1) *average* but O(n) *worst case*? When does it degrade?
- Why must `equals` and `hashCode` agree? What breaks if they don't?
- Why resize at a load factor of ~0.75? What's the time/space trade-off?
- Why must resizing **rehash** every entry rather than just copying buckets?
- How does Java 8+ **treeify** a bucket (convert a long chain to a red-black tree)
  and why? (Connects to Project 8 and the bonus shelf.)

## LeetCode (in order)
1. [Easy] 1. Two Sum (the canonical "use a hashmap" insight)
2. [Easy] 242. Valid Anagram (counting with a map)
3. [Easy] 217. Contains Duplicate (preview of HashSet)
4. [Medium] 49. Group Anagrams (map keyed by a derived signature)
5. [Medium] 380. Insert Delete GetRandom O(1) (hashmap + array combo)

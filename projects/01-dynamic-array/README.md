# Project 1 — Dynamic Array (your own `ArrayList`)

> **You are building:** a growable, generic, array-backed list — the structure
> Java calls `ArrayList`, Python calls `list`, C++ calls `std::vector`, and that
> sits underneath an enormous fraction of all software ever written.
>
> **Mentor rule:** this README guides you to *discover* the implementation. It
> contains diagrams, designs, and questions — **not** the code. Write every line
> yourself. If you're stuck, ask for a *hint*, not the answer.

---

## 1. Motivation

### The real-world problem

A plain array is **fixed size**. The moment you write `new int[10]`, you've
committed to exactly ten slots — forever. But real programs almost never know
how many items they'll need in advance:

- A web server collecting incoming requests until a batch is ready.
- A text editor holding however many lines the user types.
- A game tracking however many entities currently exist on screen.

You need a structure that **looks and feels like an array** — fast indexed
access, items in a row — but **grows on demand**.

### Why wasn't another structure sufficient?

Couldn't we just use a linked list (Project 2), which grows trivially? We could —
but we'd lose the array's superpower: **O(1) random access.** With a linked list,
getting the 500th element means walking 500 pointers. With an array, `data[500]`
is a single address calculation. A huge amount of code does indexed access in
tight loops, so we *really* want to keep O(1) `get`.

So the design question becomes: **how do we keep array-speed access while
allowing growth?**

### The trade-off being made

The trick: keep a backing array that is usually **bigger than needed**, and track
how much is actually used (`size`) separately from how much exists (`capacity`).
When the used portion fills up, allocate a *bigger* array and copy everything
over. That copy is expensive (O(n)) — but if we grow cleverly (by *doubling*), it
happens rarely enough that appends are **amortized O(1)**.

The price we pay: **wasted memory** (spare capacity) and **occasional expensive
appends** (the resize). The dynamic array is the canonical example of trading a
little memory and rare latency spikes for everyday speed and convenience.

---

## 2. Visualization

### Capacity vs. size

This is *the* mental model. Burn it in.

```
capacity = 8   (slots that physically exist)
size     = 5   (slots actually in use)

index:     0     1     2     3     4     5     6     7
        +-----+-----+-----+-----+-----+-----+-----+-----+
data:   |  A  |  B  |  C  |  D  |  E  |  -  |  -  |  -  |
        +-----+-----+-----+-----+-----+-----+-----+-----+
                                       ^
                                       size points HERE
                                       (the next free slot)
        \___________ used ___________/ \____ spare ____/
```

`size` is both "how many items I have" *and* "the index where the next `add`
goes." That dual meaning is worth pausing on.

### `add(F)` when there's room — O(1)

```
before:  | A | B | C | D | E | - | - | - |   size=5
add(F):  | A | B | C | D | E | F | - | - |   size=6
                             ^ wrote here, then size++
```

### `add(...)` when full — trigger a resize — O(n)

```
full:    | A | B | C | D | E | F | G | H |   size=8, capacity=8   ← no room!

step 1: allocate a new array, usually DOUBLE the capacity:
         | _ | _ | _ | _ | _ | _ | _ | _ | _ | _ | _ | _ | _ | _ | _ | _ |   capacity=16

step 2: copy all existing elements over:
         | A | B | C | D | E | F | G | H | _ | _ | _ | _ | _ | _ | _ | _ |

step 3: point `data` at the new array, then add the new element:
         | A | B | C | D | E | F | G | H | I | _ | ... |   size=9, capacity=16
```

### `remove(index)` — shift the tail left — O(n)

```
remove index 1 (B):
before:  | A | B | C | D | E | - | - | - |   size=5
                ^ remove this
              shift C,D,E one slot left:
after:   | A | C | D | E | - | - | - | - |   size=4
                          ^ old last slot — null it out (see FAQ on why)
```

**Draw all four of these on paper before you write any code.** If you can
reproduce them from memory, you understand the structure. If you can't, you're
about to write bugs.

---

## 3. Design

Decide this *yourself first*, then read on to compare.

### What classes need to exist?

- A `List<E>` **interface** — the contract (what a list *is*).
- A `DynamicArray<E>` **class** that `implements List<E>` — the implementation
  (how *this* list works). Separating them mirrors the real JDK and sets up a
  beautiful comparison when you build a linked list that implements the *same*
  interface in Project 2.

### What fields does `DynamicArray<E>` need? Why?

Ask yourself: what's the minimum state required to know everything about the
list at any moment?

<details>
<summary>Think first, then expand for the answer.</summary>

- `E[] data` — the backing array (the actual storage).
- `int size` — how many slots are currently *in use*. **Not** the same as
  `data.length`, which is the *capacity*. Conflating these two is the #1 bug in
  this project.

You do **not** need a separate `capacity` field — `data.length` already is the
capacity. Storing it twice means two sources of truth that can drift apart.
</details>

### What should the interface look like?

A reasonable starting contract (you can extend it):

```java
public interface List<E> {
    void    add(E item);          // append to the end
    void    add(int index, E item); // insert at a position (stretch)
    E       get(int index);
    E       set(int index, E item); // replace, return old value
    E       remove(int index);    // remove & return, shifting the tail
    int     size();
    boolean isEmpty();            // default method in terms of size()
    boolean contains(E item);     // linear scan
}
```

### Design questions to settle before coding

1. **Initial capacity.** What should an empty `DynamicArray()` allocate? Zero?
   One? Ten? (The real `ArrayList` lazily uses 10. There are trade-offs — think
   about a program that creates millions of empty lists.)
2. **Growth factor.** When you resize, how much bigger? Why is *doubling* (×2)
   the classic choice, and what would go wrong if you grew by a *fixed* +1 each
   time? (This is the crux of the complexity analysis — see §6.)
3. **Bounds checking.** Which methods need it, and what exception fits? Should
   `add` at the end ever be out of bounds? Should `add(index, item)` accept
   `index == size`?
4. **The generic array.** You can't write `new E[capacity]`. Recall the idiom
   from Phase 0. Where should that one `@SuppressWarnings("unchecked")` live so
   it's contained and explained?

> Do not move on until you can state, in one sentence each, *why* you chose your
> initial capacity and your growth factor.

---

## 4. Milestones

Implement these **in order**. Compile and run after every single one. Commit
after each (`feat(dynamic-array): ...`). Do not write `resize` before `add`
works for the easy case — build the skeleton, then handle the hard case.

```
[ ] M0  Project setup: List<E> interface + empty DynamicArray<E> skeleton
        that compiles. Stub methods can `throw new UnsupportedOperationException()`.

[ ] M1  Constructor(s): no-arg (default capacity) + DynamicArray(int capacity)
        that validates input. Allocate the backing array. size = 0.

[ ] M2  size() and isEmpty(). Trivial, but write tests now — they verify M1.

[ ] M3  add(E item) for the EASY case only (assume there's room). Write into
        data[size], then size++. Test that size grows and get works... but get
        doesn't exist yet, so:

[ ] M4  get(int index) with bounds checking. Now M3's tests can really pass.

[ ] M5  resize(): private helper that allocates a bigger array and copies.
        Then make add() call it when size == data.length. Test by adding past
        the initial capacity and confirming all elements survived.

[ ] M6  set(int index, E item): replace, return the old value, bounds-checked.

[ ] M7  remove(int index): shift the tail left, decrement size, null the old
        last slot, return the removed value. Bounds-checked.

[ ] M8  contains(E item): linear scan (null-safe equality — careful with .equals
        when item could be null).

[ ] M9  toString(): "[A, B, C]" formatting. Makes debugging far easier.

[ ] M10 (Stretch) add(int index, E item): insert in the middle, shifting right,
        resizing first if needed.

[ ] M11 Full test suite passing, including edge cases (see §5). Then request a
        code review.
```

**Why this order?** Each milestone is independently testable and depends only on
earlier ones. M3 before M5 means you get a *working* (if limited) list early,
then handle the hard part (growth) once the basics are proven. This is how
professionals build — vertical slices, not big-bang.

---

## 5. Testing

Copy [`../../docs/Assert.java`](../../docs/Assert.java) into this project's
`tests/` folder, then write `DynamicArrayTest.java` with a `main` that exercises
each milestone. **Write the tests as you go**, not at the end.

### Manual test cases & expected outputs

| # | Action | Expected |
|---|--------|----------|
| 1 | `new DynamicArray<Integer>()` then `size()` | `0`, `isEmpty()` → `true` |
| 2 | `add(10)`, `add(20)`, `add(30)`, `size()` | `3` |
| 3 | `get(0)`, `get(2)` | `10`, `30` |
| 4 | `set(1, 99)` returns, then `get(1)` | returns `20`, then `99` |
| 5 | `remove(0)` returns, then `get(0)`, `size()` | returns `10`, then `20`, `2` |
| 6 | `contains(30)`, `contains(12345)` | `true`, `false` |
| 7 | add 100 items to a default-capacity list | all retrievable; `size()` → `100` |
| 8 | `toString()` after `add(1),add(2)` | `"[1, 2]"` |

### Edge cases (these are where the bugs hide — test all of them)

- **Empty list:** `get(0)`, `remove(0)`, `set(0, x)` on a fresh list → all should
  throw `IndexOutOfBoundsException`, *not* return garbage or null silently.
- **Negative index:** `get(-1)` → throws.
- **Index == size:** `get(size)` → throws (off-by-one trap: `>=`, not `>`).
- **Resize boundary:** add *exactly* `capacity` items, then one more. Verify the
  resize fired and nothing was lost or duplicated.
- **Remove last element:** `remove(size-1)` — does your shift loop handle "no
  elements to shift" without running off the end?
- **Remove down to empty, then add again:** does the structure recover?
- **`null` elements:** can you `add(null)`? Does `contains(null)` work without a
  `NullPointerException`? (Decide your policy and test it.)
- **Constructor with capacity 0 or -1:** what happens? What *should*?

### How to run

```bash
cd projects/01-dynamic-array
javac -d out src/*.java tests/*.java
java  -cp out DynamicArrayTest
```

You should see a list of `PASS` lines and `ALL TESTS PASSED.` at the end.

---

## 6. Complexity Analysis

Fill in this table **yourself** before reading the notes below it. Derive, don't
recall.

| Operation | Time (worst) | Time (amortized/avg) | Space | Why |
|-----------|:------------:|:--------------------:|:-----:|-----|
| `get(i)` / `set(i, x)` | ? | ? | O(1) | ? |
| `add(item)` (append) | ? | ? | — | ? |
| `add(index, item)` (insert middle) | ? | ? | — | ? |
| `remove(index)` | ? | ? | — | ? |
| `contains(item)` | ? | ? | — | ? |
| whole structure | — | — | ? | ? |

<details>
<summary>Answers + the all-important amortization argument.</summary>

- `get`/`set`: **O(1)** worst case — a single address calculation `base + i*size`.
  This is the array's reason to exist.
- `add` (append): **O(n) worst case** (when a resize fires and we copy everything)
  but **O(1) amortized.** This is the headline result — see the proof below.
- `add(index, ...)`: **O(n)** — must shift up to `n` elements right.
- `remove(index)`: **O(n)** — must shift up to `n` elements left.
- `contains`: **O(n)** — linear scan; an array has no faster way to find a value.
- Space: **O(n)**, but with up to ~2× slack from spare capacity.

### Why append is amortized O(1) (the doubling argument)

Suppose we double capacity on each resize and add `n` items to an empty list.
Resizes happen at sizes 1, 2, 4, 8, ..., up to `n`. The *copy work* across all
resizes is:

```
1 + 2 + 4 + 8 + ... + n  ≈  2n     (a geometric series sums to ~2× its last term)
```

So `n` appends cost about `2n` total work → **~2 operations per append on
average → O(1) amortized.** The rare O(n) resizes are "paid for" by the many
cheap O(1) appends between them.

### Why doubling, not adding a constant?

If instead you grew by a *fixed* +1 (or +k) each time, you'd resize on *every*
add. Total copy work becomes `1 + 2 + 3 + ... + n ≈ n²/2` → **O(n) per append
amortized**, i.e. O(n²) to build the list. The geometric growth is precisely what
makes the series collapse to linear. *This is the single most important insight in
the whole project.*
</details>

---

## 7. Real-World Usage

The dynamic array is arguably the most-used data structure in existence. It's the
**default sequence type** almost everywhere:

- **Java Collections:** `java.util.ArrayList` is exactly this — backing `Object[]`,
  `size` field, grows by ~1.5× (not 2×; look up `grow()` in the JDK source and
  ask *why 1.5?*). `Vector` and `Stack` are older, synchronized cousins.
- **Python (CPython):** the built-in `list` is a dynamic array of pointers
  (`PyObject*`), with its own over-allocation growth pattern in `listobject.c`.
- **C++ STL:** `std::vector` — same idea, the textbook example of amortized
  growth. `push_back` is amortized O(1).
- **Go:** slices are a view over a growable backing array; `append` does exactly
  this doubling dance.
- **Operating systems & databases:** dynamic buffers for I/O, row batches, and
  page collections constantly use this pattern.
- **Game engines:** entity/component lists where the count changes every frame.
- **Compilers:** token streams and symbol tables often start as dynamic arrays.

When you next type `new ArrayList<>()` or `list.append(x)`, you'll know exactly
what's happening underneath — including *why* it occasionally pauses to resize.

---

## 8. LeetCode

Attempt these **in order**, easiest first. They reinforce array mechanics:
indexing, in-place shifting, two pointers, and the resize/shift thinking you just
built. **No solutions are provided here — that's the point.**

1. **[Easy] 1480. Running Sum of 1d Array** — warms up basic indexed iteration
   and building an output array. Confirms you're fluent with `arr[i]`.
2. **[Easy] 27. Remove Element** — *in-place* removal with a write pointer. This
   is literally the "shift the tail" logic from your `remove()`, generalized.
3. **[Easy] 26. Remove Duplicates from Sorted Array** — two-pointer in-place
   compaction. Deepens the "size vs. capacity" / read-vs-write-index intuition.
4. **[Medium] 88. Merge Sorted Array** — merge into a fixed backing array from
   the *back*. Forces you to think about capacity, indices, and shifting without
   overwriting. Directly exercises your array mental model.
5. **[Medium] 1209. Remove All Adjacent Duplicates II** — array-as-stack thinking;
   a bridge to Project 4 (Stack). Shows the dynamic array as a foundation for
   other structures.

For each: which property of the dynamic array does it lean on? Indexed access?
In-place shifting? Growth? Name it before you start coding.

---

## Exercises

1. **Capacity introspection.** Add a package-private `int capacity()` returning
   `data.length`. Write a test that adds 17 items to a list with initial capacity
   4 and asserts the capacity sequence is exactly `4 → 8 → 16 → 32`. This makes
   your growth strategy *observable*.
2. **`clear()`.** Reset to empty. Decide: do you shrink the backing array or keep
   it? What does `ArrayList.clear()` do? (Hint: it nulls the slots but keeps
   capacity. Why?)
3. **`indexOf(E item)`** returning the first index or `-1`. Reuse it inside
   `contains`.
4. **`toArray()`** returning a correctly-sized `Object[]` copy (never expose your
   internal backing array — why not?).

## Challenge problems

1. **Shrinking.** Make `remove` shrink the backing array when it becomes mostly
   empty (say, size < capacity/4). Why *quarter*, not *half*? (Half causes
   "thrashing" — repeated grow/shrink at the boundary. Work out a sequence of
   adds/removes that would thrash with a half-threshold and explain why quarter
   fixes it.)
4. **An `Iterator<E>`.** Implement `Iterable<E>` so your list works in a
   `for (E e : list)` loop. This previews how the JDK collections integrate with
   the language and sets up *fail-fast* iteration (next challenge).
3. **Fail-fast iteration.** Add a `modCount` that increments on structural change,
   and have your iterator throw `ConcurrentModificationException` if the list is
   modified mid-iteration. This is exactly what `ArrayList` does — find out why
   it matters.

## Stretch goals

- Benchmark your `add` against `java.util.ArrayList` for 1,000,000 appends. Are
  you in the same ballpark? Where does the time go?
- Implement growth by 1.5× (like the real `ArrayList`) and compare memory usage
  vs. 2× across a range of sizes. Why did the JDK pick 1.5?
- Make `remove` and `add(index, item)` use `System.arraycopy` instead of a manual
  loop. Measure the difference and explain *why* it's faster (it's a single bulk
  memory move, often vectorized/intrinsic).

## Common mistakes

- **Confusing `size` with `data.length`.** Iterating to `data.length` instead of
  `size` exposes null/garbage slots. Almost every bug in this project is a version
  of this.
- **Off-by-one in bounds checks.** Valid indices are `0 .. size-1`. The guard is
  `index < 0 || index >= size`. Using `> size` lets one bad index slip through.
- **Forgetting to null the vacated slot in `remove`.** The element is still
  referenced by the old slot → a subtle **memory leak** (the object can't be
  garbage-collected even though it's "gone"). See the FAQ.
- **Shifting the wrong direction or wrong range in `remove`/insert.** Draw the
  arrows first; loop bounds follow the picture.
- **Resizing *after* writing out of bounds** instead of *before*. Check capacity
  first, grow if needed, *then* write.
- **Returning the internal `data` array** from `toArray()`, letting callers mutate
  your internals. Always return a copy.

## Debugging tips

- Implement `toString()` *early* (M9 — or sooner). Printing `[A, B, C]` after
  each operation makes every bug visible.
- Print `size` and `data.length` together when a resize-related test fails — the
  relationship between them tells the whole story.
- Reproduce any failure with the **smallest** input that triggers it (capacity 1
  or 2 makes resize bugs jump out immediately).
- If `contains`/`indexOf` misbehaves with objects, you're probably using `==`
  (reference identity) instead of `.equals` (value equality), or not handling
  `null`.

## Frequently asked questions

**Q: Why can't I just write `new E[10]`?**
A: Generic type information is *erased* at runtime (type erasure), so the JVM
doesn't know what `E` is to create an array of it. The accepted idiom is
`(E[]) new Object[10]` with a contained `@SuppressWarnings("unchecked")`. The
real `ArrayList` sidesteps this by declaring its field as `Object[]` and casting
on the way *out* in `get`. Either approach is fine — know why both exist.

**Q: Why null out the slot in `remove` instead of just decrementing `size`?**
A: If you only decrement `size`, the backing array still holds a reference to the
removed object. The garbage collector can't reclaim it, even though your list no
longer "contains" it — a quiet memory leak. Setting `data[size] = null` releases
it. (Read `ArrayList.remove` in the JDK; it does exactly `elementData[--size] =
null`.)

**Q: Should the default capacity be 10, like Java?**
A: Java uses 10 (lazily — an empty `ArrayList` actually starts with a shared
empty array and allocates 10 on first `add`). There's no universally "right"
number; it's a guess balancing "don't resize immediately for typical use" against
"don't waste memory on tiny/empty lists." Pick a value, *justify it*, and note
the trade-off.

**Q: Is amortized O(1) the same as O(1)?**
A: No. A single `add` can be O(n) (when it resizes). "Amortized O(1)" means that
*across a long sequence* of adds, the average cost per add is O(1). For
latency-sensitive systems the occasional O(n) spike can matter — which is exactly
why some systems pre-size their lists or use structures with O(1) *worst-case*
appends.

**Q: When would I *not* want a dynamic array?**
A: When you do lots of insertion/deletion in the *middle* (O(n) shifting — a
linked list may be better), or when you need fast lookup *by value* rather than by
index (a hash map), or when you need elements kept in sorted order with fast
insert (a balanced tree or heap). Recognizing this is the entire point of building
the rest of the course.

---

## Definition of done

- [ ] All milestones M0–M9 implemented (M10–M11 optional but recommended).
- [ ] Full test suite passes, including every edge case in §5.
- [ ] You filled in the §6 complexity table *from your own reasoning* and can
      explain the doubling/amortization argument out loud.
- [ ] `PROGRESS.md` written (copy from [`../../docs/progress-template.md`](../../docs/progress-template.md)).
- [ ] You requested and received a code review.

→ When this is solid, continue to
[`../02-singly-linked-list/`](../02-singly-linked-list/), where you'll build a
list with the *opposite* trade-offs and implement the **same `List<E>`
interface** — making the comparison concrete.

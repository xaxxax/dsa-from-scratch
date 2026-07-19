# Project 1 — Dynamic Array (your own `vector`)

> **You are building:** a growable, array-backed list — the structure C++ calls
> `std::vector`, Java calls `ArrayList`, and Python calls `list`, and that sits
> underneath an enormous fraction of all software ever written.
>
> **You already built this in Java.** Now you rebuild it in C — and confront the
> three things the JVM was quietly doing for you: **allocating** the backing
> array, **growing** it, and **freeing** it. That last one didn't even exist in
> your Java version. It does now.
>
> **Mentor rule:** this README guides you to *discover* the implementation. It
> contains diagrams, designs, and questions — **not** the code. Write every line
> yourself. If you're stuck, ask for a *hint*, not the answer.

---

## 1. Motivation

### The real-world problem

A plain C array is **fixed size**. The moment you write `int a[10];` or
`malloc(10 * sizeof(int))`, you've committed to exactly ten slots. But real
programs rarely know how many items they'll need in advance — a server collecting
requests, an editor holding lines, a game tracking entities. You need something
that **looks and feels like an array** — fast indexed access — but **grows on
demand**.

### Why not a linked list?

A linked list (Project 2) grows trivially, but loses the array's superpower:
**O(1) random access.** Getting the 500th element of a linked list means walking
500 pointers; in an array, `data[500]` is a single address calculation. Tons of
code does indexed access in tight loops, so we really want to keep O(1) `get`.

### The trade-off

Keep a backing buffer that is usually **bigger than needed**, and track how much
is *used* (`size`) separately from how much *exists* (`capacity`). When the used
portion fills, allocate a **bigger** buffer and copy everything over. That copy is
O(n) — but if we grow by *doubling*, it happens rarely enough that appends are
**amortized O(1)**. The price: some wasted memory, and occasional expensive
appends.

### The C-specific twist

In Java, the backing array was garbage-collected the moment your list became
unreachable. In C, **you own it.** Your structure has a *lifecycle*: `da_new`
allocates, `da_free` releases. Forget `da_free` and you leak. That ownership isn't
a chore bolted on — it's a core part of designing a data structure, and learning
to think about it is half of what this whole course is for.

---

## 2. Visualization

### Capacity vs. size — *the* mental model

```
capacity = 8   (slots that physically exist — the malloc'd size)
size     = 5   (slots actually in use)

index:     0     1     2     3     4     5     6     7
        +-----+-----+-----+-----+-----+-----+-----+-----+
data ─► |  A  |  B  |  C  |  D  |  E  |  ?  |  ?  |  ?  |
        +-----+-----+-----+-----+-----+-----+-----+-----+
                                       ^
                                       size points HERE (next free slot)
        \___________ used ___________/ \____ spare ____/
```

`data` is a **pointer** into the heap. `size` is both "how many items I have" *and*
"the index where the next append goes." In C the spare slots hold **garbage**, not
a friendly `null` — reading them is undefined behavior, so `size` is the only thing
standing between you and nonsense.

### `da_add` when there's room — O(1)

```
before:  | A | B | C | D | E | ? | ? | ? |   size=5
add(F):  | A | B | C | D | E | F | ? | ? |   size=6
                             ^ wrote data[size], then size++
```

### `da_add` when full — grow — O(n)

```
full:    | A | B | C | D | E | F | G | H |   size=8, capacity=8   ← no room!

step 1: realloc the buffer to DOUBLE the capacity (it may MOVE in memory):
         | A | B | C | D | E | F | G | H | ? | ? | ? | ? | ? | ? | ? | ? |  capacity=16

step 2: data now points at the (possibly new) block; write the new element:
         | A | B | C | D | E | F | G | H | I | ? | ... |   size=9, capacity=16
```

Note the difference from Java: `realloc` copies the old bytes *for you* and may
return a **different address**. You must adopt whatever address it returns.

### `da_remove(index)` — shift the tail left — O(n)

```
remove index 1 (B):
before:  | A | B | C | D | E | ? | ? | ? |   size=5
                ^ remove this
              shift C,D,E one slot left:
after:   | A | C | D | E | ? | ? | ? | ? |   size=4
                          ^ old last slot — now "spare" (see FAQ: do we clear it?)
```

**Draw all four on paper before writing any code.** If you can reproduce them from
memory, you understand the structure. If you can't, you're about to write bugs.

---

## 3. Design

Decide this *yourself first*, then read on to compare.

### The element type

C has no generics. **Start concrete.** Use a `typedef` so the element type has one
name you can change in one place:

```c
typedef int da_elem;   /* the thing the array stores */
```

Building for `int` keeps the *mechanics* clear. Making it hold any type
(`void*` + element size + `memcpy`) is a **stretch goal** — see the end.

### The struct — what state is the minimum?

Ask: what's the least I must store to know everything about the list at any moment?

<details>
<summary>Think first, then expand.</summary>

```c
typedef struct {
    da_elem *data;      /* the heap buffer (the storage) */
    size_t   size;      /* elements in use */
    size_t   capacity;  /* elements allocated */
} DynamicArray;
```

Unlike Java (where `data.length` *was* the capacity for free), a raw C buffer has
**no length attached** — `malloc` gives you bytes and forgets. So here you *must*
store `capacity` yourself. Conflating `size` and `capacity` is the #1 bug in this
project, exactly as in Java.
</details>

### The lifecycle — new in C

Java had a constructor and no destructor. C has both, by hand:

```c
DynamicArray *da_new(void);                        // default capacity
DynamicArray *da_with_capacity(size_t capacity);   // caller-chosen capacity
void          da_free(DynamicArray *a);            // release EVERYTHING
```

`da_free` must free **two** allocations: the backing buffer *and* the struct
itself. Getting this right (and in the right order) is the memory lesson.

### Design questions to settle before coding

1. **Handle on the heap or the stack?** We return `DynamicArray *` from `da_new`
   (the struct itself is heap-allocated). The alternative is `da_init(DynamicArray
   *a)` where the caller owns a stack struct. Both are idiomatic — know why you
   picked one. (We use the heap handle; it makes ownership one clear thing to pass
   around and free.)
2. **`size_t` is unsigned.** Your Java bounds check was `index < 0 || index >=
   size`. With `size_t index`, **`index < 0` can never be true** — an unsigned type
   has no negatives; a "negative" index wraps to a gigantic positive number. So the
   check collapses to just `index >= size`. Internalize this; it's a classic C
   trap. (See Common Mistakes.)
3. **Growth factor.** When you grow, how much bigger? Why is *doubling* (×2) the
   classic choice, and what breaks if you grow by a fixed +1 each time? (This is
   the crux of §6.)
4. **How do errors signal?** Out-of-range `da_get` — `assert` (abort: "caller has a
   bug") or a return code + out-parameter (recoverable)? `malloc`/`realloc`
   failure — how do you surface that? Decide your policy and apply it consistently.
   There's no single right answer; there's a *justified* answer.

> Do not move on until you can state, in one sentence each, *why* you chose your
> default capacity, your growth factor, and your error-signaling strategy.

---

## 4. Milestones

Implement **in order**. Compile and run after every single one. Commit after each
(`feat(dynamic-array): ...`). A skeleton with signatures + TODOs is in
[`src/dynamic_array.c`](src/dynamic_array.c) / [`src/dynamic_array.h`](src/dynamic_array.h).

```
[ ] M0  Build setup: header with the struct + prototypes, and a .c whose
        functions all compile (the scaffold's TODO-asserts are fine). `make test`
        should build and then abort at the first TODO (da_new). The ONE expected
        warning is `da_grow defined but not used` — it's static and nothing calls
        it yet; it vanishes at M5 when da_add starts calling it. (A "defined but
        not used" warning is the compiler flagging dead code — a useful habit to
        notice.)

[ ] M1  da_new / da_with_capacity: malloc the struct, malloc the backing buffer,
        set size = 0. Validate capacity. da_free: free the buffer, then the struct.
        Run under ASan — new then immediately free must report ZERO leaks.

[ ] M2  da_size / da_capacity / da_is_empty. Trivial, but write tests now — they
        verify M1 and give you a clean/leak baseline.

[ ] M3  da_add for the EASY case only (assume there's room): data[size] = item,
        then size++. Test size growth... but get doesn't exist yet, so:

[ ] M4  da_get with bounds checking (remember: just `index >= size`). Now M3's
        tests can really pass.

[ ] M5  da_grow(): private helper using realloc to double capacity. Handle the
        "realloc may return NULL / may move" cases. Then make da_add call it when
        size == capacity. Test by adding well past the initial capacity and
        confirming every element survived — under ASan.

[ ] M6  da_set: replace at index, return the old value, bounds-checked.

[ ] M7  da_remove: shift the tail left, decrement size, return the removed value.
        Bounds-checked. (Decide: do you need to clear the vacated slot? See FAQ.)

[ ] M8  da_contains: linear scan comparing da_elem values.

[ ] M9  da_to_string OR a da_print helper: makes debugging far easier. (Returning
        a string means malloc'ing it — and now the CALLER must free it. Note that
        ownership question; it's real.)

[ ] M10 (Stretch) da_insert(index, item): shift right, growing first if needed.

[ ] M11 Full test suite passing under ASan, valgrind clean (no leaks), including
        every edge case in §5. Then request a code review.
```

**Why this order?** Each milestone is independently testable and depends only on
earlier ones. M3 before M5 gets you a *working* (if limited) list early, then you
handle growth once the basics are proven. This is how professionals build —
vertical slices, not big-bang.

---

## 5. Testing

`#include "test_utils.h"` from your test file (the `Makefile` adds `docs/` to the
include path with `-I`, so the short name resolves). Write tests **as you go**, one
milestone at a time. A starter file
with the rhythm is in [`tests/test_dynamic_array.c`](tests/test_dynamic_array.c).

Run everything with:

```bash
cd projects/01-dynamic-array
make test                 # builds with ASan+UBSan and runs the tests
```

Then, separately, the leak audit:

```bash
make            # build the binary
valgrind --leak-check=full --error-exitcode=1 ./out_test
```

### Manual test cases & expected outputs

| # | Action | Expected |
|---|--------|----------|
| 1 | `da_new()` then `da_size` | `0`, `da_is_empty` → true |
| 2 | `da_add(10/20/30)`, `da_size` | `3` |
| 3 | `da_get(0)`, `da_get(2)` | `10`, `30` |
| 4 | `da_set(1, 99)` returns, then `da_get(1)` | returns `20`, then `99` |
| 5 | `da_remove(0)` returns, then `da_get(0)`, `da_size` | `10`, then `20`, `2` |
| 6 | `da_contains(30)`, `da_contains(12345)` | true, false |
| 7 | add 100 items to a small-capacity list | all retrievable; `da_size` → `100` |
| 8 | `da_new` then `da_free` | **zero leaks under ASan/valgrind** |

### Edge cases (where the bugs hide — test all)

- **Empty list:** `da_get(a, 0)` on a fresh list. What's your policy — abort, or
  status code? Test whichever you chose.
- **Index == size:** `da_get(a, size)` must be rejected (off-by-one: `>= size`).
- **The unsigned trap:** what does your code do if someone passes `da_get(a,
  (size_t)-1)`? (That's a huge number, not "-1" — make sure it's rejected.)
- **Grow boundary:** add *exactly* `capacity` items, then one more. Verify the grow
  fired and nothing was lost or duplicated — under ASan.
- **Remove last element:** does your shift loop handle "nothing to shift" without
  running off the end?
- **Remove down to empty, then add again:** does the structure recover?
- **Lots of grows:** add 1000 items to a capacity-1 list; all survive; no leak.
- **Capacity 0 to the constructor:** what happens? What *should*?

---

## 6. Complexity Analysis

Fill this in **yourself** before reading the notes. Derive, don't recall.

| Operation | Time (worst) | Time (amortized/avg) | Space | Why |
|-----------|:------------:|:--------------------:|:-----:|-----|
| `da_get` / `da_set` | ? | ? | O(1) | ? |
| `da_add` (append) | ? | ? | — | ? |
| `da_insert` (middle) | ? | ? | — | ? |
| `da_remove` | ? | ? | — | ? |
| `da_contains` | ? | ? | — | ? |
| whole structure | — | — | ? | ? |

<details>
<summary>Answers + the all-important amortization argument.</summary>

- `da_get`/`da_set`: **O(1)** worst case — one address calculation `data + i`.
- `da_add`: **O(n) worst case** (a grow copies everything) but **O(1) amortized.**
- `da_insert`: **O(n)** — shift up to `n` elements right.
- `da_remove`: **O(n)** — shift up to `n` elements left.
- `da_contains`: **O(n)** — linear scan.
- Space: **O(n)**, with up to ~2× slack from spare capacity.

### Why append is amortized O(1) (the doubling argument)

Double capacity on each grow and add `n` items to an empty list. Grows happen at
sizes 1, 2, 4, 8, …, n. Total copy work across all grows:

```
1 + 2 + 4 + 8 + ... + n  ≈  2n     (a geometric series sums to ~2× its last term)
```

So `n` appends cost about `2n` total → **~2 ops per append → O(1) amortized.** The
rare O(n) grows are "paid for" by the many cheap O(1) appends between them.

### Why doubling, not +1?

Grow by a fixed +1 and you `realloc`+copy on *every* add. Total work becomes `1 + 2
+ 3 + ... + n ≈ n²/2` → **O(n) per append amortized**, i.e. O(n²) to build. Geometric
growth is precisely what collapses the series to linear. This is the single most
important insight in the project.

> Bonus C angle: `realloc` *sometimes* grows in place (if the heap has room right
> after your block), making a grow cheaper than a full copy. But you cannot rely on
> it — the worst case is still a full move, so the analysis stands.
</details>

---

## 7. Real-World Usage

The dynamic array is arguably the most-used data structure in existence — the
default sequence type almost everywhere:

- **C++ STL:** `std::vector` — the textbook amortized-growth example. `push_back`
  is amortized O(1); it does exactly this doubling dance in C++ terms.
- **Java:** `java.util.ArrayList` — backing `Object[]`, a `size` field, grows ~1.5×
  (not 2×; look up `grow()` and ask *why 1.5?*).
- **Python (CPython):** the built-in `list` is a dynamic array of `PyObject*`
  pointers with its own over-allocation pattern in `listobject.c` — itself written
  in **C**, doing what you're doing now.
- **Go:** slices are a view over a growable backing array; `append` doubles.
- **OS & databases:** dynamic buffers for I/O, row batches, page collections.
- **Game engines:** entity/component lists that change every frame.

When you next type `push_back`, `ArrayList`, or `list.append`, you'll know exactly
what's happening underneath — including *why* it occasionally pauses to grow, and
who owns the memory.

---

## 8. LeetCode

Attempt these **in order**, easiest first. They reinforce array mechanics:
indexing, in-place shifting, two pointers — the exact thinking you just built. **No
solutions here — that's the point.** (You can solve them in C to keep the pointer
practice going.)

1. **[Easy] 1480. Running Sum of 1d Array** — basic indexed iteration; confirms
   you're fluent with `arr[i]`.
2. **[Easy] 27. Remove Element** — *in-place* removal with a write pointer. Literally
   the "shift the tail" logic from your `da_remove`, generalized.
3. **[Easy] 26. Remove Duplicates from Sorted Array** — two-pointer in-place
   compaction. Deepens size-vs-capacity / read-vs-write-index intuition.
4. **[Medium] 88. Merge Sorted Array** — merge into a fixed buffer from the *back*.
   Forces thinking about capacity, indices, and shifting without overwriting.
5. **[Medium] 1209. Remove All Adjacent Duplicates in String II** — array-as-stack
   thinking; a bridge to Project 4 (Stack).

For each: which property of the dynamic array does it lean on — indexed access,
in-place shifting, growth? Name it before you start.

---

## Exercises

1. **`da_capacity` observability.** Write a test that adds 17 items to a
   capacity-4 list and asserts the capacity sequence is exactly `4 → 8 → 16 → 32`.
   This makes your growth strategy *visible*.
2. **`da_clear`.** Reset to empty. Decide: shrink the buffer or keep it? What does
   `std::vector::clear` do to capacity? (Keeps it. Why?)
3. **`da_index_of`** returning the first index or a "not found" sentinel — and how
   do you signal "not found" when the index type is unsigned? (A real C wrinkle:
   `size_t` has no −1... or does it? What does `(size_t)-1` / `SIZE_MAX` buy you,
   and what's the risk?) Reuse it inside `da_contains`.
4. **`da_copy`** returning a deep copy — a *new* independent buffer. Prove with a
   test that mutating the copy doesn't touch the original (and neither leaks).

## Challenge problems

1. **Shrinking.** Make `da_remove` shrink the buffer when it's mostly empty (say
   size < capacity/4). Why *quarter*, not *half*? (Half causes "thrashing" —
   repeated grow/shrink at the boundary. Construct an add/remove sequence that
   thrashes with a half-threshold and explain why quarter fixes it.)
2. **`memmove` for shifts.** Replace your manual shift loops in `da_remove` /
   `da_insert` with a single `memmove`. Why `memmove` and not `memcpy`? (The
   ranges *overlap*.) This is the C analog of Java's `System.arraycopy` — one bulk,
   often-vectorized memory move.
3. **An iterator, C-style.** Add `da_begin`/`da_end` returning pointers, so a caller
   can loop `for (da_elem *p = da_begin(a); p != da_end(a); p++)`. This previews how
   `std::vector` iterators work — they're just pointers.

## Stretch goals

- **Make it generic.** Store `void *data`, a `size_t elem_size`, and `memcpy`
  elements in/out — the way `qsort` works. Now it holds *any* type. Notice the new
  ownership question: if elements are themselves pointers, who frees *them*? (This
  is exactly what a "destructor callback" is for.)
- **Benchmark** `da_add` vs `std::vector::push_back` (or Java `ArrayList`) for 1M
  appends. Same ballpark? Where does the time go?
- **1.5× vs 2× growth.** Implement 1.5× and compare memory use across sizes. Why
  did the JDK pick 1.5?

## Common mistakes

- **Confusing `size` with `capacity`.** Iterating to `capacity` instead of `size`
  reads uninitialized garbage (and ASan will scream). Nearly every bug here is a
  version of this.
- **The unsigned bounds check.** `if (index < 0 || index >= size)` — the `index <
  0` half is *dead code* for `size_t` and lulls you into thinking you're safe. The
  real guard is `index >= size`; a caller's "−1" arrives as `SIZE_MAX` and must be
  caught by that.
- **Ignoring `realloc`'s return / assuming it stayed put.** `data = realloc(data,
  ...)` directly is a leak-on-failure trap (if it returns NULL you've lost the old
  pointer) *and* forgetting to reassign `data` leaves you pointing at freed memory.
  Use a temp, check for NULL, then adopt.
- **Forgetting `da_free`, or freeing in the wrong order.** Free the buffer, *then*
  the struct — once the struct is freed you can't reach `a->data`.
- **Double free / use-after-free.** Calling `da_free` twice, or using the list
  after freeing it. Null your pointers after freeing.
- **Growing *after* writing out of bounds** instead of *before*. Check capacity
  first, grow if needed, *then* write.

## Debugging tips

- Add a `da_print` early. Printing `[A, B, C]` after each op makes bugs visible.
- Print `size` and `capacity` together when a grow test fails — their relationship
  tells the whole story.
- **Read the sanitizer output.** ASan prints the *exact* line of a bad read/write
  and the allocation it belongs to. It's not noise — it's the answer.
- Reproduce failures with the **smallest** input (capacity 1 or 2 makes grow bugs
  jump out immediately).
- Run `valgrind --leak-check=full` when a test passes but you suspect a leak — it
  names the `malloc` whose `free` you forgot.

## Frequently asked questions

**Q: Why do I have to store `capacity` when Java gave me `data.length` for free?**
A: A Java array object carries its length. A C `malloc`'d block does not — `malloc`
returns a bare pointer and remembers the size only internally, unreachable to you.
So you must track `capacity` yourself. This is a good, concrete example of an
abstraction Java provided that C makes explicit.

**Q: In Java `remove` I nulled the vacated slot to avoid a memory leak. Do I here?**
A: It depends what you store. For `da_elem = int`, the slot just holds an int —
there's nothing to leak, so clearing it is optional (though clearing to 0 can make
debugging cleaner). But once your elements are *pointers to owned heap objects*
(the generic stretch goal), the ownership question returns with teeth: does the
array free removed elements, or hand them back to the caller? Decide and document.

**Q: `da_get` returns a value — how do I signal "out of range" without exceptions?**
A: Your choice, and it's a real design decision: (a) `assert(index < size)` — treat
it as a programmer bug and abort (mirrors Java's unchecked exception); (b) return a
status code and deliver the value via an out-parameter `int *out`. (a) keeps
signatures clean; (b) lets callers recover. Pick one and be consistent.

**Q: Is amortized O(1) the same as O(1)?**
A: No. A single `da_add` can be O(n) (when it grows). "Amortized O(1)" means that
*across a long sequence* of adds, the average cost per add is O(1). For
latency-sensitive systems the occasional O(n) spike matters — which is why some
code pre-sizes with `da_with_capacity` or reserves upfront.

**Q: When would I *not* want a dynamic array?**
A: Lots of insertion/deletion in the *middle* (O(n) shifting — a linked list may be
better), fast lookup *by value* (a hash map), or elements kept sorted with fast
insert (a balanced tree or heap). Recognizing this is the point of building the
rest of the course.

---

## Definition of done

- [ ] Milestones M0–M9 implemented (M10–M11 optional but recommended).
- [ ] Full test suite passes, including every edge case in §5.
- [ ] **`make test` is clean under ASan/UBSan and `valgrind` reports zero leaks.**
- [ ] You filled in the §6 complexity table *from your own reasoning* and can
      explain the doubling/amortization argument out loud.
- [ ] `PROGRESS.md` written (copy from [`../../docs/progress-template.md`](../../docs/progress-template.md)).
- [ ] You requested and received a code review.

→ When this is solid, continue to
[`../02-singly-linked-list/`](../02-singly-linked-list/), where you'll build a list
with the *opposite* trade-offs — and where the "arrows" in your diagrams become
real `struct node *` pointers you allocate and free one at a time.

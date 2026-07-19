# Phase 0 — C Refresher

> **Goal:** make sure the *language* never gets in the way of the *ideas*. The rest
> of this course assumes you can confidently work with pointers, the heap
> (`malloc`/`free`), structs, and the compile/link model. This phase is a
> checklist, not a tutorial — for each topic you should be able to write a tiny
> example from scratch. If one feels shaky, that's exactly what to drill before
> Project 1.

> **If you already built the dynamic array in Java** (you did): good. You know the
> *algorithms*. This phase is about the *machine underneath them* — the part Java
> did for you. That's the whole reason we switched to C.

You'll write a little code here, but the point is *fluency*, not a finished
artifact. Treat each section as: "can I produce this without looking?"

---

## Why these specific topics?

Every data structure you build will be:

- **a `struct`** (`DynamicArray`) — so you need structs, fields, and typedef.
- **backed by heap memory** you allocate and grow — so you need
  `malloc`/`realloc`/`free` and the discipline that comes with them.
- **navigated by pointers** (a linked list *is* pointers) — so you need pointers,
  `*`, `&`, and pointer arithmetic.
- **defensive** (rejecting bad input) — so you need C's error idioms: return
  codes and `assert`.
- **compiled and linked** from headers + sources — so you need the build model.

Master these five and the language disappears, leaving only the CS.

---

## 1. Pointers — the whole game

A pointer is a variable whose value is a **memory address**. `&x` gives you the
address *of* `x`; `*p` gives you the value *at* the address `p` holds
("dereference").

```c
int  x = 42;
int *p = &x;      // p holds the address of x
printf("%d\n", *p);   // 42  — follow the pointer, read the value
*p = 99;              // write THROUGH the pointer...
printf("%d\n", x);    // 99  — ...and x changed
```

Why this matters everywhere: in C, functions receive **copies** of their
arguments. To let a function *modify* your variable — or to avoid copying a big
struct — you pass a pointer to it.

```c
void set_to_zero(int *n) { *n = 0; }   // takes the address, writes through it

int a = 5;
set_to_zero(&a);
// a is now 0
```

`NULL` is the pointer that points at nothing. Dereferencing it (`*p` when
`p == NULL`) crashes — deliberately. It's C's "there is no object here."

**Check yourself:**
- What's the difference between `p` (a pointer) and `*p` (what it points to)?
- Why does `set_to_zero(a)` (without `&`) fail to compile — and why is that a good
  thing?
- What happens if you dereference a pointer you never assigned? (Undefined
  behavior — it holds garbage. This is the #1 source of C crashes.)

---

## 2. The heap — `malloc`, `realloc`, `free`

Local variables live on the **stack** and vanish when the function returns. To
make memory that *outlives* the function — memory whose size you decide at
runtime, and that a data structure can own — you allocate it on the **heap**.

```c
#include <stdlib.h>   // malloc, realloc, free

int *nums = malloc(5 * sizeof(int));   // room for 5 ints; UNINITIALIZED
if (nums == NULL) { /* allocation failed — handle it */ }

nums[0] = 10;                          // use it like an array
nums[4] = 50;

int *bigger = realloc(nums, 10 * sizeof(int));  // grow it (may move!)
if (bigger == NULL) { /* realloc failed; nums is still valid */ }
nums = bigger;                          // adopt the (possibly new) address

free(nums);                             // give it back. exactly once.
nums = NULL;                            // optional but wise: no dangling pointer
```

Three rules that will save you weeks of pain:

1. **Every `malloc`/`realloc` has exactly one matching `free`.** No free = leak.
2. **`realloc` may return a *different* address** (it can move your data to fit
   the new size). Always write `p = realloc(p, ...)` via a temp and re-check —
   never assume the pointer stayed put.
3. **After `free(p)`, `p` is radioactive.** Reading it is *use-after-free*; freeing
   it again is *double-free*. Both are undefined behavior. Null it out.

`calloc(n, size)` is `malloc` that also **zeroes** the memory — handy when you want
a fresh block initialized to 0/NULL.

**Check yourself:**
- Where does a `malloc`'d block live, and when does it go away? (The heap; only
  when *you* `free` it.)
- Why must you check `malloc`'s return for `NULL`?
- What's a memory *leak*, and why doesn't the program crash when you have one?

---

## 3. Arrays & pointer arithmetic

An array is a contiguous block; its name decays to a pointer to its first element.
Indexing is *defined as* pointer arithmetic:

```c
int arr[5] = {10, 20, 30, 40, 50};
// arr[i] is EXACTLY *(arr + i)
printf("%d\n", arr[2]);      // 30
printf("%d\n", *(arr + 2));  // 30 — identical
```

`arr + 2` doesn't add 2 *bytes* — it adds 2 *elements* (the compiler scales by
`sizeof(int)`). This is why `arr[i]` is O(1): it's one multiply-and-add, a single
address calculation. It's also why reading `arr[5]` (past the end) doesn't throw —
C just computes an address and reads it. Garbage, or a crash, or *undefined
behavior*. There is no bounds check. Ever.

```c
// arr[5] = 1;   // no compile error, no runtime exception — silent corruption
```

**Check yourself:**
- Why is `arr[i]` O(1)? Spell out the address arithmetic.
- What does C do when you index out of bounds? (Nothing helpful — UB. Your job,
  and the sanitizer's, to catch it.)
- If `int *p = arr;`, what is `p + 1` — one byte later, or four?

---

## 4. Structs & typedef

A `struct` groups related fields into one type. It's how you'll bundle a data
structure's state (its buffer, its size, its capacity) into a single value.

```c
struct point { int x; int y; };

struct point a = {3, 4};
a.x = 10;                 // access a field with .

struct point *pp = &a;
pp->y = 20;               // through a pointer, use ->  (shorthand for (*pp).y)
```

`typedef` gives the type a shorter name so you can drop the `struct` keyword:

```c
typedef struct {
    int *data;
    size_t size;
    size_t capacity;
} DynamicArray;

DynamicArray v;           // no "struct" needed now
```

**Check yourself:**
- When do you use `.` vs `->`? (`.` on a struct value; `->` on a pointer to one.)
- Why pass big structs by pointer rather than by value?
- What's the difference between a `DynamicArray` (a value) and a `DynamicArray *`
  (a pointer to one)? Which does `da_new()` return, and why?

---

## 5. C's answers to generics and exceptions

C has neither. That's not a gap to mourn — it's two design decisions you now get
to make yourself.

### "Generics" → concrete first, then `void*`

There is no `List<E>`. You start with a **concrete element type** (we'll use `int`
via a `typedef` so it's easy to change), which keeps the mechanics crystal clear.
When you want a structure that holds *any* type, the C way is a `void*` (an
untyped pointer) plus a remembered element size, copying bytes with `memcpy` — the
exact trick `qsort` and `bsearch` use. We treat that as a *stretch goal*, not the
starting point.

### "Exceptions" → return codes or `assert`

There is no `throw`. When something goes wrong you choose one of:

```c
// (a) return a status code; deliver the real value through an out-parameter
int da_get(const DynamicArray *a, size_t i, int *out);  // returns 0 ok, non-0 error

// (b) assert: for PROGRAMMER errors (bugs), abort loudly and immediately
#include <assert.h>
assert(i < a->size);   // if false, prints file:line and aborts
```

Rough rule (you'll refine it per lesson): use `assert` for *"the caller has a bug"*
(an impossible index — analogous to Java's unchecked `IndexOutOfBoundsException`),
and return codes for *recoverable* conditions the caller should handle (like
`malloc` failing). Deciding which is which is real API design — each lesson asks
you to.

**Check yourself:**
- How would you let a function "return" both a success/failure flag *and* a value?
  (Return the flag; write the value through a pointer out-parameter.)
- What does `assert(cond)` do when `cond` is false? When would you *not* want it in
  shipping code? (It aborts; it's compiled out with `-DNDEBUG`.)

---

## 6. The build model (headers, sources, linking)

C has no `import`. You split declarations from definitions:

- A **header** `foo.h` declares *what exists*: struct definitions, function
  prototypes. Guard it against double-inclusion with `#ifndef/#define/#endif`.
- A **source** `foo.c` defines *how it works*: the function bodies. It `#include`s
  its own header.
- A test/main `.c` `#include`s the header to use the functions.
- The compiler turns each `.c` into an object file; the **linker** stitches them
  into one binary.

```c
/* dynamic_array.h */
#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H
typedef struct { int *data; size_t size; size_t capacity; } DynamicArray;
DynamicArray *da_new(void);        // prototype: promise it exists somewhere
#endif
```

```c
/* dynamic_array.c */
#include "dynamic_array.h"
DynamicArray *da_new(void) { /* ... the real body ... */ }
```

The project `Makefile` runs compile + link + run for you (`make test`). Read it
once — the whole model is only a few lines.

**Check yourself:**
- What goes in a `.h` vs a `.c`?
- What is an "undefined reference" linker error usually telling you? (You declared
  a function but never defined it — or forgot to compile the `.c` that has it.)
- What does the `#ifndef` include guard prevent?

---

## Warm-up exercise (write this yourself)

Build a **fixed-capacity** `IntBox` on the heap to exercise all five topics at
once. A scaffold with signatures and TODOs is in
[`src/int_box.c`](src/int_box.c) — fill in the bodies.

**Spec:**
- `IntBox *box_new(size_t capacity)` — `malloc` the struct *and* its backing array;
  return `NULL` (or `assert`) on capacity 0. Decide and document which.
- `void box_free(IntBox *b)` — free the backing array, then the struct. (Order
  matters — why?)
- `int box_push(IntBox *b, int value)` — append; return non-zero if full.
- `int box_get(const IntBox *b, size_t index)` — read; `assert` on out-of-range.
- `size_t box_size(const IntBox *b)` and `int box_is_full(const IntBox *b)`.

Write a tiny `main` that exercises every function, then build it with the
sanitizers on and confirm **no leaks**:

```bash
cc -std=c11 -Wall -Wextra -g -fsanitize=address,undefined src/int_box.c -o box
./box
```

> This is intentionally a miniature of Project 1 — but *fixed* size, so there's no
> `realloc` yet. If you can write `IntBox` and it comes back clean under the
> sanitizer, you're ready. If pointers, `malloc`/`free`, or the build tripped you
> up, that's the signal to re-read those sections first.

---

## Reflection questions

1. In Java, who freed your `DynamicArray`'s backing array when it went out of
   scope? In C, who does? What does that change about how you *design* a structure?
2. Why is `arr[i]` O(1) in both Java and C — and what safety did Java add on top
   that C leaves to you?
3. `realloc` can move your data to a new address. What kind of bug appears if some
   other pointer was still holding the *old* address? (This previews a real hazard
   in Project 1.)

---

## When you're ready

You're ready for **Project 1 — Dynamic Array** when you can, from a blank file:

- [ ] Declare a pointer, take an address with `&`, and dereference with `*`.
- [ ] `malloc` a block, use it, and `free` it exactly once — clean under ASan.
- [ ] `realloc` a block to grow it, handling the "it may move" case.
- [ ] Define a `struct` with `typedef` and access fields via `.` and `->`.
- [ ] Explain why `arr[i]` is `*(arr + i)` and why it's O(1).
- [ ] Compile and run a program with `cc` and understand what the linker did.

→ Continue to [`../01-dynamic-array/`](../01-dynamic-array/).

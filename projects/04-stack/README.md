# Project 4 — Stack (composed from your Dynamic Array)

> **You are building:** a LIFO stack — but the real lesson is **composition**. You
> won't write storage code; you'll *reuse* your Lesson 1 dynamic array and expose
> a deliberately tiny interface on top of it.
>
> **Builds on:** Lesson 1. Your `DynamicArray` is the backing store. This is your
> first multi-module C build: the stack's source and the array's source compile
> and link **together** (see the Makefile).
>
> **The C leap:** two structs, two allocations, two owners. The stack owns the
> array; when the stack is freed it must free the array first. And you'll `#include`
> a header from *another project directory* — a real taste of how C programs are
> assembled from separately-compiled units.
>
> **Mentor rule:** the bodies are short (delegation), so the thinking is in the
> *design questions*, not the typing. Answer them.

---

## 1. Motivation

### The problem

Countless problems need exactly one discipline: **last in, first out**. The
program call stack, undo/redo, bracket matching, expression evaluation,
backtracking (mazes, DFS). Each wants to add and remove at the *same* end and
never touch the middle.

### The idea

You already have a structure that's O(1) at one end: the dynamic array (append and
remove-last are cheap). A stack is that structure with a **restricted interface** —
only `push`, `pop`, `peek`, `is_empty`, `size`. Map "top of stack" to "end of
array" and you're done.

### Why restriction is a feature

By *hiding* `da_get(i)` and `da_insert(i, ...)`, the stack makes an entire class of
bugs unrepresentable — nobody can reach into the middle. A smaller interface is
easier to reason about, test, and use correctly. "Make illegal states
unrepresentable" is one of the most valuable ideas in software design, and a stack
is its simplest instance.

### The C-specific twist: composition without inheritance

In an OO language you might be tempted to `extends DynamicArray` — inheriting
(and exposing!) all its methods, which *breaks* the restriction. C has no
inheritance, which turns out to be a gift: the only tool is **composition** (HAS-A,
a `DynamicArray *` field), and composition is exactly what you want here. You
expose only what you choose. This is why "favor composition over inheritance" is
evergreen advice — C forces the good habit.

---

## 2. Visualization

```
 A stack is a VIEW of the array's tail end:

   backing array:  index 0   1   2   3
                  [  A  ][  B  ][  C  ][  D  ]
                    ^bottom              ^top == last index (size-1)

   push(E): da_add(E)              → [A][B][C][D][E]     top = E
   peek():  da_get(size-1)         → E                    (no change)
   pop():   da_remove(size-1) → E  → [A][B][C][D]         top = D
```

`push`/`pop`/`peek` all touch index `size-1`. Nothing shifts, because it's the
last slot — that's why the array's *end* (not front) is the stack's top.

---

## 3. Design

```c
typedef struct {
    DynamicArray *backing;   /* HAS-A */
} Stack;
```

### Design questions (this is where the learning is)

1. **Why compose, not inherit?** If `Stack` exposed the array's full API, callers
   could `da_insert(stack->backing, 2, x)` and violate LIFO. Composition lets you
   publish *only* push/pop/peek. State this in your own words.
2. **Why the end, not the front?** Popping the front of an array is O(n) (shift all
   remaining left). The end is O(1). Confirm: what would a "front-is-top" stack cost
   per operation, and why?
3. **Empty policy.** `pop`/`peek` on an empty stack — abort via `assert` (caller
   bug, like Java's `EmptyStackException`) or return a status? Note the C hazard:
   computing `size - 1` on an empty stack underflows to `SIZE_MAX` (the unsigned
   trap again) — you must check emptiness *before* that subtraction.
4. **Ownership.** The stack allocates its backing array in `stack_new` and must
   `da_free` it in `stack_free` *before* freeing the `Stack` struct. Two
   allocations, freed in the right order.

---

## 4. Milestones

Scaffold: [`src/stack.c`](src/stack.c) / [`src/stack.h`](src/stack.h). The
[`Makefile`](Makefile) compiles your Lesson 1 `dynamic_array.c` alongside this.

```
[ ] M0  Build: header + compiling .c with TODO-asserts. `make test` builds BOTH
        modules and aborts at the first TODO. (If it fails to *compile* pointing at
        dynamic_array.c, your Lesson 1 header/signatures drifted — fix that first.)
[ ] M1  stack_new (da_new the backing) / stack_free (da_free then free struct).
        new-then-free: ZERO leaks under ASan.
[ ] M2  stack_size / stack_is_empty — delegate to the array.
[ ] M3  stack_push — da_add (append == push onto the top).
[ ] M4  stack_peek — da_get(size-1); guard empty FIRST (unsigned trap).
[ ] M5  stack_pop — da_remove(size-1) returns the value; O(1), no shift.
[ ] M6  (Stretch) A Min-Stack: track the current minimum in O(1) (LeetCode 155) —
        push a paired (value, min-so-far). What extra state, and where?
[ ] M7  Full suite green under ASan; valgrind clean. Request review.
```

---

## 5. Testing

```bash
cd projects/04-stack
make test
make && valgrind --leak-check=full --error-exitcode=1 ./out_test
```

Starter: [`tests/test_stack.c`](tests/test_stack.c). If a test aborts inside
`dynamic_array.c`, that's the dependency talking — finish Lesson 1.

### Edge cases

- **Pop/peek an empty stack** — exercise whatever policy you chose.
- **Push past the backing array's initial capacity** (100+ items) — LIFO order must
  survive the array's internal grow.
- **Empty, then push again** — the stack must be reusable.
- **`stack_free` on a fresh stack** — zero leaks, no crash.

---

## 6. Complexity Analysis

| Operation | Time | Why |
|-----------|:----:|-----|
| `push` | ? | ? (inherits the array's append cost) |
| `pop` | ? | ? |
| `peek` | ? | ? |
| `is_empty` / `size` | ? | ? |

<details>
<summary>Answers.</summary>

- `push`: **amortized O(1)** — it's `da_add`, which is O(1) amortized (O(n) only on
  the rare grow).
- `pop`, `peek`, `is_empty`, `size`: **O(1) worst case** — they touch the last index
  or a counter; nothing shifts. Space: **O(n)** (plus the array's slack).

Notice the stack *inherits* the array's complexity — composition means you also
compose the performance characteristics. If you'd backed it with a linked list
instead, push/pop would be O(1) *worst case* (no amortized grow), at the cost of
per-node allocation. Both are valid; know the trade.
</details>

---

## 7. Real-World Usage

- **The call stack itself** — every function call pushes a frame (locals, return
  address); every return pops it. Stack overflow is literally this structure
  overrunning its bounds. You're modeling the machine.
- **Undo/redo**, browser back button, editor bracket matching.
- **Expression evaluation** — compilers/calculators use stacks for operators and
  operands (RPN).
- **Backtracking & DFS (Lesson 12)** — the recursion stack *is* a stack; an explicit
  stack lets you do DFS iteratively.
- **Virtual machines** — the JVM and CPython are *stack machines*: bytecodes push
  and pop operands.

---

## 8. LeetCode

**No solutions here.** The canonical stack set:

1. **[Easy] 20. Valid Parentheses** — the textbook stack problem.
2. **[Easy] 155. Min Stack** — augment to report the min in O(1) (see M6).
3. **[Medium] 150. Evaluate Reverse Polish Notation** — a stack machine in miniature.
4. **[Medium] 739. Daily Temperatures** — *monotonic stack* (keep it sorted).
5. **[Hard] 84. Largest Rectangle in Histogram** — advanced monotonic stack.

---

## Exercises

1. **`stack_from_array`** — build a stack from an existing `int[]` in order.
2. **`stack_peek_at(k)`** — the k-th element from the top *without* popping. Does
   exposing this weaken the LIFO guarantee? Discuss.
3. **Linked-list-backed stack** — reimplement `Stack` composing your Lesson 2 list
   instead of the array. Same interface, different internals — that's the power of a
   contract. Compare push/pop complexity (worst-case vs. amortized).

## Challenge problems

1. **Min-Stack (M6).** O(1) `push`, `pop`, `peek`, *and* `get_min`. Hint: store the
   running minimum alongside each value.
2. **Two stacks, one array.** Implement two stacks sharing a single backing array,
   growing toward each other from both ends. When is it full?
3. **Evaluate an expression** like `3 + 4 * 2 - 1` using an operator stack and an
   operand stack (shunting-yard, lite).

## Stretch goals

- **Generic stack** by making the backing array generic (the Lesson 1 `void*`
  stretch). Then a stack of strings, structs, anything.
- **Bounded stack** with a fixed capacity that rejects `push` when full — model a
  hardware stack.

## Common mistakes

- **`size - 1` on an empty stack.** Underflows to `SIZE_MAX`, then `da_get` reads
  wild memory (ASan screams). Check `is_empty` first.
- **Freeing in the wrong order / leaking the backing array.** `stack_free` must
  `da_free(backing)` then `free(s)`. Forgetting the first leaks the whole array.
- **Reaching around the interface** — calling `da_*` on `s->backing` from outside
  the stack defeats the entire point. Keep the backing private in spirit.
- **Assuming push is O(1) worst-case** — it's *amortized* O(1); a push that triggers
  a grow is O(n). Matters for latency-sensitive code.

## Debugging tips

- If the build fails inside `dynamic_array.c`, your Lesson 1 and its header
  disagree — the composed build surfaces interface drift immediately (a feature).
- Print the backing array (`da_print`, if you wrote it) to see the stack bottom→top.
- Test the empty-policy path explicitly; it's the one everyone forgets.

## Frequently asked questions

**Q: Isn't wrapping the array just pointless indirection?**
A: No — the *restriction* is the value. The wrapper guarantees an invariant (LIFO,
only-touch-the-end) that raw array access can't. You're trading a tiny amount of
indirection for a strong, enforced contract. That trade is most of software
architecture.

**Q: Should I back it with the array or a linked list?**
A: Array (this lesson): great cache locality, amortized O(1), occasional grow
pause. Linked list: worst-case O(1), stable addresses, but a `malloc` per push and
poor locality. For a general stack the array usually wins; do the linked-list
version as an exercise to feel the difference.

**Q: How is this the "call stack"?**
A: Identically. Each function call pushes an activation record; returning pops it;
you can only access the top frame. Recursion too deep overflows it. Building this
demystifies "stack overflow" and how DFS-via-recursion really works.

---

## Definition of done

- [ ] Milestones M1–M5 implemented (M6–M7 recommended).
- [ ] Full suite passes, every §5 edge case covered.
- [ ] **`make test` clean under ASan/UBSan; `valgrind` zero leaks** (both modules).
- [ ] §6 table filled from your reasoning; you can explain why push is *amortized*
      O(1) and why the top maps to the array's end.
- [ ] You can articulate, in one sentence, why composition beats inheritance here.
- [ ] `PROGRESS.md` written. Code review requested.

→ Next: [`../05-queue/`](../05-queue/) — FIFO, where the array is the *wrong* tool
(front removal is O(n)) and a linked list with head+tail is the natural fit. Then
you'll rebuild it as a **circular buffer** and see how an array reclaims front space
without shifting.

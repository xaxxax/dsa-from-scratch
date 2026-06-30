# Project 4 — Stack (built on your Dynamic Array)

> **Status:** lesson outline (full eight-section treatment follows the Project 1
> template).

**Builds on:** Project 1 — your `DynamicArray<E>` is the backing store. This is
your first taste of **composition**: building a new structure by *using* one you
already wrote, not by reinventing storage.

## The core question
Many problems need only one access pattern: **last in, first out** (LIFO). Undo
in an editor, the call stack, matching brackets, backtracking. Could we expose a
*restricted* interface that makes LIFO trivial and misuse impossible?

## The idea
A stack supports just `push`, `pop`, `peek`, `isEmpty`. Back it with your dynamic
array and operate only at the *end* (where the array is O(1) amortized).

```
push(D):                   pop() → D:
  top → | A | B | C | D |    top → | A | B | C |
        | A | B | C |              | A | B |
```

## What you'll implement
- `Stack<E>` wrapping a `DynamicArray<E>` (favor composition over inheritance —
  why?).
- `push`, `pop` (throw on empty — `EmptyStackException` or your own), `peek`,
  `isEmpty`, `size`.

## Key questions
- Why push/pop at the *end* of the array rather than the front?
- Why is "restrict the interface" a feature, not a limitation?
- Composition vs. inheritance: why *not* `extends DynamicArray`?
- How does the program **call stack** mirror this exact structure?

## LeetCode (in order)
1. [Easy] 20. Valid Parentheses (the textbook stack problem)
2. [Easy] 155. Min Stack (augment a stack to track its min in O(1))
3. [Medium] 150. Evaluate Reverse Polish Notation
4. [Medium] 739. Daily Temperatures (monotonic stack)
5. [Hard] 84. Largest Rectangle in Histogram (monotonic stack, advanced)

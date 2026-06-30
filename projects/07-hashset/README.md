# Project 7 — HashSet (built on your HashMap)

> **Status:** lesson outline (full eight-section treatment follows the Project 1
> template).

**Builds on:** Project 6. A short, satisfying project that teaches a deep lesson
about **code reuse and the right abstraction**.

## The core question
We often want to know only *"have I seen this before?"* — membership, with no
associated value, and no duplicates. How do we get O(1) `add`/`contains`/`remove`
of unique elements? And can we get it almost for free from work already done?

## The idea
A set is a map whose values don't matter. The classic implementation **delegates
to a `HashMap`**, storing each element as a key mapped to a dummy/sentinel value.
This is exactly how `java.util.HashSet` is built (it wraps a `HashMap`).

```
HashSet<String>            backing HashMap<String, Object>
 {cat, dog, fox}    ──▶    cat → PRESENT
                           dog → PRESENT
                           fox → PRESENT     (PRESENT = a shared dummy object)
```

## What you'll implement
- `Set<E>` interface; `HashSet<E>` that wraps your `HashMap<E, Object>`.
- `add`, `contains`, `remove`, `size`.
- **Stretch:** set algebra — `union`, `intersection`, `difference`.

## Key questions
- Why implement a set on top of a map instead of from scratch? (What duplication
  do you avoid? What does it cost?)
- What's the single dummy value Java uses, and why one *shared* object?
- How do `equals`/`hashCode` on your *elements* drive correctness here too?

## LeetCode (in order)
1. [Easy] 217. Contains Duplicate (the purest set problem)
2. [Easy] 349. Intersection of Two Arrays
3. [Medium] 128. Longest Consecutive Sequence (O(n) with a set — clever!)
4. [Medium] 36. Valid Sudoku (sets to track seen values per row/col/box)

# Project 2 — Singly Linked List

> **Status:** lesson outline. The full eight-section treatment follows the
> Project 1 template (Motivation → Visualization → Design → Milestones → Testing
> → Complexity → Real-World → LeetCode + exercises, reflections, challenges,
> stretch goals, common mistakes, debugging tips, FAQ). Ask your mentor to
> expand this when you reach it.

**Builds on:** Phase 0. Implements the **same `List<E>` interface** as Project 1
— so you can directly compare the trade-offs.

## The core question
A dynamic array gives O(1) indexed access but O(n) insertion in the middle and
occasional O(n) resizes. What if you need **O(1) insertion/removal at the front**
and never want to copy/resize — and you're willing to give up random access to
get it?

## The idea
Stop storing elements in one contiguous block. Instead, each element lives in its
own **node** that holds a value and a **reference to the next node**. The list is
just a pointer to the first node (`head`).

```
head
 │
 ▼
[A|·]──▶[B|·]──▶[C|·]──▶[D|/]      (/ = null, the end)
```

## What you'll implement
- A private `Node<E>` (value + `next`).
- Fields: `head`, `size` (and consider `tail` for O(1) append — why does that
  help, and what does it cost you on removal?).
- `addFirst`, `addLast`, `get`, `remove`, `contains`, plus the `List<E>` methods.
- Reversing the list (the quintessential pointer-rewiring exercise).

## Key questions to wrestle with
- Why is `get(i)` now O(n) when it was O(1) for the array?
- Why is `addFirst` O(1) here but O(n) for the array?
- What breaks if you lose the `head` reference mid-operation? (Order of pointer
  reassignments matters enormously — draw every step.)
- Do you need a `tail` pointer? What does it make fast, and what stays slow?

## LeetCode (attempt in order, no solutions here)
1. [Easy] 876. Middle of the Linked List (slow/fast pointers)
2. [Easy] 206. Reverse Linked List (the canonical pointer exercise)
3. [Easy] 21. Merge Two Sorted Lists
4. [Easy] 141. Linked List Cycle (Floyd's tortoise & hare)
5. [Medium] 19. Remove Nth Node From End (two-pointer gap)

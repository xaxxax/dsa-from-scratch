# Progress Log — Project NN: <Name>

> Copy this file into the project folder as `PROGRESS.md` and fill it in as you
> go. The most valuable parts are the bugs and the questions — write those down
> *while they hurt*, not after.

**Started:** 2026-07-01
**Finished:** 2026-07-03
**Branch:** `project/00-dynamic-array`

---

## Milestones

- [ ] Read Motivation + Visualization, drew the diagram on paper
- [ ] Designed classes/fields before reading the Design section
- [ ] Constructor
- [ ] ... (copy the milestone list from the project README)
- [ ] Tests pass
- [ ] Code review done

---

## What clicked

_The moments where it suddenly made sense. What was the unlock?_

Realizing why things were doubled to save compute time and efficieny so we do not have to copy array to new block of memory every time it gets full
Trade off is that you may have unused memory, but worth it in the longrun

---

## What didn't (yet)

_Concepts that are still fuzzy. Be specific — "resizing" is vague; "why we
double instead of adding a fixed amount" is a real question._

---

## Bugs I hit (the gold)

| Symptom | Root cause | How I found it |
|---------|-----------|----------------|
|     Size returning wrong value    |     Forgetting to increase/decrease size at the end of mutating List with remove/add, etc      |       Realized in test cases         |
|     Realizing a way better approach to insert method     |     The algorithm worked, but was very slow and needed two variables     |     Help with Claude having it check my    work     |


_For each bug: what did it look like, what actually caused it, and what
debugging move surfaced it? This table is the most useful thing you'll write._

---

## My complexity derivations

_Don't copy the table from the README — derive it yourself, then compare._

| Operation | My answer | Why | README says |
|-----------|-----------|-----|-------------|
|     add      |     O(1)      |  Simply adding element to last part of array   |      O(n), did not consider resize() being needed in worst case, very true       |
|     get      |     O(1)      |  Simple array lookup, using array index gets O(1) time, no traversal  |      Correct       |
|     set      |     O(1)      |  Again, no traversal and use index to set, instantaneous   |      Correct       |
|     remove      |  O(n)         |  Worst case you must shift every single element to the left one, needing you to traverse the whole array  |       Correct      |
|     contains      |     O(n)      |  Again, worst case you may have to traverse whole array to return T or F   |      Correct       |
|     size     |     O(1)      |   This is a pre-computed value that is simply being returned  |             |
|     capacity      |     O(1)      |  Same as size()  |             |
|     resize     |     O(n)      |  In this case, we must traverse the array and copy each element over, then allocate memory for new array and paste everything there   |             |
|     insert      |     O(n)      |  Same as remove(), worst case you must shift every element right one, needing to traverse the whole array   |             |

---

## Questions I still can't answer

_Open loops to revisit. List them so they don't quietly disappear._

1. Look into when shift left starting from index and moving up, why better
2. Look into when shift right starting from end of array and moving down, why better

---

## One-paragraph summary (write this last)

_Explain this structure to an imaginary beginner in your own words. If you can't,
you're not done._

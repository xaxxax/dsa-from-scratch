# Project 12 — Depth First Search (DFS)

> **Status:** lesson outline (full eight-section treatment follows the Project 1
> template).

**Builds on:** Project 11 (the graph), Project 4 (a stack — DFS *is* a stack,
explicit or via recursion), Project 7 (a set, to mark visited nodes).

## The core question
Given a graph, how do we **visit every reachable vertex** systematically? DFS
answers: go as **deep** as possible down one path before backtracking. This
single idea powers cycle detection, topological sort, connectivity, maze-solving,
and more.

## The idea
From a start vertex, dive into a neighbor, then *its* neighbor, and so on, until
you hit a dead end; then back up and try the next unexplored branch. Track
**visited** vertices (your HashSet) so you don't loop forever on cycles.

```
   1 — 2        DFS(1): 1 → 2 → 4 → (back) → 3   (deep before wide)
   |   |
   3   4        recursion uses the CALL stack; the iterative version uses YOUR
                Stack from Project 4 — same algorithm, explicit vs. implicit.
```

## What you'll implement
- **Recursive** DFS (implicit call-stack).
- **Iterative** DFS (your explicit `Stack`) — and notice why the visit order can
  differ.
- Applications: connected components, **cycle detection**, **topological sort**.

## Key questions
- Why is DFS O(V + E)? Account for both the vertices and the edges.
- Why does iterative DFS need an explicit stack, and how does it relate to the
  recursive call stack?
- Why must you mark visited *when you pop/visit* (and sometimes *when you push*)?
  What bug appears if you forget?
- How does DFS detect a cycle differently in directed vs. undirected graphs?

## LeetCode (in order)
1. [Easy] 200. Number of Islands (grid as an implicit graph)
2. [Medium] 695. Max Area of Island
3. [Medium] 207. Course Schedule (cycle detection via DFS)
4. [Medium] 210. Course Schedule II (topological sort)
5. [Medium] 417. Pacific Atlantic Water Flow

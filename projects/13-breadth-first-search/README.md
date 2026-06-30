# Project 13 — Breadth First Search (BFS)

> **Status:** lesson outline (full eight-section treatment follows the Project 1
> template).

**Builds on:** Project 11 (the graph), Project 5 (a queue — BFS *is* a queue),
Project 7 (a set for visited).

## The core question
DFS finds *a* path. But what if we want the **shortest** path (fewest edges) in
an unweighted graph — the minimum number of hops? DFS can't guarantee that; BFS
can, because it explores **level by level**, nearest first.

## The idea
Visit all vertices at distance 1, then all at distance 2, and so on. A **queue**
(FIFO) enforces this ordering: enqueue a vertex's unvisited neighbors, dequeue in
the order discovered. The first time you reach the target, you've used the fewest
edges possible.

```
   1 — 2        BFS(1): [1] → [2,3] → [4]      (wide before deep)
   |   |
   3   4        distance from 1:  1:0  2:1  3:1  4:2
```

## What you'll implement
- BFS traversal using **your `Queue`** and a visited set.
- **Shortest path** (edge count) in an unweighted graph, reconstructing the path
  via a `parent`/`predecessor` map.
- **Stretch:** multi-source BFS; level-order over a tree (ties back to Project 8).

## Key questions
- Why does the FIFO queue guarantee shortest paths in *unweighted* graphs — and
  why does that guarantee break once edges have weights? (That's why Dijkstra
  uses a *priority* queue — your Project 9 heap.)
- Why O(V + E) time and O(V) space?
- DFS vs. BFS: when do you reach for each? (Shortest path → BFS; exhaustive
  exploration / backtracking / topo-sort → DFS.)

## LeetCode (in order)
1. [Easy] 1971. Find if Path Exists in Graph (re-solve, BFS this time)
2. [Medium] 200. Number of Islands (re-solve with BFS; compare to DFS)
3. [Medium] 994. Rotting Oranges (multi-source BFS over time)
4. [Medium] 127. Word Ladder (shortest transformation — classic BFS)
5. [Medium] 286. Walls and Gates (multi-source BFS)

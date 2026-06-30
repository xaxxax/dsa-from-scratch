# Project 14 — Union-Find (Disjoint Set Union)

> **Status:** lesson outline (full eight-section treatment follows the Project 1
> template).

**Builds on:** Project 1 (the whole structure is just one or two int arrays —
elegantly minimal). Pairs naturally with graph connectivity (Project 11).

## The core question
"Are these two elements in the same group?" and "merge these two groups." Think
network connectivity, Kruskal's minimum spanning tree, image-segment merging,
account-merging. We could rerun BFS/DFS each query (O(V+E) each), but with many
queries that's far too slow. Can we make both operations **nearly O(1)**?

## The idea
Represent each set as a tree where every element points to a **parent**, and the
root identifies the set. `find(x)` walks to the root; `union(a, b)` links one
root under the other. Two optimizations make it astonishingly fast:

- **Union by rank/size:** always attach the smaller tree under the larger, keeping
  trees shallow.
- **Path compression:** during `find`, flatten the path by pointing nodes
  directly at the root.

```
parent[]:  index: 0 1 2 3 4
                  0 0 1 3 3      → sets {0,1,2} (root 0) and {3,4} (root 3)

find(2): 2→1→0  (root 0).  With path compression, 2 now points straight to 0.
```

## What you'll implement
- `UnionFind(int n)` over a `parent[]` (and a `rank[]`/`size[]`).
- `find(x)` with **path compression**; `union(a, b)` **by rank**; `connected(a,b)`.

## Key questions
- Why does combining path compression *and* union by rank give ~O(α(n)) — the
  inverse-Ackermann bound that's effectively constant? (See the cheat sheet.)
- What goes wrong (degenerate linked-list trees) if you skip *both* optimizations?
- Why is this often faster than BFS/DFS for *repeated* connectivity queries — and
  what can it *not* do that traversal can (e.g. report the actual path)?

## LeetCode (in order)
1. [Medium] 323. Number of Connected Components in an Undirected Graph
2. [Medium] 547. Number of Provinces
3. [Medium] 684. Redundant Connection (detect the edge that forms a cycle)
4. [Medium] 200. Number of Islands (re-solve with Union-Find; compare DFS/BFS)
5. [Hard] 305. Number of Islands II (where Union-Find truly shines)

# Project 11 — Graph (representation)

> **Status:** lesson outline (full eight-section treatment follows the Project 1
> template).

**Builds on:** Projects 1, 2, 6 (you'll store the graph using your own array,
lists, and map). Prerequisite for Projects 12 (DFS), 13 (BFS), 14 (Union-Find).

## The core question
Trees model hierarchy (one parent). But the world is full of **arbitrary
relationships**: friends, roads, web links, dependencies, course prerequisites.
How do we represent a set of entities (**vertices**) connected by arbitrary
relationships (**edges**) — possibly directed, possibly weighted?

## The idea
Two classic representations, each with different trade-offs:

```
Adjacency LIST (good for sparse graphs):     Adjacency MATRIX (good for dense):
 0 → [1, 2]                                       0  1  2  3
 1 → [0, 3]                                    0 [ 0  1  1  0 ]
 2 → [0]                                       1 [ 1  0  0  1 ]
 3 → [1]                                       2 [ 1  0  0  0 ]
                                               3 [ 0  1  0  0 ]
 space O(V + E)                                space O(V²)
```

## What you'll implement
- A `Graph` supporting `addVertex`, `addEdge`, `neighbors(v)`, with flags for
  **directed/undirected** and **weighted/unweighted**.
- Build it **both** ways — adjacency list (using your map + lists) and adjacency
  matrix (using a 2D array) — so the trade-off is concrete, not abstract.

## Key questions
- Adjacency list vs. matrix: when is each better? (Think sparse vs. dense, and
  the cost of "is there an edge u→v?" vs. "list all neighbors of u".)
- How does an undirected edge differ from two directed edges in storage?
- Where do edge *weights* live in each representation?
- What real systems are graphs? (Social networks, maps, the web, build systems,
  package managers, neural nets.)

## LeetCode (in order — these blend into Projects 12/13)
1. [Easy] 1971. Find if Path Exists in Graph (build + simple traversal)
2. [Medium] 133. Clone Graph (forces you to traverse and copy)
3. [Medium] 207. Course Schedule (model prerequisites as a graph)
4. [Medium] 323. Number of Connected Components (sets up Union-Find, Project 14)

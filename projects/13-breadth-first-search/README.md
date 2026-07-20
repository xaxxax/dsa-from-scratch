# Project 13 — Breadth-First Search (BFS)

> **You are building:** the traversal that explores in **rings of increasing
> distance** — and, as a direct consequence, finds **shortest paths in unweighted
> graphs.** It's the same graph as DFS, driven by a *queue* instead of a stack.
>
> **Builds on:** Lesson 11 (the graph) and Lesson 5 (a queue — the frontier
> structure). Together with DFS this completes the traversal toolkit.
>
> **The C leap:** same "algorithm owns its scratch" lesson as DFS, but the key
> insight is structural: swapping DFS's stack for a **FIFO queue** changes deep-dive
> into level-by-level — a vivid demonstration that the *frontier data structure*
> determines the traversal.
>
> **Mentor rule:** diagrams and questions here, not code.

---

## 1. Motivation

### The problem

DFS reaches everything, but it dives — the path it finds to a vertex may be long and
winding. Often you need the **shortest** route: fewest hops in a maze, degrees of
separation in a social graph, minimum moves in a puzzle, nearest cache node. On an
**unweighted** graph, "shortest" means "fewest edges." How do you find that?

### The idea

Explore in **order of distance**. Visit the start (distance 0), then *all* its
neighbors (distance 1), then all of *their* unseen neighbors (distance 2), and so
on — expanding outward in rings. A **FIFO queue** enforces exactly this: you enqueue
newly discovered vertices at the back and process from the front, so everything at
distance *d* is handled before anything at distance *d+1*.

### The headline property

Because BFS reaches vertices in nondecreasing distance order, **the first time it
reaches a vertex is along a shortest path.** Record `dist[v] = dist[u] + 1` when you
first discover v from u, and you've computed single-source shortest paths in an
unweighted graph — the reason BFS, not DFS, is the shortest-path traversal.

---

## 2. Visualization

```
 undirected graph:     0 —— 1 —— 3 —— 4
                       |    |
                       2 ———+

 BFS(0) with a queue:
   enqueue 0.                      queue:[0]        dist 0: 0
   dequeue 0 → enqueue 1,2.        queue:[1,2]      dist 1: 1,2
   dequeue 1 → enqueue 3.          queue:[2,3]
   dequeue 2 → (3 already queued)  queue:[3]
   dequeue 3 → enqueue 4.          queue:[4]        dist 2: 3
   dequeue 4.                      queue:[]         dist 3: 4
   order: 0, 1, 2, 3, 4     distances: 0,1,1,2,3
```

The queue is what makes it breadth-first: contrast DFS, whose stack would have gone
`0→1→3→4` deep before ever visiting 2. **Same graph, same code shape — the frontier
structure (queue vs. stack) is the entire difference.**

---

## 3. Design

BFS composes the graph — only `graph_num_vertices` and `graph_neighbors`.

```c
size_t bfs(const Graph *g, size_t start, size_t *out_order);       /* visit order */
void   bfs_distances(const Graph *g, size_t start, int *dist);     /* shortest hops */
```

### Design questions

1. **Mark on ENQUEUE, not on dequeue.** This is *the* BFS correctness rule. If you
   mark only when dequeuing, a vertex can be enqueued several times (once per
   neighbor that discovers it) before it's processed — inflating the queue and
   breaking the distance guarantee. Enqueue and mark are one atomic step.
2. **The queue's size.** Each vertex is enqueued at most once, so a plain `size_t`
   array of `num_vertices` slots with head/tail indices suffices (no wrap needed).
   Reuse your Lesson 5 ring-buffer thinking if you like, but you don't have to grow.
3. **`dist` doubles as visited.** In `bfs_distances`, `dist[v] == -1` means
   "unseen." Setting the distance *is* marking visited — one array, two jobs. Why is
   the first distance assigned always the shortest?
4. **Caller-owned output.** Both `out_order` and `dist` are caller buffers of length
   `num_vertices`; you fill them. Your scratch (the queue, and a visited array for
   `bfs`) you allocate and free.
5. **Weighted graphs break this.** BFS's shortest-path guarantee holds *only* for
   unit-weight edges. With weights, a 2-hop path can be cheaper than a 1-hop one, so
   you need Dijkstra (BFS + your Lesson 9 heap). Know the boundary.

---

## 4. Milestones

Scaffold: [`src/bfs.c`](src/bfs.c) / [`src/bfs.h`](src/bfs.h). The [`Makefile`](Makefile)
compiles your Lesson 11 `graph.c` alongside this.

```
[ ] M0  Build: compiles both modules; make test aborts at the first TODO (bfs).
[ ] M1  (Prereq) Lesson 11 graph implemented enough to build a test graph.
[ ] M2  bfs: queue + visited (mark on enqueue!); fill out_order in distance order;
        free scratch; return count. Test: reaches the whole component; start first.
[ ] M3  bfs_distances: dist[]=-1 init, dist[start]=0, propagate dist[v]=dist[u]+1 on
        first discovery. Test the exact distances (the shortest-path guarantee).
[ ] M4  (Stretch) parent[] to RECONSTRUCT a shortest path start→target; multi-source
        BFS (enqueue several starts at distance 0); BFS on an implicit grid graph.
[ ] M5  (Stretch) 0-1 BFS (deque) for graphs with weights in {0,1}; then read up on
        Dijkstra for general weights (uses your Lesson 9 heap).
[ ] M6  Full suite green under ASan; valgrind clean (no leaked scratch). Review.
```

---

## 5. Testing

```bash
cd projects/13-breadth-first-search
make test
make && valgrind --leak-check=full --error-exitcode=1 ./out_test
```

Starter: [`tests/test_bfs.c`](tests/test_bfs.c). Aborts inside `graph.c` ⇒ finish
Lesson 11.

### Edge cases (test all)

- **Distances are minimal:** build a graph where a vertex is reachable by both a
  short and a long path; BFS must report the short one.
- **Unreachable = -1:** an isolated vertex keeps `dist == -1`.
- **Cycle:** BFS must terminate, visiting each vertex once.
- **Start first, distance 0:** `out_order[0] == start`, `dist[start] == 0`.
- **Single vertex.**
- **No leaks:** the queue and visited array are freed (valgrind clean).

---

## 6. Complexity Analysis

Derive first. V = vertices, E = edges.

| Aspect | Cost | Why |
|--------|:----:|-----|
| time | ? | ? |
| space (scratch) | ? | ? |

<details>
<summary>Answers.</summary>

- Time: **O(V + E)** — identical to DFS. Each vertex is enqueued/dequeued once
  (O(V)); each edge is examined once per direction when its source is dequeued
  (O(E)). The traversal cost doesn't depend on stack-vs-queue.
- Space: **O(V)** — the queue holds at most V vertices, plus the visited/dist array.
- Shortest paths: BFS gives them in **O(V + E)** for *unweighted* graphs — far
  cheaper than Dijkstra's O(E log V), which you only need once edges have weights.
</details>

---

## 7. Real-World Usage

- **Shortest path in unweighted graphs** — maze/grid pathfinding, "fewest moves"
  puzzles (Rubik's, sliding puzzles), word-ladder.
- **Social networks** — degrees of separation, "people you may know" within k hops.
- **Web crawling** — BFS from a seed URL explores nearby pages first.
- **Broadcast / flood** — network routing, peer-to-peer propagation, GC mark phase.
- **Bipartite testing, level-order tree traversal, multi-source expansion**
  (e.g., "rotting oranges," nearest-facility maps).

---

## 8. LeetCode

**No solutions here.**

1. **[Easy] 1091. Shortest Path in Binary Matrix** — grid BFS, the archetype.
2. **[Medium] 994. Rotting Oranges** — multi-source BFS by time step.
3. **[Medium] 127. Word Ladder** — BFS over an implicit word graph.
4. **[Medium] 133. Clone Graph** — BFS or DFS + visited map.
5. **[Medium] 542. 01 Matrix** — multi-source BFS distances.

---

## Exercises

1. **`shortest_path(g, a, b, out)`** — BFS with a `parent[]`, then walk parents from
   b back to a to reconstruct the actual path (not just its length).
2. **Multi-source BFS** — seed the queue with several starts at distance 0 (e.g.,
   "distance to the nearest exit"). Why does one BFS solve all sources at once?
3. **Bipartite check** — 2-color the graph via BFS; a same-color edge means not
   bipartite.

## Challenge problems

1. **Grid BFS without building a Graph** — treat an `m×n` grid as an implicit graph
   (neighbors = up/down/left/right in-bounds). Most BFS interview problems are this.
2. **0-1 BFS** — edges weighted 0 or 1; use a *deque* (push 0-edges to the front,
   1-edges to the back) for O(V+E) shortest paths. A neat bridge toward Dijkstra.
3. **Bidirectional BFS** — search from both start and target, meeting in the middle;
   dramatically fewer vertices explored on large graphs. Prove correctness.

## Stretch goals

- **Dijkstra** — generalize `bfs_distances` to weighted graphs by replacing the
  FIFO queue with your Lesson 9 **min-heap** keyed by distance. Feel how BFS is the
  unit-weight special case of Dijkstra.
- **A\*** — Dijkstra plus a heuristic; the pathfinding used in games and maps.

## Common mistakes

- **Marking on dequeue** — the cardinal BFS bug; a vertex gets enqueued multiple
  times and the distance guarantee breaks. Mark on ENQUEUE.
- **Using a stack instead of a queue** — that's DFS; you lose the distance ordering
  silently (it still "works," just wrong distances).
- **Leaking the queue/visited scratch** — allocate and free every call.
- **Expecting BFS to be correct on weighted graphs** — it isn't; that's Dijkstra.
- **Off-by-one in the array queue** — head/tail index bugs; test with a 2-3 vertex
  graph first.

## Debugging tips

- Print the queue contents and the `dist` array as BFS runs; the ring structure
  (all of distance d before any of d+1) should be visible.
- If distances look wrong, check the mark-on-enqueue rule first — it's the usual
  culprit.
- Verify the *graph* before blaming BFS: a missing reverse edge makes vertices look
  unreachable.

## Frequently asked questions

**Q: Why does BFS find shortest paths but DFS doesn't?**
A: BFS processes vertices in nondecreasing distance order (the queue guarantees it),
so the first time it reaches a vertex is necessarily via a fewest-edges path. DFS
dives along one branch and may reach a vertex by a long route first, with no such
guarantee. The frontier discipline — FIFO vs. LIFO — is the whole reason.

**Q: What changes for weighted graphs?**
A: The "first time reached = shortest" logic assumes every edge costs 1. With
weights, a longer-hop path can be cheaper, so you must always expand the
*currently-cheapest* frontier vertex — a priority queue (min-heap) instead of a FIFO.
That's Dijkstra; BFS is its unit-weight special case.

**Q: DFS and BFS are both O(V+E) — when does the choice matter?**
A: When the *question* is about distance/levels (BFS) versus structure/reachability/
ordering (DFS). Also practically: BFS's queue can hold O(V) vertices (wide graphs
use lots of memory); DFS's stack is O(depth). Pick by what you're computing and the
graph's shape.

---

## Definition of done

- [ ] Milestones M2–M3 implemented (M4–M6 recommended; path reconstruction + grid
      BFS are the most-used skills).
- [ ] Full suite passes, especially the exact shortest distances and unreachable=-1.
- [ ] **`make test` clean under ASan/UBSan; `valgrind` zero leaks** (scratch freed).
- [ ] §6 filled from your reasoning; you can explain why mark-on-enqueue is required
      and why BFS gives shortest paths only when unweighted.
- [ ] `PROGRESS.md` written. Code review requested.

→ Next: [`../14-union-find/`](../14-union-find/) — leave traversal for a structure
that answers "are these connected?" and "merge these groups" in almost O(1), using
nothing but an array of parent pointers.

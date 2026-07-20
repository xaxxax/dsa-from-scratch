# Project 12 — Depth-First Search (DFS)

> **You are building:** the traversal that goes as **deep as possible** before
> backtracking — the workhorse behind cycle detection, topological sort, connected
> components, maze/backtracking solvers, and countless interview problems.
>
> **Builds on:** Lesson 11 (the graph you traverse) and Lesson 4 (a stack — because
> the recursion stack *is* one). This is your first **algorithm on** a structure
> rather than a structure itself.
>
> **The C leap:** an algorithm that allocates its own scratch memory. DFS needs a
> `visited` array; you `malloc`/`calloc` it, use it, and `free` it — even a
> read-only traversal must not leak its bookkeeping. And you'll see the recursion
> stack made explicit as a real array-stack.
>
> **Mentor rule:** diagrams and questions here, not code.

---

## 1. Motivation

### The problem

You have a graph (Lesson 11) and need to *systematically visit* everything reachable
from a starting vertex — to find a path, detect a cycle, order dependencies, or
count connected pieces. You need a disciplined way to explore that (a) reaches every
reachable vertex and (b) never gets stuck looping on a cycle.

### The idea

**Go deep.** From the current vertex, walk to an unvisited neighbor, then *its*
unvisited neighbor, and so on — plunging down one path until you hit a dead end or
an already-seen vertex, then **backtrack** and try the next branch. A **visited**
marker per vertex is what makes cycles safe: you never re-enter a vertex.

Recursion expresses this naturally — each recursive call is "explore from here" and
returning *is* the backtrack. The call stack holds the path you're currently on,
which is exactly why "the recursion stack is a stack" (Lesson 4): you can replace it
with an explicit stack and get the same traversal.

---

## 2. Visualization

```
 undirected graph:        0 —— 1 —— 3
                          |         |
                          2 ————————+

 DFS(0), exploring neighbors in ascending order:
   visit 0  → go to 1
   visit 1  → go to 3
   visit 3  → neighbors 1(seen), 2 → go to 2
   visit 2  → neighbors 0(seen), 3(seen) → dead end, backtrack all the way
   order: 0, 1, 3, 2
```

The **visited** set is doing the quiet heavy lifting: when 2 looks back at 0 and 3,
both are marked, so DFS doesn't loop. Remove the marker and this graph (which has a
cycle 0-1-3-2-0) spins forever.

```
 recursion == an implicit stack of "where I still have neighbors to try":
   stack grows:  [0] → [0,1] → [0,1,3] → [0,1,3,2]
   then unwinds as each vertex runs out of unvisited neighbors.
```

**Trace DFS on paper for a small cyclic graph, maintaining the visited set and the
stack.** When your trace matches the code, you understand it.

---

## 3. Design

DFS composes the graph — it never touches the `Graph` internals, only:

```c
size_t          graph_num_vertices(const Graph *g);
const AdjNode  *graph_neighbors(const Graph *g, size_t u);   /* walk ->next, ->dest */
```

Your functions:

```c
size_t dfs(const Graph *g, size_t start, size_t *out_order);            /* recursive */
size_t dfs_iterative(const Graph *g, size_t start, size_t *out_order);  /* explicit stack */
```

### Design questions

1. **Where does `visited` live?** It's O(V) scratch you allocate per traversal (a
   `calloc`'d array, all zero = unvisited), use, and free. A traversal that leaks its
   visited array is a bug even though it "returns the right answer." Own your memory.
2. **What does `out_order` mean, and who owns it?** The caller passes a buffer of at
   least `num_vertices`; you fill it with visit order and return the count. The
   caller owns it — you never allocate it. (Why is caller-owned output often cleaner
   in C than returning a malloc'd array?)
3. **Component vs. whole graph.** DFS from `start` visits only start's connected
   component. To cover the *whole* graph you loop over all vertices and DFS any still
   unvisited one — that's how you count components (a stretch).
4. **Recursive vs. iterative.** Recursion is clean but risks call-stack overflow on
   a huge/deep graph (V frames). The explicit-stack version is O(V) heap instead —
   and proves the recursion stack was a stack. Their visit *orders* can differ
   (mark-on-push vs. mark-on-pop); both are valid DFS.
5. **Pre-order vs. post-order.** Recording a vertex *when you enter* it (pre-order)
   vs. *when you finish* it (post-order) gives different orders — post-order is the
   key to topological sort. Note where you record.

---

## 4. Milestones

Scaffold: [`src/dfs.c`](src/dfs.c) / [`src/dfs.h`](src/dfs.h). The [`Makefile`](Makefile)
compiles your Lesson 11 `graph.c` alongside this.

```
[ ] M0  Build: compiles both modules; make test aborts at the first TODO (dfs).
        "unused" warning for dfs_visit is expected until M3.
[ ] M1  (Prereq) Lesson 11 graph implemented enough to build a test graph.
[ ] M2  dfs: allocate/free the visited array, bounds-check start, delegate to the
        recursive helper, return the count.
[ ] M3  dfs_visit (recursive core): mark, record, recurse into unvisited neighbors.
        Test: DFS reaches exactly the start's component; terminates on cycles.
[ ] M4  dfs_iterative: same traversal with an explicit stack. Confirm it reaches the
        same SET of vertices (order may differ).
[ ] M5  (Stretch) whole-graph DFS + connected-component count; cycle detection;
        topological sort (post-order on a DAG); path reconstruction via a parent[].
[ ] M6  Full suite green under ASan; valgrind clean (no leaked scratch). Review.
```

---

## 5. Testing

```bash
cd projects/12-depth-first-search
make test
make && valgrind --leak-check=full --error-exitcode=1 ./out_test
```

Starter: [`tests/test_dfs.c`](tests/test_dfs.c). Aborts inside `graph.c` ⇒ finish
Lesson 11. Because visit order varies with adjacency order, test **invariants**, not
an exact sequence.

### Edge cases (test all)

- **Cycle:** a triangle `0-1-2-0` — DFS must visit each once and *terminate* (the
  visited set is what guarantees this).
- **Correct component:** from `start`, you reach exactly its connected component and
  no more.
- **Start first:** `out_order[0] == start`.
- **Single vertex / isolated vertex:** DFS returns just that vertex.
- **Recursive vs. iterative** reach the same *set* of vertices.
- **No leaks:** valgrind is clean — the visited array and any stack are freed.

---

## 6. Complexity Analysis

Derive first. V = vertices, E = edges.

| Aspect | Cost | Why |
|--------|:----:|-----|
| time | ? | ? |
| space (scratch) | ? | ? |
| recursion depth | ? | ? |

<details>
<summary>Answers.</summary>

- Time: **O(V + E)** — each vertex is marked once, and across the whole run each
  edge is examined a constant number of times (once per direction on an adjacency
  list). This is *the* reason the adjacency list is the right representation: on a
  matrix, listing neighbors is O(V) per vertex, making traversal O(V²).
- Scratch space: **O(V)** for the visited array (plus O(V) for the explicit stack in
  the iterative version).
- Recursion depth: up to **O(V)** in the worst case (a path graph) — which can
  overflow the real call stack for large V, the practical reason to keep the
  iterative version around.
</details>

---

## 7. Real-World Usage

- **Topological sort** — build order for package managers, task schedulers,
  spreadsheet recalculation (DFS post-order on a DAG).
- **Cycle detection** — deadlock detection, dependency-cycle errors ("circular
  import").
- **Connected components / flood fill** — image regions, "number of islands," maze
  regions.
- **Backtracking** — Sudoku, N-queens, maze solving, generating permutations: DFS
  over a state graph.
- **Compilers** — reachability, dominator trees; **garbage collectors** — mark phase
  is a DFS/BFS over the object graph.

---

## 8. LeetCode

**No solutions here.**

1. **[Easy] 733. Flood Fill** — DFS on a grid (implicit graph).
2. **[Medium] 200. Number of Islands** — DFS components on a grid.
3. **[Medium] 207. Course Schedule** — cycle detection / topo sort (directed).
4. **[Medium] 133. Clone Graph** — DFS + visited map.
5. **[Medium] 417. Pacific Atlantic Water Flow** — multi-source DFS.

---

## Exercises

1. **`connected_components(g)`** — DFS from every unvisited vertex; return the count
   (compare with your Lesson 14 Union-Find answer!).
2. **`has_path(g, a, b)`** — DFS from a, report whether b was reached.
3. **Path reconstruction** — keep `parent[v]` during the walk; rebuild the a→b path
   by following parents backward.

## Challenge problems

1. **Topological sort** — on a DAG, DFS and push each vertex to a list on *finish*
   (post-order); the reverse is a valid build order. Prove it. Detect a cycle
   (back-edge) and report "not a DAG."
2. **Cycle detection in a directed graph** — three-color DFS (white/gray/black); a
   gray→gray edge is a back-edge = cycle.
3. **Iterative DFS matching recursive order** — with care (push neighbors in reverse,
   mark on pop) you can make the explicit stack reproduce the recursive order.

## Stretch goals

- **Whole-graph DFS forest** — DFS from every component, recording discovery/finish
  times (the basis of Tarjan's SCC algorithm).
- **Strongly connected components** (Kosaraju/Tarjan) — advanced, builds directly on
  DFS finish order.

## Common mistakes

- **No visited set (or checking it too late)** — infinite loop on any cycle. Mark a
  vertex *before* recursing into its neighbors.
- **Leaking the visited/stack scratch** — allocate and free it every call; ASan/
  valgrind will flag a leak even when the answer is right.
- **Unbounded recursion** — a deep graph overflows the call stack; know the O(V)
  depth and reach for the iterative version when V is large.
- **Marking in the wrong place (iterative)** — mark-on-pop can enqueue a vertex
  twice; decide mark-on-push or dedupe on pop, consistently.
- **Assuming a specific visit order** — order depends on adjacency insertion; test
  invariants.

## Debugging tips

- Print the visit order and the visited array; a wrong result is usually a missing
  mark or a missing reverse edge in the *graph* (verify the graph first).
- Reproduce on the smallest cyclic graph (a triangle) — if DFS terminates there, the
  visited logic is sound.
- Compare `dfs` and `dfs_iterative` outputs as *sets*; a mismatch localizes which
  one is wrong.

## Frequently asked questions

**Q: DFS or BFS — how do I choose?**
A: DFS (stack/recursion) dives deep — ideal for "does a path exist," topological
sort, cycle detection, backtracking, and anything about *structure*. BFS (queue,
Lesson 13) expands by distance — ideal for *shortest paths in unweighted graphs* and
level-order problems. Same O(V+E); different frontier discipline, different answers.

**Q: Why does DFS need a graph with a visited set but tree traversal didn't?**
A: A tree has no cycles and one path to each node, so you can't revisit. A general
graph has cycles and multiple paths, so without a visited set you'd loop forever and
redo work. DFS on a tree is just... tree traversal.

**Q: Is recursion or the explicit stack "better"?**
A: Recursion is clearer and usually preferred — until the graph is deep enough to
overflow the call stack (O(V) frames). Then the explicit stack (O(V) heap) is
essential. Knowing both, and when each matters, is the point of building them side by
side.

---

## Definition of done

- [ ] Milestones M2–M4 implemented (M5–M6 recommended; topo sort is superb practice).
- [ ] Full suite passes, especially cycle-termination and correct-component.
- [ ] **`make test` clean under ASan/UBSan; `valgrind` zero leaks** (scratch freed).
- [ ] §6 filled from your reasoning; you can explain why DFS is O(V+E) and where the
      recursion depth risk comes from.
- [ ] `PROGRESS.md` written. Code review requested.

→ Next: [`../13-breadth-first-search/`](../13-breadth-first-search/) — the same graph,
explored by a QUEUE instead of a stack, which visits vertices in order of distance
and so finds shortest paths in unweighted graphs.

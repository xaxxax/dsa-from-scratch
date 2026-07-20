# Project 11 — Graph (adjacency list)

> **You are building:** the most general structure in the course — vertices
> connected by arbitrary edges — the model behind maps, social networks, the web,
> build systems, and course prerequisites. Lessons 12 (DFS) and 13 (BFS) traverse
> exactly this.
>
> **Builds on:** Lesson 2 (linked lists — each vertex's neighbors are a chain) and
> Lesson 1 thinking (an array indexed by vertex). Sets up all the traversal and
> shortest-path work that follows.
>
> **The C leap:** an *array of linked lists*, so it's a per-edge allocation drill —
> one `malloc` per edge (twice for undirected), and `graph_free` must free every
> edge node in every list before the array and the handle. It's the memory patterns
> of Lessons 1 and 2 combined.
>
> **Mentor rule:** diagrams and questions here, not code.

---

## 1. Motivation

### The problem

Trees model hierarchy (one parent, no cycles). But most relationships aren't
hierarchical: cities linked by roads, people who follow each other, web pages that
link, tasks that depend on tasks. Any vertex may connect to any other, in cycles,
in both directions. You need a structure for **arbitrary pairwise relationships** —
and fast answers to "what's connected to X?"

### The idea

A **graph** is a set of **vertices** (numbered `0..n-1`) and **edges** between them.
Two standard representations:

- **Adjacency list** (this lesson): an array indexed by vertex, each slot a linked
  list of that vertex's neighbors. Space **O(V + E)** — you store only edges that
  exist. Great for sparse graphs (most real ones).
- **Adjacency matrix** (a stretch): a `V × V` grid where `m[u][v] = 1` means an
  edge. O(1) edge lookup, but **O(V²)** space even if there are few edges.

### Directed vs. undirected — one decision, everywhere

An **undirected** edge `u–v` means both can reach each other, so it appears in
*both* lists (`adj[u]→v` and `adj[v]→u`). A **directed** edge `u→v` appears in
*one* (`adj[u]→v` only). That single choice — store both directions or one — is the
entire difference between the two graph kinds, and your `graph_add_edge` encodes it.

---

## 2. Visualization

```
 vertices 0..3, UNDIRECTED edges {0-1, 0-2, 1-3}:

   adj[0] ─▶ (1) ─▶ (2) /
   adj[1] ─▶ (0) ─▶ (3) /
   adj[2] ─▶ (0) /
   adj[3] ─▶ (1) /            ( / = NULL, end of list )

 Each undirected edge appears TWICE (0-1 lives in adj[0]→1 AND adj[1]→0).
 A DIRECTED graph with edge 0→1 would put it ONLY in adj[0].
```

### `graph_add_edge(g, 0, 2)` (undirected)

```
 push (2) onto adj[0]'s list, AND push (0) onto adj[2]'s list.
 Two malloc'd AdjNodes, one per direction.
```

### Iterating a vertex's neighbors (what DFS/BFS do)

```c
for (const AdjNode *e = graph_neighbors(g, u); e != NULL; e = e->next) {
    size_t v = e->dest;   /* u has an edge to v */
}
```

**Draw the adjacency list for a small graph and add/remove an edge on paper.** Note
where undirected edges double.

---

## 3. Design

```c
typedef struct AdjNode {           /* one edge: "there is an edge to dest" */
    size_t          dest;
    struct AdjNode *next;          /* next out-edge of the SAME source vertex */
} AdjNode;

typedef struct {
    AdjNode **adj;                 /* array of num_vertices list-heads */
    size_t    num_vertices;
    int       directed;
} Graph;
```

### Design questions

1. **Adjacency list vs. matrix.** List: O(V+E) space, O(degree) to check an edge,
   fast to iterate neighbors. Matrix: O(V²) space, O(1) edge check, wasteful for
   sparse graphs. Which fits a road network (sparse)? A tournament where everyone
   plays everyone (dense)? Justify the list as the default.
2. **The neighbor-iteration contract.** `graph_neighbors(g, u)` returns the head
   `AdjNode*` (or NULL); callers walk `next`. DFS/BFS depend on this exact shape —
   don't change it without updating them. (Exposing the node beats copying neighbors
   into a caller buffer: no size guessing, and you practice walking your own nodes.)
3. **Self-loops & parallel edges.** The raw `add_edge` permits `u==u` and adding the
   same edge twice. Decide your policy (allow, reject, or dedupe) and document it —
   there's no universally right answer; there's a justified one.
4. **Freeing.** `graph_free` must walk every adjacency list freeing each `AdjNode`,
   then free the `adj` array, then the handle — Lesson 2's "free every node" times V.
5. **Fixed vertex count.** Vertices are `0..n-1`, set at construction (like
   Union-Find's universe). Growing the vertex set is a stretch.

---

## 4. Milestones

Scaffold: [`src/graph.c`](src/graph.c) / [`src/graph.h`](src/graph.h).

```
[ ] M0  Build: header + compiling .c with TODO-asserts; make test aborts at graph_new.
[ ] M1  graph_new (malloc the handle; malloc adj = num_vertices heads, all NULL —
        calloc is ideal) / graph_free (free every AdjNode in every list, then adj,
        then the handle). new-then-free: ZERO leaks under ASan.
[ ] M2  graph_add_edge: push a new AdjNode onto adj[u]; if UNDIRECTED, also onto
        adj[v]. Bounds-check u, v < num_vertices.
[ ] M3  graph_has_edge (walk adj[u] looking for dest==v) and graph_neighbors
        (return adj[u]) — the traversal contract.
[ ] M4  graph_degree (length of adj[u]), graph_num_vertices, graph_is_directed.
[ ] M5  (Stretch) adjacency-MATRIX variant behind the same API; weighted edges (add
        a weight to AdjNode); graph_remove_edge; a growable vertex set.
[ ] M6  Full suite green under ASan; valgrind clean. Request review.
```

---

## 5. Testing

```bash
cd projects/11-graph
make test
make && valgrind --leak-check=full --error-exitcode=1 ./out_test
```

Starter: [`tests/test_graph.c`](tests/test_graph.c).

### Edge cases (test all)

- **Undirected symmetry:** after `add_edge(0,1)` on an undirected graph, *both*
  `has_edge(0,1)` and `has_edge(1,0)` are true; on a directed graph only `has_edge
  (0,1)`.
- **Degree counts** match the edges you added (mind the doubling for undirected).
- **Self-loop** `add_edge(2,2)` — behaves per your documented policy.
- **No spurious edges:** `has_edge` is false for pairs you never connected.
- **The unsigned trap:** an out-of-range vertex index is rejected.
- **Leak audit:** a graph with many edges frees every `AdjNode` — valgrind zero.
  `graph_free` on a fresh graph.

---

## 6. Complexity Analysis

Derive first. V = vertices, E = edges.

| Operation | Adjacency list | Adjacency matrix |
|-----------|:--------------:|:----------------:|
| `add_edge` | ? | ? |
| `has_edge` | ? | ? |
| iterate neighbors of u | ? | ? |
| space | ? | ? |

<details>
<summary>Answers.</summary>

- List: `add_edge` **O(1)** (push on the head); `has_edge` **O(degree(u))**;
  iterate neighbors **O(degree(u))**; space **O(V + E)**.
- Matrix: `add_edge`/`has_edge` **O(1)**; iterate neighbors **O(V)** (scan a whole
  row even if the vertex has one neighbor); space **O(V²)**.
- The list wins for **sparse** graphs (E ≪ V²) — which is most real graphs. The
  matrix wins when you do constant-time edge checks on a **dense** graph. Traversals
  (DFS/BFS) are **O(V + E)** on a list, **O(V²)** on a matrix — another reason the
  list is the default.
</details>

---

## 7. Real-World Usage

- **Maps & routing:** intersections = vertices, roads = weighted edges (Dijkstra,
  A\*).
- **Social networks:** people = vertices, follows/friendships = edges (directed vs.
  undirected matters!).
- **The web:** pages = vertices, hyperlinks = directed edges (PageRank runs on this).
- **Build systems / package managers:** targets = vertices, dependencies = directed
  edges; topological sort (a DFS application) gives a valid build order.
- **Compilers:** control-flow and dependency graphs; **networks:** routing tables.

---

## 8. LeetCode

**No solutions here.** (Most graph problems combine this structure with Lesson 12/13
traversals — build the representation first.)

1. **[Medium] 1971. Find if Path Exists in Graph** — build the adjacency list, then
   a traversal.
2. **[Medium] 133. Clone Graph** — forces you to understand adjacency + visited.
3. **[Medium] 207. Course Schedule** — cycle detection in a directed graph (topo
   sort).
4. **[Medium] 323. Number of Connected Components** — components (also a Union-Find
   problem — compare!).
5. **[Medium] 743. Network Delay Time** — weighted directed graph (Dijkstra; needs
   your Lesson 9 heap).

---

## Exercises

1. **`graph_print`** — dump each vertex and its neighbor list. Your first debugging
   tool for every graph problem to come.
2. **Edge & vertex counts** — `graph_num_edges` (careful: undirected counts each
   edge once, but it appears twice in the lists).
3. **Build from an edge list** — a helper that takes `(n, directed, pairs[])` and
   returns a ready graph; you'll reuse it constantly in LeetCode.

## Challenge problems

1. **Adjacency-matrix twin.** Implement the same public API backed by a `V×V`
   matrix. Swap it under DFS/BFS unchanged (thanks to the shared contract) and
   compare memory + traversal cost on sparse vs. dense graphs.
2. **Weighted edges.** Add a `weight` to `AdjNode`; expose `graph_add_weighted_edge`.
   This is the prerequisite for Dijkstra / MST.
3. **`graph_remove_edge`** — unlink the `AdjNode` from `adj[u]` (and `adj[v]` if
   undirected), free it, keep degrees correct. Lesson 2 removal, applied per vertex.

## Stretch goals

- **Growable vertex set** — add vertices after construction (resize `adj`, like the
  dynamic array).
- **Represent a real dataset** (e.g., a small road or friendship graph) and answer
  reachability questions once you have DFS/BFS.

## Common mistakes

- **Undirected edge added one-way** — forgetting the second push means `has_edge
  (v,u)` fails and traversals miss half the graph.
- **Leaking `AdjNode`s** — `graph_free` must walk *every* list; freeing only the
  `adj` array leaks all the edges. valgrind names them.
- **Unsigned vertex index** — bounds-check `u, v < num_vertices`.
- **Confusing degree with edge count** for undirected graphs (sum of degrees = 2E).
- **Assuming neighbor order** — pushing on the head reverses insertion order; don't
  write tests that depend on a particular order unless you sort.

## Debugging tips

- Build `graph_print` before anything else; nearly every graph bug is visible once
  you can see the adjacency lists.
- Test the smallest graphs: 1 vertex/0 edges, 2 vertices/1 edge (check both
  directions for undirected).
- For traversal bugs later, first confirm the *graph* is right — a wrong answer is
  often a missing reverse edge, not a broken DFS.
- valgrind after adding many edges — the per-edge `malloc`/`free` is where leaks
  hide.

## Frequently asked questions

**Q: When would I choose the matrix over the list?**
A: When the graph is **dense** (E close to V²) and you do lots of "is there an edge
u→v?" checks — the matrix answers in O(1) with good cache behavior. For sparse
graphs (most of them), the list's O(V+E) space and fast neighbor iteration win. Know
both; pick by density and access pattern.

**Q: Why expose the `AdjNode` instead of copying neighbors into a buffer?**
A: A buffer forces the caller to guess a size (the degree), and copies memory for no
reason. Returning the list head lets DFS/BFS walk in place — and it makes you
practice iterating the linked nodes you allocated. The trade-off is that callers must
not mutate the list while iterating (a real-world iterator-invalidation lesson).

**Q: Directed or undirected — how do I decide?**
A: By the relationship. "Is friends with" is symmetric → undirected. "Follows" /
"depends on" / "links to" is asymmetric → directed. Getting this wrong silently
changes every reachability answer, so decide deliberately per problem.

---

## Definition of done

- [ ] Milestones M1–M4 implemented (M5–M6 recommended).
- [ ] Full suite passes, every §5 edge case (undirected symmetry, degrees, leaks).
- [ ] **`make test` clean under ASan/UBSan; `valgrind` zero leaks** across all the
      per-edge allocations.
- [ ] §6 table filled from your reasoning; you can explain list vs. matrix trade-offs
      and why traversals are O(V+E).
- [ ] `PROGRESS.md` written. Code review requested.

→ Next: [`../12-depth-first-search/`](../12-depth-first-search/) — the first
traversal. You'll walk this adjacency list as deep as possible before backtracking,
using a visited set so cycles don't trap you, and discover that the recursion stack
*is* a stack.

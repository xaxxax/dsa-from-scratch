/*
 * graph.h — the CONTRACT for your graph (adjacency-list representation).
 *
 * A graph is a set of VERTICES (numbered 0 .. num_vertices-1) connected by EDGES.
 * Unlike a tree, there's no root and no "one parent" rule — any vertex may connect
 * to any other. This is how you model roads, friendships, web links, course
 * prerequisites, package dependencies: arbitrary relationships.
 *
 * We store it as an ADJACENCY LIST: an array indexed by vertex, where each slot
 * holds a linked list of that vertex's out-neighbors. Each edge is one malloc'd
 * AdjNode — so this lesson is also a per-edge allocation drill (contrast the ONE
 * malloc of a dynamic array).
 *
 *   vertices 0..3, undirected edges {0-1, 0-2, 1-3}:
 *
 *     adj[0] ─▶ (1) ─▶ (2) /
 *     adj[1] ─▶ (0) ─▶ (3) /
 *     adj[2] ─▶ (0) /
 *     adj[3] ─▶ (1) /            (/ = NULL, end of list)
 *
 *   An UNDIRECTED edge 0-1 appears TWICE: once in adj[0] (→1) and once in adj[1]
 *   (→0). A DIRECTED edge u→v appears ONLY in adj[u]. That single choice — store
 *   both directions or one — is the whole difference between the two graph kinds.
 *
 * NEIGHBOR ITERATION — THE STABLE CONTRACT DFS/BFS DEPEND ON:
 *   graph_neighbors(g, u) returns the HEAD of u's adjacency list (or NULL). Walk it
 *   with the AdjNode `next` pointer:
 *
 *       for (const AdjNode *e = graph_neighbors(g, u); e != NULL; e = e->next) {
 *           size_t v = e->dest;      // u has an edge to v
 *           ...
 *       }
 *
 *   We expose the node type (rather than copying neighbors into a caller buffer) so
 *   traversals never have to guess a buffer size, and so you practice walking the
 *   linked nodes you allocated. Lessons 12 (DFS) and 13 (BFS) are written against
 *   exactly this function — do NOT change its shape without updating them.
 */
#ifndef GRAPH_H
#define GRAPH_H

#include <stddef.h>   /* size_t */

/* One edge in an adjacency list: "there is an edge to `dest`". `next` chains to the
 * source vertex's other out-edges. You allocate one of these per graph_add_edge
 * (twice, for an undirected edge). */
typedef struct AdjNode {
    size_t          dest;   /* the neighbor vertex this edge points at */
    struct AdjNode *next;   /* next out-edge of the SAME source vertex, or NULL */
} AdjNode;

/* The graph handle. `adj` is an array of num_vertices list-heads (adj[i] is the
 * head of vertex i's out-edge list, or NULL if i has no out-edges). */
typedef struct {
    AdjNode **adj;           /* array of `num_vertices` adjacency-list heads */
    size_t    num_vertices;  /* vertices are the fixed range 0 .. num_vertices-1 */
    int       directed;      /* 1 = directed (u→v only), 0 = undirected (both ways) */
} Graph;

/* ---- lifecycle ---------------------------------------------------------- */

/* Allocate a graph on `num_vertices` vertices (0..num_vertices-1), directed if the
 * flag is nonzero. All adjacency lists start empty. NULL on allocation failure. */
Graph *graph_new(size_t num_vertices, int directed);

/* Free every adjacency node in every list, then the adj array, then the handle.
 * Safe on NULL. */
void   graph_free(Graph *g);

/* ---- building ----------------------------------------------------------- */

/* Add an edge u→v. If the graph is UNDIRECTED, also add v→u. u and v must both be
 * < num_vertices. Self-loops (u == v) and parallel edges (adding the same edge
 * twice) are permitted by this raw API — the README asks you to decide your policy. */
void   graph_add_edge(Graph *g, size_t u, size_t v);

/* ---- querying ----------------------------------------------------------- */

/* 1 if an edge u→v exists, else 0. Walks u's adjacency list. */
int    graph_has_edge(const Graph *g, size_t u, size_t v);

/* Head of vertex u's out-edge list (or NULL). THE neighbor-iteration API for
 * DFS/BFS — see the block comment above. */
const AdjNode *graph_neighbors(const Graph *g, size_t u);

/* Out-degree of u: the number of edges leaving u (length of its adjacency list). */
size_t graph_degree(const Graph *g, size_t u);

/* Number of vertices (the fixed count fixed at construction). */
size_t graph_num_vertices(const Graph *g);

/* 1 if the graph was built directed, else 0. */
int    graph_is_directed(const Graph *g);

#endif /* GRAPH_H */

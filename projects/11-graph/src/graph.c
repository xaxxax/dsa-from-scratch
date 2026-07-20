/*
 * graph.c — YOUR graph (adjacency-list representation).
 *
 * SCAFFOLD (README §4). Implement milestone by milestone under ASan. The memory
 * lesson here is PER-EDGE allocation: each edge is a malloc'd AdjNode, and an
 * undirected edge is TWO of them. graph_free must release every node in every list
 * before it frees the array and the handle — miss one and ASan names the leak.
 *
 * Invariants:
 *   - vertices are exactly 0 .. num_vertices-1 (a fixed range, never grows here)
 *   - adj[i] is the head of vertex i's out-edge list, or NULL if it has none
 *   - an undirected edge u-v is represented by TWO nodes: v in adj[u] AND u in adj[v]
 *   - degree(u) == number of nodes reachable from adj[u] via `next`
 */
#include "graph.h"

#include <assert.h>
#include <stdlib.h>   /* malloc, free */

/* ---- private helper you will write --------------------------------------- */

/* Allocate one adjacency node pointing at `dest`, linked in front of `next`.
 * Writing this once keeps every add path honest (and makes prepending trivial:
 * new node's next = old head, then head = new node). Returns NULL on malloc
 * failure. NOTE: until graph_add_edge calls this at M2, the compiler will warn
 * "adj_node_new defined but not used" — that's expected at M0/M1. */
static AdjNode *adj_node_new(size_t dest, AdjNode *next) {
    (void)dest; (void)next;
    /* TODO (M2): malloc an AdjNode; set dest and next; check malloc; return it. */
    assert(0 && "TODO: implement adj_node_new (README M2)");
    return NULL;
}

/* ------------------------------------------------------------------ M1 ---- */

Graph *graph_new(size_t num_vertices, int directed) {
    (void)num_vertices; (void)directed;
    /* TODO (M1): malloc the Graph. Allocate adj = an array of num_vertices AdjNode*
     * ALL set to NULL (calloc is perfect — every vertex starts with an empty list).
     * Store num_vertices and directed (normalize to 0/1). Check every allocation;
     * if the adj array fails, free the Graph before returning NULL. Consider what
     * num_vertices == 0 should do. */
    assert(0 && "TODO: implement graph_new (README M1)");
    return NULL;
}

void graph_free(Graph *g) {
    (void)g;
    /* TODO (M1): if g is NULL, nothing. Otherwise for EACH vertex i, walk adj[i]
     * freeing every node (save `next` BEFORE free(cur) — use-after-free otherwise,
     * same rule as Lesson 2). Then free(g->adj). Then free(g). Three layers; miss
     * any and ASan reports the leak. */
    assert(0 && "TODO: implement graph_free (README M1)");
}

/* ------------------------------------------------------------------ M2 ---- */

void graph_add_edge(Graph *g, size_t u, size_t v) {
    (void)g; (void)u; (void)v;
    /* TODO (M2): bounds-check u < num_vertices and v < num_vertices (unsigned trap:
     * only the >= check is needed). Prepend v to adj[u]:
     *     g->adj[u] = adj_node_new(v, g->adj[u]);
     * If the graph is UNDIRECTED, ALSO prepend u to adj[v]. (Prepending is O(1);
     * appending would be O(degree) — order within a list doesn't matter for
     * correctness, only which end you insert at for speed.)
     * Decide your policy on self-loops (u == v) and parallel edges — see README. */
    assert(0 && "TODO: implement graph_add_edge (README M2)");
}

/* ------------------------------------------------------------------ M3 ---- */

int graph_has_edge(const Graph *g, size_t u, size_t v) {
    (void)g; (void)u; (void)v;
    /* TODO (M3): bounds-check u (and treat out-of-range as "no edge", or assert —
     * your policy). Walk adj[u]; return 1 if any node's dest == v, else 0. This is
     * O(degree(u)) — the adjacency list's weakness vs. a matrix's O(1) lookup. */
    assert(0 && "TODO: implement graph_has_edge (README M3)");
    return 0;
}

const AdjNode *graph_neighbors(const Graph *g, size_t u) {
    (void)g; (void)u;
    /* TODO (M3): bounds-check u, then return g->adj[u] (the list head, maybe NULL).
     * This is THE function DFS/BFS iterate with — keep it a trivial accessor. */
    assert(0 && "TODO: implement graph_neighbors (README M3)");
    return NULL;
}

/* ------------------------------------------------------------------ M4 ---- */

size_t graph_degree(const Graph *g, size_t u) {
    (void)g; (void)u;
    /* TODO (M4): bounds-check u; walk adj[u] counting nodes; return the count. */
    assert(0 && "TODO: implement graph_degree (README M4)");
    return 0;
}

size_t graph_num_vertices(const Graph *g) {
    (void)g;
    /* TODO (M4): return g->num_vertices. */
    assert(0 && "TODO: implement graph_num_vertices (README M4)");
    return 0;
}

int graph_is_directed(const Graph *g) {
    (void)g;
    /* TODO (M4): return g->directed. */
    assert(0 && "TODO: implement graph_is_directed (README M4)");
    return 0;
}

/*
 * dfs.h — the CONTRACT for depth-first search over your Lesson 11 graph.
 *
 * DFS explores as DEEP as possible along each branch before backtracking. From a
 * start vertex it dives to a neighbor, then that neighbor's neighbor, and so on,
 * only backing up when it hits a dead end (or an already-visited vertex).
 *
 *   graph:  0 — 1 — 3          DFS(0) might visit: 0, 1, 3, 2
 *           |   |              (dive 0→1→3, backtrack, then 0→2)
 *           2 ——+
 *
 * A VISITED marker is essential: without it, any cycle traps DFS in an infinite
 * loop. The recursion stack IS a stack — this is Lesson 4 made implicit. (You'll
 * also build an explicit-stack iterative version, proving the equivalence.)
 *
 * These functions COMPOSE the graph: they call graph_neighbors()/graph_num_vertices()
 * and never peek inside the Graph struct. The `out_order` buffer is CALLER-OWNED and
 * must hold at least graph_num_vertices() elements; the function fills it with the
 * vertices in visit order and returns how many it visited.
 */
#ifndef DFS_H
#define DFS_H

#include <stddef.h>
#include "graph.h"   /* -I../11-graph/src */

/* Recursive DFS from `start`. Fills out_order[0..return-1] with the visit order.
 * Returns the number of vertices reached (the size of start's connected part,
 * for an undirected graph). out_order must have room for num_vertices. */
size_t dfs(const Graph *g, size_t start, size_t *out_order);

/* Same traversal, but with an EXPLICIT stack instead of recursion (milestone M4).
 * Proves the recursion stack was a stack all along; also avoids call-stack
 * overflow on huge/deep graphs. */
size_t dfs_iterative(const Graph *g, size_t start, size_t *out_order);

#endif /* DFS_H */

/*
 * bfs.h — the CONTRACT for breadth-first search over your Lesson 11 graph.
 *
 * BFS explores in RINGS of increasing distance from the start: first the start,
 * then all its neighbors (distance 1), then all THEIR unseen neighbors (distance
 * 2), and so on. A FIFO QUEUE enforces that order — the exact opposite of DFS's
 * stack.
 *
 *   graph:  0 — 1 — 3          BFS(0) visits by distance:
 *           |   |                 dist 0: 0
 *           2 ——+                 dist 1: 1, 2   (0's neighbors)
 *                                 dist 2: 3      (reached via 1 or 2)
 *           order: 0, 1, 2, 3
 *
 * THE HEADLINE PROPERTY: because BFS reaches vertices in nondecreasing distance
 * order, the FIRST time it reaches a vertex is along a SHORTEST path (fewest
 * edges). That's why BFS — not DFS — solves shortest paths in UNWEIGHTED graphs.
 *
 * Composes the graph via graph_neighbors()/graph_num_vertices(); never peeks
 * inside Graph. Scratch (visited, the queue) is allocated and freed internally.
 */
#ifndef BFS_H
#define BFS_H

#include <stddef.h>
#include "graph.h"   /* -I../11-graph/src */

/* BFS from `start`. Fills out_order[0..return-1] with the visit order (which is
 * distance order). Returns the number of vertices reached. out_order must hold at
 * least num_vertices. */
size_t bfs(const Graph *g, size_t start, size_t *out_order);

/* Shortest-path distances (in EDGES) from `start` to every vertex, written into
 * the caller's `dist` array (length num_vertices). Unreachable vertices get -1.
 * This is BFS's signature application. */
void bfs_distances(const Graph *g, size_t start, int *dist);

#endif /* BFS_H */

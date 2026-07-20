/*
 * bfs.c — YOUR breadth-first search. SCAFFOLD (README §4).
 *
 * SELF-CONTAINED apart from the graph: allocate your own FIFO queue (a size_t
 * array with head/tail indices — a ring buffer, or a simple array since each
 * vertex is enqueued at most once so num_vertices slots suffice) and your own
 * visited array. Free both.
 *
 * THE ONE RULE THAT MAKES BFS CORRECT: mark a vertex visited WHEN YOU ENQUEUE it,
 * not when you dequeue it. Marking on dequeue lets a vertex get enqueued several
 * times (via different neighbors) before it's processed — bloating the queue and
 * breaking the distance guarantee. Enqueue ⇒ mark, together, always.
 *
 * Neighbor iteration (Lesson 11 contract):
 *     for (const AdjNode *e = graph_neighbors(g, u); e; e = e->next) { ... e->dest ... }
 */
#include "bfs.h"

#include <assert.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ M2 ---- */

size_t bfs(const Graph *g, size_t start, size_t *out_order) {
    (void)g; (void)start; (void)out_order;
    /* TODO (M2): bounds-check start. Allocate visited (calloc num_vertices) and a
     * queue (size_t array, num_vertices slots, head=tail=0). Enqueue start AND
     * mark visited[start]=1. size_t count=0.
     * While the queue is non-empty:
     *   u = dequeue; out_order[count++] = u;
     *   for each neighbor v of u: if !visited[v] { visited[v]=1; enqueue v; }
     * Free the queue and visited; return count.
     * Remember: mark ON ENQUEUE (see the file header). */
    assert(0 && "TODO: implement bfs (README M2)");
    return 0;
}

/* ------------------------------------------------------------------ M3 ---- */

void bfs_distances(const Graph *g, size_t start, int *dist) {
    (void)g; (void)start; (void)dist;
    /* TODO (M3): initialize dist[v] = -1 for all v (unreachable). Set dist[start]
     * = 0, enqueue start. While the queue is non-empty: u = dequeue; for each
     * neighbor v with dist[v] == -1: dist[v] = dist[u] + 1; enqueue v.
     * Here `dist[v] != -1` doubles as the visited flag — a vertex gets its final
     * (shortest) distance the first time it's reached, which is why BFS solves
     * unweighted shortest paths. Free your queue. */
    assert(0 && "TODO: implement bfs_distances (README M3)");
}

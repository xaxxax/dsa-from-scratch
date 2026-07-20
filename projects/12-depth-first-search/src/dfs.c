/*
 * dfs.c — YOUR depth-first search. SCAFFOLD (README §4).
 *
 * The traversal is SELF-CONTAINED apart from the graph: allocate your own
 * `visited` array (one bool per vertex) and, for the iterative version, your own
 * explicit stack (a plain size_t array). Free whatever you allocate — even a
 * read-only traversal must not leak its scratch memory.
 *
 * Neighbor iteration uses the Lesson 11 contract:
 *     for (const AdjNode *e = graph_neighbors(g, u); e; e = e->next) {
 *         size_t v = e->dest;
 *         ...
 *     }
 */
#include "dfs.h"

#include <assert.h>
#include <stdlib.h>

/* Recursive core: visit u, record it, then recurse into each unvisited neighbor.
 * Unused until dfs() calls it — expected "unused" warning until M2/M3. */
static void dfs_visit(const Graph *g, size_t u, int *visited,
                      size_t *out_order, size_t *count) {
    (void)g; (void)u; (void)visited; (void)out_order; (void)count;
    /* TODO (M3): mark visited[u] = 1; out_order[(*count)++] = u; then for each
     * neighbor v of u, if !visited[v], dfs_visit(g, v, ...). That's the whole
     * algorithm — depth-first falls out of recursing before moving to the next
     * neighbor. */
    assert(0 && "TODO: implement dfs_visit (README M3)");
}

/* ------------------------------------------------------------------ M2 ---- */

size_t dfs(const Graph *g, size_t start, size_t *out_order) {
    (void)g; (void)start; (void)out_order;
    /* TODO (M2): allocate visited = calloc(num_vertices, sizeof(int)) (all 0);
     * size_t count = 0; call dfs_visit(g, start, visited, out_order, &count);
     * free(visited); return count. Bounds-check start < num_vertices.
     * NOTE: this visits only start's connected component. Visiting the WHOLE
     * graph (looping over all vertices, DFS-ing any unvisited one) is a stretch. */
    assert(0 && "TODO: implement dfs (README M2/M3)");
    return 0;
}

/* ------------------------------------------------------------------ M4 ---- */

size_t dfs_iterative(const Graph *g, size_t start, size_t *out_order) {
    (void)g; (void)start; (void)out_order;
    /* TODO (M4): an EXPLICIT stack (size_t array of capacity num_vertices) instead
     * of recursion. Push start. While the stack isn't empty: pop u; if visited,
     * skip; else mark+record u, then push each neighbor. Free the stack (and a
     * visited array) at the end.
     * SUBTLETY: the visit ORDER may differ from the recursive version depending on
     * whether you mark-on-push or mark-on-pop and the neighbor order — both are
     * valid DFS orders. The README explains why. */
    assert(0 && "TODO: implement dfs_iterative (README M4)");
    return 0;
}

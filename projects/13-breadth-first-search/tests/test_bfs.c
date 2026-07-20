/*
 * test_bfs.c — YOUR test harness for Project 13.
 *
 * Run:        make test
 * Leak audit: make && valgrind --leak-check=full --error-exitcode=1 ./out_test
 *
 * Exercises BFS over your Lesson 11 graph. A TODO-assert from graph.c means:
 * finish Lesson 11 first. BFS order within a distance ring can vary, so we test
 * the distance guarantee and reachability, plus that the start comes first.
 */
#include "test_utils.h"
#include "bfs.h"
#include "graph.h"

int main(void) {
    /* undirected, 6 vertices:
     *   0-1, 0-2, 1-3, 2-3, 3-4     (5 is isolated)
     *   distances from 0: 0→0, 1→1, 2→1, 3→2, 4→3, 5→unreachable
     */
    Graph *g = graph_new(6, 0);
    graph_add_edge(g, 0, 1);
    graph_add_edge(g, 0, 2);
    graph_add_edge(g, 1, 3);
    graph_add_edge(g, 2, 3);
    graph_add_edge(g, 3, 4);

    size_t order[6];
    size_t n = bfs(g, 0, order);
    CHECK_INT(5, (int)n, "BFS(0) reaches 5 vertices (all but isolated 5)");
    CHECK_INT(0, (int)order[0], "BFS starts at the start vertex");

    int seen[6] = {0};
    for (size_t i = 0; i < n; i++) seen[order[i]] = 1;
    CHECK(!seen[5], "isolated vertex 5 not reached");

    /* the headline: shortest-path distances in edges */
    int dist[6];
    bfs_distances(g, 0, dist);
    CHECK_INT(0, dist[0], "dist(0) = 0");
    CHECK_INT(1, dist[1], "dist(1) = 1");
    CHECK_INT(1, dist[2], "dist(2) = 1");
    CHECK_INT(2, dist[3], "dist(3) = 2 (via 1 or 2, not the long way)");
    CHECK_INT(3, dist[4], "dist(4) = 3");
    CHECK_INT(-1, dist[5], "dist(5) = -1 (unreachable)");

    graph_free(g);

    /* ====================================================================== */
    /* TODO: a graph with a CYCLE — BFS must terminate; verify BFS distance is  */
    /* the MINIMUM even when a longer path exists; single-vertex graph; a       */
    /* directed graph where reachability is asymmetric.                         */
    /* ====================================================================== */

    return test_summary();
}

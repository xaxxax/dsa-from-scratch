/*
 * test_dfs.c — YOUR test harness for Project 12.
 *
 * Run:        make test
 * Leak audit: make && valgrind --leak-check=full --error-exitcode=1 ./out_test
 *
 * Exercises DFS over your Lesson 11 graph. A TODO-assert from graph.c means:
 * finish Lesson 11 first. DFS visit order can vary with adjacency order, so we
 * test INVARIANTS (start first, correct count, only reachable vertices), not an
 * exact sequence.
 */
#include "test_utils.h"
#include "dfs.h"
#include "graph.h"

int main(void) {
    /* undirected graph, 6 vertices:
     *   0-1, 0-2, 1-3, 2-3, 4-5   (component A = {0,1,2,3}, component B = {4,5})
     */
    Graph *g = graph_new(6, 0);
    graph_add_edge(g, 0, 1);
    graph_add_edge(g, 0, 2);
    graph_add_edge(g, 1, 3);
    graph_add_edge(g, 2, 3);
    graph_add_edge(g, 4, 5);

    size_t order[6];

    /* DFS from 0 reaches exactly component A = {0,1,2,3} */
    size_t n = dfs(g, 0, order);
    CHECK_INT(4, (int)n, "DFS(0) visits 4 vertices (component A)");
    CHECK_INT(0, (int)order[0], "DFS starts at the start vertex");

    /* every visited vertex is in {0,1,2,3}; 4 and 5 are NOT reached */
    int seen[6] = {0};
    for (size_t i = 0; i < n; i++) seen[order[i]] = 1;
    CHECK(seen[0] && seen[1] && seen[2] && seen[3], "all of A visited");
    CHECK(!seen[4] && !seen[5], "component B not reached from 0");

    /* DFS from 4 reaches exactly {4,5} */
    size_t n2 = dfs(g, 4, order);
    CHECK_INT(2, (int)n2, "DFS(4) visits 2 vertices (component B)");
    CHECK_INT(4, (int)order[0], "DFS(4) starts at 4");

    graph_free(g);

    /* ====================================================================== */
    /* TODO: a graph with a CYCLE (0-1,1-2,2-0) — DFS must terminate, visiting */
    /* each once (not loop forever); a single-vertex graph; compare dfs vs     */
    /* dfs_iterative reach the same SET of vertices; directed graph reachability*/
    /* ====================================================================== */

    return test_summary();
}

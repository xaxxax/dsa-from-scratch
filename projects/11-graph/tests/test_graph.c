/*
 * test_graph.c — YOUR test harness for Project 11.
 *
 * Run:        make test
 * Leak audit: make && valgrind --leak-check=full --error-exitcode=1 ./out_test
 *
 * Write tests AS YOU GO, one milestone at a time. A few starters show the rhythm;
 * the rest are TODOs mapped to the milestones and edge cases in README §5. The
 * graph you build here is also what Lessons 12 (DFS) and 13 (BFS) will traverse, so
 * getting neighbor iteration right matters beyond this lesson.
 */
#include "test_utils.h"   /* found via -I../../docs */
#include "graph.h"        /* found via -Isrc */

/* Small helper local to the test: count neighbors of u by walking the list, so we
 * exercise the exact iteration DFS/BFS will use. */
static size_t count_neighbors(const Graph *g, size_t u) {
    size_t n = 0;
    for (const AdjNode *e = graph_neighbors(g, u); e != NULL; e = e->next) n++;
    return n;
}

int main(void) {

    /* --- M1/M4: construction ------------------------------------------------ */
    Graph *g = graph_new(4, 0 /* undirected */);
    CHECK_INT(4, (long)graph_num_vertices(g), "graph has 4 vertices");
    CHECK_INT(0, graph_is_directed(g), "graph is undirected");
    CHECK_INT(0, (long)graph_degree(g, 0), "vertex 0 starts with degree 0");

    /* --- M2/M3: add edges, query -------------------------------------------- */
    graph_add_edge(g, 0, 1);
    graph_add_edge(g, 0, 2);
    graph_add_edge(g, 1, 3);

    CHECK(graph_has_edge(g, 0, 1), "edge 0-1 exists");
    CHECK(graph_has_edge(g, 1, 0), "undirected: 1-0 exists too");
    CHECK(!graph_has_edge(g, 0, 3), "no edge 0-3");

    CHECK_INT(2, (long)graph_degree(g, 0), "vertex 0 has degree 2 (to 1 and 2)");
    CHECK_INT(2, (long)count_neighbors(g, 1), "vertex 1 has 2 neighbors (0 and 3)");
    CHECK_INT(1, (long)graph_degree(g, 2), "vertex 2 has degree 1 (to 0)");

    graph_free(g);   /* frees every AdjNode, the array, then the handle — ZERO leaks */

    /* --- directed graph: edge goes ONE way ---------------------------------- */
    Graph *d = graph_new(3, 1 /* directed */);
    graph_add_edge(d, 0, 1);
    CHECK(graph_has_edge(d, 0, 1), "directed: 0->1 exists");
    CHECK(!graph_has_edge(d, 1, 0), "directed: 1->0 does NOT exist");
    CHECK_INT(1, (long)graph_degree(d, 0), "out-degree of 0 is 1");
    CHECK_INT(0, (long)graph_degree(d, 1), "out-degree of 1 is 0");
    graph_free(d);

    /* ====================================================================== */
    /* TODO: add more — a bigger graph, self-loop policy (add_edge(g,2,2)),     */
    /* parallel edges (add 0-1 twice: does degree become 2? your policy),      */
    /* graph_free on a fresh graph (no edges), and the unsigned bounds trap    */
    /* (graph_add_edge with an out-of-range vertex — test your chosen policy). */
    /* ====================================================================== */

    return test_summary();
}

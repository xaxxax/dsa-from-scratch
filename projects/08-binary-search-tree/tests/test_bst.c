/*
 * test_bst.c — YOUR test harness for Project 8 (Binary Search Tree).
 *
 * Run:        make test
 * Leak audit: make && valgrind --leak-check=full --error-exitcode=1 ./out_test
 *
 * The leak audit matters: a BST is many separate mallocs, and the #1 free bug is
 * dropping a subtree (free the parent before its children). valgrind names the
 * malloc whose free you missed.
 *
 * At M0 this aborts at the first TODO (bst_new). Uncomment / add checks as you
 * complete each milestone.
 */
#include "test_utils.h"
#include "bst.h"

int main(void) {
    BST *t = bst_new();
    CHECK_INT(0, (int)bst_size(t), "new tree size 0");
    CHECK(!bst_contains(t, 42), "contains on empty returns 0");

    /* Insert the classic balanced shape:
     *            50
     *          /    \
     *        30      70
     *       /  \    /  \
     *      20  40  60  80                                              */
    int keys[] = { 50, 30, 70, 20, 40, 60, 80 };
    for (size_t i = 0; i < sizeof keys / sizeof keys[0]; i++)
        bst_insert(t, keys[i]);
    CHECK_INT(7, (int)bst_size(t), "size 7 after seven inserts");

    /* Re-inserting an existing key is a no-op — no duplicates. */
    bst_insert(t, 50);
    CHECK_INT(7, (int)bst_size(t), "size still 7 after duplicate insert");

    /* --- contains ---------------------------------------------------------- */
    CHECK(bst_contains(t, 20), "contains(20)");
    CHECK(bst_contains(t, 80), "contains(80)");
    CHECK(bst_contains(t, 50), "contains(50) (root)");
    CHECK(!bst_contains(t, 55), "!contains(55)");

    /* --- in-order == sorted ----------------------------------------------- */
    bst_key out[16];
    size_t n = bst_inorder(t, out);
    CHECK_INT(7, (int)n, "inorder wrote 7 keys");
    int sorted = 1;
    for (size_t i = 1; i < n; i++)
        if (out[i - 1] >= out[i]) { sorted = 0; break; }
    CHECK(sorted, "in-order traversal is strictly ascending (BST is ordered)");
    CHECK_INT(20, out[0], "smallest key first");
    CHECK_INT(80, out[6], "largest key last");

    /* --- height ------------------------------------------------------------ */
    /* Balanced tree of 7 nodes over 3 levels → height 2 edges (root→leaf).
     * (If you chose a different empty/leaf convention, adjust this expectation.) */
    CHECK_INT(2, (int)bst_height(t), "height 2 for the balanced 7-node tree");

    /* --- remove: the three cases ------------------------------------------ */
    CHECK(bst_remove(t, 20), "remove(20) leaf returns 1");   /* (a) leaf         */
    CHECK(!bst_contains(t, 20), "20 gone");
    CHECK_INT(6, (int)bst_size(t), "size 6 after removing a leaf");

    CHECK(bst_remove(t, 30), "remove(30) one-child returns 1"); /* (b) one child (40 remains) */
    CHECK(!bst_contains(t, 30), "30 gone");
    CHECK(bst_contains(t, 40), "40 survived its parent's removal");
    CHECK_INT(5, (int)bst_size(t), "size 5 after one-child removal");

    CHECK(bst_remove(t, 50), "remove(50) two-children (root) returns 1"); /* (c) two children */
    CHECK(!bst_contains(t, 50), "50 gone");
    CHECK_INT(4, (int)bst_size(t), "size 4 after two-children removal");

    /* Tree must still be a valid BST (in-order still sorted) after deletes. */
    n = bst_inorder(t, out);
    int still_sorted = 1;
    for (size_t i = 1; i < n; i++)
        if (out[i - 1] >= out[i]) { still_sorted = 0; break; }
    CHECK(still_sorted, "still sorted in-order after three deletes");

    CHECK(!bst_remove(t, 999), "remove missing key returns 0");

    bst_free(t);   /* frees every remaining node post-order — ZERO leaks */

    /* ====================================================================== */
    /* TODO: add these edge cases as you go:                                   */
    /*   - bst_free on a FRESH (empty) tree — no crash, no leak.               */
    /*   - Degenerate insert: insert 1,2,3,4,5 in order; height should be 4    */
    /*     (a linked list!) — this is the O(n) worst case; prove it to         */
    /*     yourself, then note why AVL/Red-Black exist.                        */
    /*   - Remove the root when it is the ONLY node → empty tree, size 0.      */
    /*   - Remove down to empty, then insert again — the tree must recover.    */
    /*   - Two-children delete where the successor itself has a right child.   */
    /* ====================================================================== */

    return test_summary();
}

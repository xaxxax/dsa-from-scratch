/*
 * bst.h — the CONTRACT for your binary search tree (int keys).
 *
 * Read this top to bottom before writing a line of bst.c. It fixes the types and
 * the signatures; the bodies are yours (see the README milestones).
 *
 * A BST is a tree of separately-allocated nodes obeying ONE invariant, at EVERY
 * node: every key in the left subtree is smaller, every key in the right subtree
 * is larger. That single rule is what turns "search" into "go left or right,"
 * halving the problem at each step.
 *
 *              (50)
 *             /    \
 *          (30)    (70)
 *          /  \     /  \
 *       (20)(40) (60)(80)
 *
 *   contains(60): 60 > 50 → right → 60 < 70 → left → found.  (3 hops for 7 nodes)
 *
 * Two things C forces on you that Java hid:
 *   1. OWNERSHIP. Each node is its own malloc. bst_free must walk the WHOLE tree
 *      and free every node — and it must free children BEFORE their parent
 *      (post-order), or it drops the pointers it still needs. See the README.
 *   2. RECURSION = STACK. Insert/search/free/height are naturally recursive, and
 *      the recursion depth equals the tree's HEIGHT. A degenerate (unbalanced)
 *      tree is n deep, so deep recursion can overflow the real call stack. That
 *      failure mode is the entire reason balanced trees (AVL / Red-Black) exist.
 *
 * NOTE: this BST stores DISTINCT keys (a set of ints). Re-inserting an existing
 * key is a no-op (decide this in §3). No duplicate keys.
 */
#ifndef BST_H
#define BST_H

#include <stddef.h>   /* size_t */

/* The key type. One name, change it in one place (C has no generics). Keys must
 * be totally ordered by < and == — that's all the BST needs. */
typedef int bst_key;

/* A node owns its key and two child pointers. `struct BSTNode` names itself so
 * `left`/`right` can point at the same type they live in. A NULL child is an
 * empty subtree — the base case of every recursion in this lesson. */
typedef struct BSTNode {
    bst_key          key;
    struct BSTNode  *left;
    struct BSTNode  *right;
} BSTNode;

/* The tree handle. `root` is the whole tree (NULL when empty); `size` is a cached
 * node count so bst_size is O(1) instead of an O(n) walk — but that cache is a
 * promise you must maintain in EVERY insert and remove. */
typedef struct {
    BSTNode *root;
    size_t   size;
} BST;

/* ---- lifecycle ---------------------------------------------------------- */
BST *bst_new(void);          /* empty tree; NULL on allocation failure          */
void bst_free(BST *t);       /* free EVERY node (post-order), then the handle    */

/* ---- core operations ---------------------------------------------------- */
void bst_insert  (BST *t, bst_key key);       /* insert; existing key = no-op    */
int  bst_contains(const BST *t, bst_key key); /* 1 if present, else 0            */
int  bst_remove  (BST *t, bst_key key);       /* 1 if a key was removed, else 0  */

/* ---- introspection ------------------------------------------------------ */
size_t bst_size  (const BST *t);   /* number of nodes (cached, O(1))            */
size_t bst_height(const BST *t);   /* edges on the longest root→leaf path       */

/* In-order traversal into a CALLER-PROVIDED array. `out` must have room for at
 * least bst_size(t) keys. Returns the number written (== size). Because in-order
 * visits left, then node, then right, the keys land in SORTED order — the proof
 * that a BST is an ordered structure. */
size_t bst_inorder(const BST *t, bst_key *out);

#endif /* BST_H */

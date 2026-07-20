/*
 * bst.c — YOUR binary search tree. SCAFFOLD ONLY (README §4).
 *
 * Implement one milestone at a time, compiling and testing under ASan after each.
 * The theme of this lesson is RECURSION over a linked, heap-allocated structure:
 * almost every operation is "do something at this node, then recurse on the
 * relevant child." The natural shape for the mutating ops is a static helper that
 * takes a subtree root and RETURNS the (possibly new) subtree root:
 *
 *     node = insert_node(node, key);   // rebuilds the spine on the way back up
 *
 * That return-the-root idiom is how you thread a new/rewired child back into its
 * parent without special-casing "the parent is the tree's root."
 *
 * Invariants (must hold after every public call returns):
 *   - BST property: for every node, all left-subtree keys < node->key < all
 *     right-subtree keys.
 *   - Keys are DISTINCT (insert of an existing key is a no-op).
 *   - t->size == the number of nodes reachable from t->root.
 *
 * Ownership: every node is one malloc. bst_free frees them POST-ORDER (both
 * children before the node itself) — free the node first and you've thrown away
 * the pointers to its children and leaked the whole subtree.
 */
#include "bst.h"

#include <assert.h>
#include <stdlib.h>

/* ---- helpers you will write ---------------------------------------------- */

/* Allocate a fresh leaf node: key = key, left = right = NULL. Returns NULL on
 * malloc failure (decide how the callers surface that). A new node is always a
 * leaf — it gets wired into the tree by the caller. */
static BSTNode *node_new(bst_key key) {
    (void)key;
    /* TODO (M2): malloc a BSTNode, set key, NULL both children, return it. */
    assert(0 && "TODO: implement node_new (README M2)");
    return NULL;
}

/* Recursively insert `key` into the subtree rooted at `node`; return the subtree
 * root (unchanged, except the one spot where a new leaf is grafted on). Set
 * *inserted to 1 if a NEW node was created, 0 if the key was already present. */
static BSTNode *insert_node(BSTNode *node, bst_key key, int *inserted) {
    (void)node; (void)key; (void)inserted;
    /* TODO (M2): base case — node == NULL: make a new leaf, *inserted = 1,
     *   return it (this is the graft point the parent adopts).
     * key < node->key: node->left  = insert_node(node->left,  key, inserted);
     * key > node->key: node->right = insert_node(node->right, key, inserted);
     * key == node->key: duplicate — *inserted = 0, change nothing.
     * Always return node so the parent re-adopts this (possibly rewired) child. */
    assert(0 && "TODO: implement insert_node (README M2)");
    return NULL;
}

/* Recursively search the subtree for `key`. Return 1 if found, 0 if not. This is
 * the halving search: at each node go left or right, never both. */
static int contains_node(const BSTNode *node, bst_key key) {
    (void)node; (void)key;
    /* TODO (M3): NULL → 0 (fell off the tree). key == node->key → 1.
     * key < node->key → recurse left, else recurse right. */
    assert(0 && "TODO: implement contains_node (README M3)");
    return 0;
}

/* Free an entire subtree. MUST be POST-ORDER: free left, free right, THEN free
 * node. Freeing `node` first would drop node->left / node->right and leak both
 * subtrees (and reading them after free is undefined behavior). */
static void free_node(BSTNode *node) {
    (void)node;
    /* TODO (M1): if node == NULL return. free_node(node->left);
     * free_node(node->right); free(node);  — in THAT order. */
    assert(0 && "TODO: implement free_node (README M1)");
}

/* Height in EDGES of the subtree rooted at `node`. A single leaf is 0; an empty
 * subtree is the awkward case — see the README design question (returning -1 for
 * "empty" underflows size_t; we treat empty as 0). Height = 1 + max(left, right)
 * for a non-empty node. */
static size_t height_node(const BSTNode *node) {
    (void)node;
    /* TODO (M5): if node == NULL return 0 (our clamp for "empty"); else
     * 1 + the larger of height_node(left) and height_node(right). Watch the
     * base case so a single leaf comes out as 0, not 1 — adjust to your chosen
     * convention and TEST it. */
    assert(0 && "TODO: implement height_node (README M5)");
    return 0;
}

/* In-order fill: write the subtree's keys into out[] starting at index `idx`,
 * return the next free index. In-order = recurse LEFT, write THIS node, recurse
 * RIGHT — which emits keys in ascending (sorted) order. */
static size_t inorder_node(const BSTNode *node, bst_key *out, size_t idx) {
    (void)node; (void)out; (void)idx;
    /* TODO (M4): if node == NULL return idx unchanged.
     * idx = inorder_node(node->left,  out, idx);
     * out[idx++] = node->key;
     * idx = inorder_node(node->right, out, idx);
     * return idx. */
    assert(0 && "TODO: implement inorder_node (README M4)");
    return idx;
}

/* Smallest node in a (non-NULL) subtree = walk left children until there is no
 * left child. Used to find the IN-ORDER SUCCESSOR in the two-children delete. */
static BSTNode *find_min(BSTNode *node) {
    (void)node;
    /* TODO (M6): while node->left != NULL, node = node->left; return node. */
    assert(0 && "TODO: implement find_min (README M6)");
    return NULL;
}

/* Recursively remove `key` from the subtree; return the new subtree root. Set
 * *removed to 1 if a node was deleted, 0 if the key wasn't present. This is the
 * famous THREE-CASE delete — see the README diagrams before you write it. */
static BSTNode *remove_node(BSTNode *node, bst_key key, int *removed) {
    (void)node; (void)key; (void)removed;
    /* TODO (M6):
     *   node == NULL: key not found — *removed stays 0, return NULL.
     *   key < node->key: node->left  = remove_node(node->left,  key, removed);
     *   key > node->key: node->right = remove_node(node->right, key, removed);
     *   key == node->key: this is the victim. *removed = 1. Handle three cases:
     *     (a) LEAF (no children): free(node), return NULL.
     *     (b) ONE child: grab the non-NULL child, free(node), return that child.
     *     (c) TWO children: find the in-order successor = find_min(node->right).
     *         Copy its key into node. Then delete that successor from the right
     *         subtree: node->right = remove_node(node->right, successor_key, &d)
     *         (use a throwaway flag — you already counted this removal). Return
     *         node. WHY the successor? It's the smallest key bigger than node,
     *         so it preserves the BST ordering when it takes node's place.
     *   Return node in the recursive/rewired cases so the parent re-adopts it. */
    assert(0 && "TODO: implement remove_node (README M6)");
    return NULL;
}

/* ------------------------------------------------------------------ M1 ---- */

BST *bst_new(void) {
    /* TODO (M1): malloc a BST, root = NULL, size = 0, return it. Check malloc. */
    assert(0 && "TODO: implement bst_new (README M1)");
    return NULL;
}

void bst_free(BST *t) {
    (void)t;
    /* TODO (M1): if t == NULL return (free(NULL)-style no-op). Else
     * free_node(t->root) to release every node post-order, THEN free(t). */
    assert(0 && "TODO: implement bst_free (README M1)");
}

/* ------------------------------------------------------------------ M2 ---- */

void bst_insert(BST *t, bst_key key) {
    (void)t; (void)key;
    /* TODO (M2): int inserted = 0; t->root = insert_node(t->root, key,
     * &inserted); if (inserted) t->size++;  — keep the cached size honest. */
    assert(0 && "TODO: implement bst_insert (README M2)");
}

/* ------------------------------------------------------------------ M3 ---- */

int bst_contains(const BST *t, bst_key key) {
    (void)t; (void)key;
    /* TODO (M3): return contains_node(t->root, key). */
    assert(0 && "TODO: implement bst_contains (README M3)");
    return 0;
}

/* ------------------------------------------------------------------ M4 ---- */

size_t bst_inorder(const BST *t, bst_key *out) {
    (void)t; (void)out;
    /* TODO (M4): return inorder_node(t->root, out, 0). The return value is the
     * count written (== t->size). Caller guarantees out has room for size keys. */
    assert(0 && "TODO: implement bst_inorder (README M4)");
    return 0;
}

/* ------------------------------------------------------------------ M5 ---- */

size_t bst_size(const BST *t) {
    (void)t;
    /* TODO (M5): return the cached t->size (O(1) — no walk). */
    assert(0 && "TODO: implement bst_size (README M5)");
    return 0;
}

size_t bst_height(const BST *t) {
    (void)t;
    /* TODO (M5): return height_node(t->root). Mind the empty-tree convention. */
    assert(0 && "TODO: implement bst_height (README M5)");
    return 0;
}

/* ------------------------------------------------------------------ M6 ---- */

int bst_remove(BST *t, bst_key key) {
    (void)t; (void)key;
    /* TODO (M6): int removed = 0; t->root = remove_node(t->root, key, &removed);
     * if (removed) t->size--; return removed. */
    assert(0 && "TODO: implement bst_remove (README M6)");
    return 0;
}

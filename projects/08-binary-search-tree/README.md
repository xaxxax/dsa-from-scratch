# Project 8 — Binary Search Tree

> **Status:** lesson outline (full eight-section treatment follows the Project 1
> template).

**Builds on:** recursion fluency and Project 5 (a queue, for level-order
traversal). Sets up Project 9 (heaps) and the bonus self-balancing trees.

## The core question
A sorted array gives O(log n) *search* (binary search) but O(n) *insertion*. A
linked list gives O(1) insertion but O(n) search. Can one structure give us
**O(log n) search, insertion, AND deletion**, while keeping elements ordered?

## The idea
A **binary search tree** keeps the invariant: for every node, all keys in its
left subtree are smaller and all keys in its right subtree are larger. Searching
becomes "go left or right" — halving the problem at each step.

```
            (50)
           /    \
        (30)    (70)
        /  \     /  \
     (20)(40) (60)(80)

search(60): 60>50 → right → 60<70 → left → found.   (3 steps for 7 nodes)
```

## What you'll implement
- A `Node` (key, `left`, `right`); a `BST<E extends Comparable<E>>`.
- `insert`, `contains`, `delete` (the **three cases** — leaf, one child, two
  children — are the famous hard part).
- Traversals: in-order (sorted!), pre-order, post-order (recursive *and*
  iterative), and level-order (using your queue).

## Key questions
- Why does in-order traversal yield sorted output? (Prove it to yourself.)
- The deletion "two children" case: why the in-order successor (or predecessor)?
- Why is the *average* case O(log n) but the *worst* case O(n)? What input causes
  the worst case? (Insert already-sorted data and watch it become a linked list —
  this failure is the entire reason AVL/Red-Black trees exist, in the bonus.)
- Recursion depth = tree height: when can that overflow the call stack?

## LeetCode (in order)
1. [Easy] 700. Search in a Binary Search Tree
2. [Easy] 701. Insert into a Binary Search Tree
3. [Easy] 94. Binary Tree Inorder Traversal (recursive then iterative)
4. [Medium] 98. Validate Binary Search Tree (subtle bounds bug magnet)
5. [Medium] 230. Kth Smallest Element in a BST (in-order + counting)

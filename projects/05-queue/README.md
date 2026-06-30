# Project 5 — Queue (built on your Linked List)

> **Status:** lesson outline (full eight-section treatment follows the Project 1
> template).

**Builds on:** Projects 2/3. You'll use a linked list as the backing store —
and discover *why* it's a better fit here than a dynamic array.

## The core question
Many problems need **first in, first out** (FIFO): print jobs, task schedulers,
BFS traversal (Project 13), request buffers. We need fast insertion at one end
and fast removal at the *other*. Which structure gives us both in O(1)?

## The idea
A queue supports `enqueue` (add at back) and `dequeue` (remove from front). A
linked list with `head` and `tail` pointers does both in O(1). (A naive array
makes `dequeue` O(n) — *unless* you use a clever **circular buffer**, which is a
great stretch goal.)

```
   front                         back
     │                            │
 dequeue ◀── [A]──[B]──[C]──[D] ◀── enqueue
```

## What you'll implement
- `Queue<E>` backed by your linked list: `enqueue`, `dequeue`, `peek`, `isEmpty`,
  `size`.
- **Stretch:** a `Deque<E>` (double-ended) and a fixed-size **circular array**
  queue — then compare with `java.util.ArrayDeque`.

## Key questions
- Why is a singly linked list with a `tail` pointer ideal for a queue?
- Why would an array-backed queue be O(n) per `dequeue` *without* a ring buffer?
- How does a circular buffer reclaim space without shifting?
- Why does BFS (Project 13) specifically need a queue, not a stack?

## LeetCode (in order)
1. [Easy] 232. Implement Queue using Stacks (deepens both structures)
2. [Easy] 933. Number of Recent Calls (sliding window via queue)
3. [Medium] 622. Design Circular Queue (build the ring buffer)
4. [Medium] 239. Sliding Window Maximum (monotonic deque — advanced)

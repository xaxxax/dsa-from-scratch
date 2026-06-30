# Data Structures & Algorithms — Built From Scratch

> A self-taught, university-style course in which **you** implement your own
> miniature version of the Java Collections Framework, one data structure at a
> time, and finish able to reason about how Java, CPython, and the C++ STL work
> under the hood.

---

## 1. Purpose

This repository is **not** a library you install. It is a **course you walk through.**

The goal is not to *have* a `DynamicArray` class. The goal is to be the kind of
engineer who, handed a blank file, could *write* `DynamicArray` from first
principles — and explain every design decision, every Big-O bound, and every
trade-off while doing it.

By the end you will have built, by hand:

- A dynamic array (your own `ArrayList`)
- Singly and doubly linked lists
- A stack and a queue
- A hash map and a hash set (your own `HashMap` / `HashSet`)
- A binary search tree
- A binary heap / priority queue
- A trie
- A graph with traversals (DFS, BFS)
- Union-Find (disjoint set)
- An LRU cache (composing your own structures)
- A segment tree
- ...and a stretch shelf of self-balancing trees and probabilistic structures.

You will understand, for each one: **what problem it solves, why it exists, how
it works internally, what it costs, and where it lives in real software.**

---

## 2. Learning Philosophy

Five principles drive everything here:

1. **You write the code. Always.** The READMEs guide, diagram, and question —
   they do not hand you implementations. Treat any urge to copy a solution as a
   signal that you don't yet understand the structure. Sit with it.
2. **Intuition before memorization.** You should never memorize that "HashMap
   lookup is O(1)." You should be able to *derive* it, and explain when it
   degrades to O(n).
3. **Diagrams before code.** If you can't draw the memory layout on paper, you
   are not ready to type. Every project starts with pictures.
4. **Small milestones.** Each structure is broken into tiny, testable steps:
   constructor → one method → next method → resize → tests. You should be able
   to compile and run after *every* milestone.
5. **Connect to the real world.** Every structure ends with where it actually
   lives — in operating systems, browsers, databases, Git, the JVM, CPython —
   and which LeetCode problems exercise it.

### The mentor contract

When you work through these with your AI mentor (or a human one), the rule is:
**no code is handed to you unless you explicitly ask.** Instead you get guiding
questions, design review, debugging help, and honest critique. If you get stuck,
the right prompt is *"give me a hint,"* not *"give me the answer."*

---

## 3. Roadmap

```
Phase 0  ──  Java Refresher (classes, generics, interfaces, exceptions, arrays)
                      │
   ┌──────────────────┼─────────────────────────────────────────┐
   │   LINEAR          │   ASSOCIATIVE        │   HIERARCHICAL     │
   ▼                   ▼                      ▼                    ▼
 1 Dynamic Array     6 HashMap            8 Binary Search Tree   9 Heap / PQ
 2 Singly Linked     7 HashSet                                  10 Trie
 3 Doubly Linked
 4 Stack  (on #1)
 5 Queue  (on #2/#3)
                      │
   ┌──────────────────┴───────────────────────────┐
   │   GRAPHS                  │   APPLIED          │
   ▼                           ▼                    ▼
11 Graph                     15 LRU Cache (#3 + #6) 16 Segment Tree
12 Depth First Search        14 Union-Find
13 Breadth First Search
                      │
                      ▼
            BONUS: AVL · Red-Black · B-Tree · B+Tree
                   · Skip List · Fenwick · Bloom Filter
```

Each project **builds on the previous ones.** A Stack uses your Dynamic Array. A
Queue uses your Linked List. A HashSet uses your HashMap. An LRU Cache fuses your
HashMap and your Doubly Linked List. This is deliberate: you are assembling a
real, interlocking collections framework, not a pile of disconnected exercises.

---

## 4. Expected Outcomes

When you finish, you should be able to, without notes:

- Implement any of the above structures on a whiteboard.
- State and **derive** the time/space complexity of every operation.
- Explain *why* `ArrayList.add` is "amortized O(1)" — including what "amortized"
  means and the doubling argument behind it.
- Explain how Java's `HashMap` handles collisions, resizing, and (since Java 8)
  treeification of long buckets.
- Pick the right structure for a problem and justify it.
- Read the source of `java.util.ArrayList`, `HashMap`, `ArrayDeque`,
  `PriorityQueue`, `TreeMap` and understand the real implementations.
- Approach a LeetCode medium and recognize which structure it wants.

---

## 5. Suggested Weekly Schedule

This is a ~14–16 week course at a sustainable pace. Adjust freely.

| Week | Focus | Projects |
|------|-------|----------|
| 0 | Java warm-up | Phase 0 |
| 1 | Arrays & growth | 1 Dynamic Array |
| 2 | Pointers & nodes | 2 Singly Linked, 3 Doubly Linked |
| 3 | Restricted-access structures | 4 Stack, 5 Queue |
| 4–5 | Hashing | 6 HashMap |
| 6 | Sets | 7 HashSet |
| 7–8 | Trees | 8 Binary Search Tree |
| 9 | Heaps | 9 Heap / Priority Queue |
| 10 | Prefix structures | 10 Trie |
| 11 | Graph representation | 11 Graph |
| 12 | Graph traversal | 12 DFS, 13 BFS |
| 13 | Disjoint sets | 14 Union-Find |
| 14 | Composition | 15 LRU Cache |
| 15 | Range queries | 16 Segment Tree |
| 16+ | Stretch | Bonus shelf |

**Rule of thumb:** spend at least as long *thinking and drawing* as you spend
typing. If a week feels too fast, slow down — depth beats coverage.

---

## 6. How to Study

For each project, follow the README's eight sections **in order**:

1. **Motivation** — read it and answer aloud: *what problem, why not something
   else, what trade-off?*
2. **Visualization** — copy the ASCII diagrams onto paper. Add your own.
3. **Design** — decide the classes and fields *before* reading further. Compare.
4. **Milestones** — implement one tiny step at a time. Compile after each.
5. **Testing** — write the tests *as you go*, not at the end. Try to break it.
6. **Complexity** — fill in the complexity table yourself, then check it.
7. **Real-World Usage** — read where it lives. Look up one real example.
8. **LeetCode** — attempt the problems in order. No solutions in this repo.

Then do the **Exercises, Reflection Questions, Challenge Problems, and Stretch
Goals**. Finally, request a **code review** before moving on.

> A good loop: *draw it → design it → build one method → test it → break it →
> fix it → explain it out loud.* If you can't explain it, you haven't learned it.

---

## 7. How to Use Git Branches

Treat each project like a feature in a real codebase. See
[`docs/git-workflow.md`](docs/git-workflow.md) for the full guide. The short
version:

```bash
# one branch per project
git checkout -b project/01-dynamic-array

# commit at every milestone — small, frequent, meaningful
git add projects/01-dynamic-array
git commit -m "feat(dynamic-array): implement add() with capacity check"

# when the project passes its tests and review, merge
git checkout main
git merge --no-ff project/01-dynamic-array
git tag p01-complete
```

Commit messages should describe *what milestone* you completed. Your Git history
becomes a record of your learning.

---

## 8. How to Document Your Progress

Keep a running log. Copy [`docs/progress-template.md`](docs/progress-template.md)
into each project folder as `PROGRESS.md` and fill it in. Capture:

- What clicked and what didn't.
- Bugs you hit and how you found them (this is the gold).
- Questions you couldn't answer yet.
- Your own complexity derivations.

Future-you (in an interview, or six months from now) will thank present-you.

---

## 9. How to Build & Test (no heavy tooling required)

Everything here is plain Java — **no Maven, Gradle, or JUnit required** so you
can focus on the data structures, not build files. You need only a JDK (17+
recommended; anything 11+ works).

```bash
# check your toolchain
java  -version
javac -version
```

> **No JDK yet?** If the commands above aren't found, install one first —
> [Eclipse Temurin (Adoptium)](https://adoptium.net/) or
> [Microsoft Build of OpenJDK](https://learn.microsoft.com/java/openjdk/download)
> are good free choices on Windows. After installing, open a new terminal and
> re-run `javac -version` to confirm it's on your `PATH`.

Each project is self-contained in `projects/<nn>-name/` with its own `src/` and
`tests/`. To compile and run a project's test harness:

```bash
cd projects/01-dynamic-array
javac -d out src/*.java tests/*.java
java  -cp out DynamicArrayTest
```

The repo ships a tiny zero-dependency assertion helper,
[`docs/Assert.java`](docs/Assert.java), that you can copy into a project's
`tests/` folder. It gives you `Assert.equals(...)`, `Assert.isTrue(...)`,
`Assert.throwsException(...)`, and prints a pass/fail summary — enough to test
rigorously without pulling in a framework. (When you're comfortable, graduating
to JUnit 5 is a great stretch goal.)

> Prefer an IDE? IntelliJ IDEA Community Edition opens this folder fine. But do
> try the command line at least once — knowing what `javac` and the classpath
> actually do is part of the education.

---

## 10. Repository Layout

```
dsa-from-scratch/
├── README.md                  ← you are here
├── docs/                      ← course-wide guides & helpers
│   ├── how-to-study.md
│   ├── git-workflow.md
│   ├── progress-template.md
│   ├── complexity-cheatsheet.md
│   └── Assert.java            ← tiny test helper, copy into projects
├── assets/                    ← diagrams & images for the docs
├── src/                       ← (reserved) shared code, if any emerges
├── tests/                     ← (reserved) cross-project tests, if any
└── projects/
    ├── 00-java-refresher/
    ├── 01-dynamic-array/      ← README + src/ + tests/
    ├── 02-singly-linked-list/
    ├── ...
    ├── 16-segment-tree/
    └── bonus/
```

Each project folder contains a `README.md` (the lesson), a `src/` folder (where
**you** write the implementation), and a `tests/` folder (where **you** write
the tests).

---

## 11. A Note on Honesty With Yourself

The single biggest failure mode in self-study is **fooling yourself** — reading
a solution, nodding, and believing you understood it. You didn't. Understanding
is *productive*: it's the ability to generate the answer, not recognize it.

So: write the code. Get it wrong. Debug it. Draw the picture. Explain it to an
empty room. That struggle *is* the learning. This repo is built to protect that
struggle, not to shortcut it.

Now go to [`projects/00-java-refresher/`](projects/00-java-refresher/) and begin.

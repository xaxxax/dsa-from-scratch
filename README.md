# Data Structures & Algorithms — Built From Scratch in C

> A self-taught, university-style course in which **you** implement the core data
> structures by hand, in **C**, one at a time — and finish able to reason about
> what the machine is actually doing, and how the JVM, CPython, and the C++ STL
> are built underneath.

---

## 1. Purpose

This repository is **not** a library you install. It is a **course you walk through.**

The goal is not to *have* a `dynamic_array`. The goal is to become the kind of
engineer who, handed a blank file and a C compiler, could *write* one from first
principles — allocate the memory, grow it, free it, and explain every pointer,
every `malloc`, every Big-O bound, and every trade-off while doing it.

**Why C?** Because higher-level languages *hide the machine from you*, and hiding
the machine is exactly what you're trying to see through. In Java or TypeScript,
memory management, pointers, and allocation all happen behind a curtain. In C
there is no curtain: you call `malloc`, you get a raw block of memory, and you are
responsible for every byte until you `free` it. That responsibility *is* the
education. When you finish this course you will understand what garbage collectors,
`ArrayList`, Python's `list`, and `std::vector` are doing for you — because you'll
have done it by hand.

By the end you will have built, from raw memory:

- A dynamic array (your own `vector` / `ArrayList`)
- Singly and doubly linked lists
- A stack and a queue
- A hash map and a hash set
- A binary search tree
- A binary heap / priority queue
- A trie
- A graph with traversals (DFS, BFS)
- Union-Find (disjoint set)
- An LRU cache (composing your own structures)
- A segment tree
- ...and a stretch shelf of self-balancing trees and probabilistic structures.

You will understand, for each one: **what problem it solves, why it exists, how it
works internally, what it costs, who owns its memory, and where it lives in real
software.**

---

## 2. Learning Philosophy

Six principles drive everything here:

1. **You write the code. Always.** The READMEs guide, diagram, and question —
   they do not hand you implementations. Treat any urge to copy a solution as a
   signal that you don't yet understand the structure. Sit with it.
2. **Intuition before memorization.** You should never memorize that "hash lookup
   is O(1)." You should be able to *derive* it, and explain when it degrades to
   O(n).
3. **Diagrams before code.** If you can't draw the memory layout — the boxes and
   the *arrows between them* — on paper, you are not ready to type. In C the arrows
   are literal: they are pointers.
4. **Small milestones.** Each structure is broken into tiny, testable steps:
   allocate → one function → next function → grow → free → tests. You should be
   able to compile and run after *every* milestone.
5. **Own your memory.** Every `malloc` has a matching `free`. Every structure has
   a lifecycle. You will run your tests under **AddressSanitizer** and **valgrind**
   and treat a leak or an invalid read as a failing test, not a warning.
6. **Connect to the real world.** Every structure ends with where it actually
   lives — in operating systems, browsers, databases, Git, the JVM, CPython — and
   which LeetCode problems exercise it.

### The mentor contract

When you work through these with your AI mentor (or a human one), the rule is:
**no code is handed to you unless you explicitly ask.** Instead you get guiding
questions, design review, debugging help, and honest critique. If you get stuck,
the right prompt is *"give me a hint,"* not *"give me the answer."*

---

## 3. What C changes (read this before Project 1)

You may have seen these structures in a higher-level language. C is not a syntax
swap — it changes *what you have to think about*. Four shifts run through the whole
course:

| In Java/TS you had... | In C you have... | Why it matters |
|---|---|---|
| Garbage collection | `malloc` / `free` — **you** own every byte | Forgetting to `free` is a leak; freeing twice is a crash. Memory becomes a first-class design concern. |
| Generics (`List<E>`) | one concrete type, then `void*` + element size | You'll build for `int` first, then learn how real C libraries (`qsort`, growable buffers) fake generics. |
| Exceptions | return codes, `errno`, or `assert()` | There is no `throw`. "How does this signal an error?" becomes a real API design decision you make. |
| References (safe, checked) | raw pointers (fast, unchecked) | `arr[500]` past the end doesn't throw — it's *undefined behavior*. Sanitizers catch it; the language won't. |

None of this is busywork — it is precisely the "how is stuff actually built" layer
you came here for.

---

## 4. Roadmap

```
Phase 0  ──  C Refresher (pointers, malloc/free, structs, the build model)
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
HashMap and your Doubly Linked List. This is deliberate: you are assembling a real,
interlocking toolkit, not a pile of disconnected exercises.

---

## 5. Expected Outcomes

When you finish, you should be able to, without notes:

- Implement any of the above structures on a whiteboard, in C, managing the memory
  correctly.
- State and **derive** the time/space complexity of every operation.
- Explain *why* appending to a dynamic array is "amortized O(1)" — including what
  "amortized" means and the doubling argument behind it.
- Explain what `malloc`/`realloc`/`free` do, what a "dangling pointer," a "leak,"
  a "double free," and a "use-after-free" are, and how to catch each.
- Explain how Java's `HashMap`, CPython's `dict`, and C++'s `unordered_map` handle
  collisions and resizing — because you built the same machinery.
- Read AddressSanitizer / valgrind output and fix the bug it points at.
- Pick the right structure for a problem and justify it.

---

## 6. Suggested Weekly Schedule

A ~14–16 week course at a sustainable pace. Adjust freely; C will make the early
weeks a little slower than a managed language would — that friction is the point.

| Week | Focus | Projects |
|------|-------|----------|
| 0 | Pointers, heap, the build model | Phase 0 |
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

## 7. How to Study

For each project, follow the README's eight sections **in order**:

1. **Motivation** — *what problem, why not something else, what trade-off?*
2. **Visualization** — copy the diagrams onto paper. In C, draw the pointers.
3. **Design** — decide the `struct` and function signatures *before* reading on.
4. **Milestones** — implement one tiny step at a time. Compile after each.
5. **Testing** — write the tests *as you go*, and run them under the sanitizers.
6. **Complexity** — fill in the table yourself, then check it.
7. **Real-World Usage** — read where it lives. Look up one real example.
8. **LeetCode** — attempt the problems in order. No solutions in this repo.

See [`docs/how-to-study.md`](docs/how-to-study.md) for the full method and
[`docs/how-to-build.md`](docs/how-to-build.md) for the toolchain.

---

## 8. Git Workflow

Treat each project like a feature in a real codebase. See
[`docs/git-workflow.md`](docs/git-workflow.md). Short version: one branch per
project (`project/01-dynamic-array`), commit at every milestone with
[Conventional Commits](https://www.conventionalcommits.org/), merge with
`--no-ff` and tag `p01-complete` when done.

---

## 9. How to Build & Test

Everything here is plain C11 with a tiny per-project `Makefile` — no CMake, no
frameworks — so you can focus on the data structures, not the build system. You
need a C compiler (`gcc` or `clang`) and `make`.

```bash
# check your toolchain
cc --version
make --version
```

Each project is self-contained in `projects/<nn>-name/`. To build and run its
tests (compiled with warnings-as-guidance and AddressSanitizer on):

```bash
cd projects/01-dynamic-array
make test          # compiles src/ + tests/ and runs the test binary
make clean         # remove build artifacts
```

> **No compiler yet?** On Linux/WSL: `sudo apt install build-essential valgrind`.
> On macOS: `xcode-select --install`. On Windows, use **WSL** (which you already
> do) — native MSVC works too but the sanitizer flags differ. See
> [`docs/how-to-build.md`](docs/how-to-build.md).

The shared test helper is [`docs/test_utils.h`](docs/test_utils.h) — a tiny
zero-dependency header you `#include` from each project's test file. It gives you
`CHECK`, `CHECK_INT`, `CHECK_STR`, and a pass/fail summary.

---

## 10. Repository Layout

```
dsa-from-scratch/
├── README.md                  ← you are here
├── docs/                      ← course-wide guides & helpers
│   ├── how-to-study.md
│   ├── how-to-build.md        ← C toolchain, sanitizers, valgrind
│   ├── git-workflow.md
│   ├── progress-template.md
│   ├── complexity-cheatsheet.md
│   └── test_utils.h           ← tiny test helper, #include into tests
└── projects/
    ├── 00-c-refresher/
    ├── 01-dynamic-array/      ← README + src/ + tests/ + Makefile
    ├── ...
    ├── 16-segment-tree/
    └── bonus/
```

Each project folder contains a `README.md` (the lesson), a `src/` folder (where
**you** write the implementation), a `tests/` folder (where **you** write the
tests), and a `Makefile`.

---

## 11. A Note on Honesty With Yourself

The single biggest failure mode in self-study is **fooling yourself** — reading a
solution, nodding, and believing you understood it. You didn't. Understanding is
*productive*: it's the ability to generate the answer, not recognize it. In C
there's a second, crueler check: **the machine does not forgive.** A leak, a
dangling pointer, or an off-by-one won't be politely caught for you — it'll corrupt
memory silently until it doesn't. That's not a reason to fear C. It's the reason C
teaches you more.

So: write the code. Get it wrong. Read the sanitizer output. Draw the pointers.
Explain it to an empty room. That struggle *is* the learning.

Now go to [`projects/00-c-refresher/`](projects/00-c-refresher/) and begin.

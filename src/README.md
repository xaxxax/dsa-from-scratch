# `src/` — reserved for shared code

The real implementation work happens **inside each project**, in
`projects/<nn>-name/src/`. That keeps every structure self-contained and
compilable on its own.

This top-level `src/` is reserved for the day you want to share code *across*
projects — for example, once your `DynamicArray`, `HashMap`, and
`DoublyLinkedList` are battle-tested, you might lift the polished versions here as
a single `mini-collections` package and have later projects depend on them rather
than re-copying. That's an excellent late-course refactor and a great Git
exercise, but don't rush it: building each structure fresh is where the learning
is. Leave this empty until you have a deliberate reason to use it.

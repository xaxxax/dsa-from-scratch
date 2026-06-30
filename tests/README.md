# `tests/` — reserved for cross-project tests

Each project ships its own test harness in `projects/<nn>-name/tests/`, using the
zero-dependency [`../docs/Assert.java`](../docs/Assert.java) helper. That's where
day-to-day testing lives.

This top-level `tests/` is reserved for **integration tests that span multiple
projects** — for instance, a test that builds an LRU Cache (Project 15) from your
shared `HashMap` and `DoublyLinkedList` and verifies they cooperate, or a
benchmark suite comparing your structures against `java.util` equivalents. Add
those here when you reach that point. Until then, it intentionally stays empty.

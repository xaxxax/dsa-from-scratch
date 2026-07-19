# How to Study This Course

A short manual for getting the most out of the repository. Read it once now,
then revisit it whenever a project feels overwhelming.

---

## The core loop

For every data structure, run this loop:

```
   ┌─────────┐
   │  DRAW   │  Sketch the memory layout on paper. No code yet.
   └────┬────┘
        ▼
   ┌─────────┐
   │ DESIGN  │  Decide structs, fields, function signatures. Still no bodies.
   └────┬────┘
        ▼
   ┌─────────┐
   │  BUILD  │  Implement ONE milestone (e.g. just the constructor).
   └────┬────┘
        ▼
   ┌─────────┐
   │  TEST   │  Write a test for that one method. Run it. Watch it pass.
   └────┬────┘
        ▼
   ┌─────────┐
   │  BREAK  │  Feed it edge cases until it fails. Empty? Full? Huge? Then run
   └────┬────┘  it under AddressSanitizer + valgrind — a leak IS a failure.
        ▼
   ┌─────────┐
   │ EXPLAIN │  Say out loud why it works and what it costs. If you can't,
   └────┬────┘  you're not done — go back to DRAW.
        │
        └──────► next milestone
```

The two steps people skip are **DRAW** and **EXPLAIN**. They are the two that
matter most. Skipping them is how you end up "understanding" a structure you
can't actually reproduce.

### The memory dimension (new in C)

This course is in **C**, so every structure has a fifth question the garbage
collector used to answer for you: **who frees this, and when?** Fold it into the
loop — during DESIGN decide the lifecycle (`xxx_new` / `xxx_free`, who owns what);
during BUILD write the `free` alongside the `new`; during BREAK prove it with
`make test` (AddressSanitizer) and `valgrind --leak-check=full`. A structure that
works but leaks is not done. Owning memory *is* half of what this course teaches.

---

## How to use your AI mentor well

The mentor's job is to *not* give you code. Help it help you:

**Good prompts**
- "I drew this layout for a dynamic array — is my mental model right?"
- "Give me a hint about why my `remove()` leaves a gap."
- "Review my `add()` method for correctness and style."
- "Ask me a question that will tell me whether I really understand resizing."
- "What edge case am I probably forgetting?"

**Prompts that rob you of the learning**
- "Just write the HashMap for me." (Now you've learned nothing.)
- "Paste the solution." (Recognition ≠ understanding.)

If you're truly stuck after a real attempt, escalate gradually:
*concept hint → pseudocode → one method → full method.* Never start at the end.

---

## Reading the eight-section template

Every project README has the same spine. Here's what each section is *for*:

| Section | What it's really teaching you |
|---------|------------------------------|
| 1. Motivation | *Why this exists.* The problem that made someone invent it. |
| 2. Visualization | *The mental model.* Pictures you must be able to redraw. |
| 3. Design | *Decomposition.* Turning an idea into structs, fields, and a memory lifecycle. |
| 4. Milestones | *Incremental construction.* How pros avoid big-bang failure. |
| 5. Testing | *Verification & edge-case thinking.* |
| 6. Complexity | *Cost reasoning.* The heart of the CS, not the coding. |
| 7. Real-World Usage | *Relevance.* Where this lives in software you use daily. |
| 8. LeetCode | *Pattern recognition.* Connecting structure to problems. |

---

## When you're stuck (a debugging ladder)

1. **Re-read your own diagram.** Does the code match the picture?
2. **Add print statements** at the top and bottom of the method. What's the
   actual state vs. what you expected?
3. **Shrink the input.** Reproduce the bug with the smallest possible case.
4. **Check the boundaries.** Off-by-one (`<` vs `<=`), empty, single-element,
   full-capacity, first index, last index, and the `size_t` unsigned trap
   (a "negative" index arrives as a huge positive number).
5. **Run the sanitizer.** `make test` builds with AddressSanitizer + UBSan; its
   report names the exact line of a bad read/write/free. For leaks,
   `valgrind --leak-check=full ./out_test` names the `malloc` you forgot to free.
   These tools are not noise — they're the answer.
6. **Rubber-duck it.** Explain the function line by line to an object on your
   desk. The bug usually surfaces mid-sentence.
7. *Then* ask for a hint.

---

## Signs you've actually learned it

- You can implement it from a blank file without looking.
- You can derive the Big-O, not recite it.
- You can name a real system that uses it and say why.
- You can explain its weakness — every structure has one.
- You can spot it inside a LeetCode problem description.

If all five are true, tag the project complete and move on. If not, that's not
failure — that's information about what to revisit.

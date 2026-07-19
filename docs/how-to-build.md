# How to Build & Test (the C toolchain)

A short manual for the tools this course uses. You are on WSL/Linux, which is the
happy path — everything below "just works" there.

---

## The toolchain

- **A C compiler** — `gcc` or `clang`. Either is fine; `cc` on your system points
  at one of them. This course targets **C11** (`-std=c11`).
- **`make`** — runs the tiny per-project `Makefile` so you don't retype compiler
  flags.
- **`valgrind`** — a memory-error and leak detector you run your test binary
  under. Slower but thorough.

Install on Debian/Ubuntu/WSL:

```bash
sudo apt update
sudo apt install build-essential valgrind
```

Verify:

```bash
cc --version
make --version
valgrind --version
```

---

## The build model (this is new if you come from Java)

C has no `import`. Instead:

- A **header** (`.h`) declares *what exists* — struct definitions and function
  prototypes. Other files `#include` it to learn the shapes.
- A **source** (`.c`) file defines *how it works* — the actual function bodies.
- **Compiling** turns each `.c` into machine code (an object file).
- **Linking** stitches the object files together into one runnable binary.

The `Makefile` in each project does compile + link + run in one `make test`. Read
it once — knowing what the compiler and linker actually do is part of the point.

---

## The flags we use, and why

The per-project `Makefile` compiles with:

```
-std=c11 -Wall -Wextra -Wpedantic -g -fsanitize=address,undefined
```

- `-Wall -Wextra -Wpedantic` — turn on the compiler's warnings. **Treat warnings
  as bugs.** In C, a warning is very often the compiler telling you about a real
  defect it can't refuse to compile.
- `-g` — include debug info (line numbers in sanitizer/valgrind output, usable in
  `gdb`).
- `-fsanitize=address,undefined` — compile in **AddressSanitizer (ASan)** and
  **UndefinedBehaviorSanitizer (UBSan)**. These instrument your program so that a
  buffer overrun, use-after-free, leak, or signed-overflow *crashes loudly with a
  stack trace* instead of silently corrupting memory. This is the safety net that
  makes learning C sane. **A sanitizer report is a failing test.**

---

## Running under valgrind (the belt-and-suspenders check)

ASan catches most things, but `valgrind` is the classic tool and catches leaks
precisely:

```bash
make            # build without running (or build the binary once)
valgrind --leak-check=full --error-exitcode=1 ./out_test
```

Goal: **"0 errors, 0 bytes definitely lost."** If valgrind reports leaked bytes,
you `malloc`'d something you never `free`'d — find the missing `free`.

> Tip: don't run valgrind *and* ASan at the same time (they conflict). Use ASan
> during normal `make test`; run valgrind separately when you want the leak audit.

---

## The mental checklist for every structure

1. Every `malloc`/`calloc`/`realloc` has exactly one matching `free`.
2. After `free(p)`, treat `p` as radioactive — don't read or free it again. (Set
   it to `NULL` if it might be touched later.)
3. Never read past `size`; never index past `capacity`.
4. `make test` passes **and** `valgrind` reports no leaks. Only then is it done.

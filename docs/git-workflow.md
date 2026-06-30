# Git Workflow for This Course

You'll treat each project like a feature shipped to a real codebase. This builds
two habits at once: clean version control, *and* a permanent record of your
learning that you can show in interviews.

---

## One branch per project

```bash
git checkout main
git pull                                   # if you have a remote
git checkout -b project/01-dynamic-array
```

Branch names: `project/<nn>-<name>`. The number keeps them sorted; the name
keeps them readable.

---

## Commit at every milestone

The project READMEs break each structure into small milestones. **Commit after
each one.** Small commits are easier to review, easier to revert, and they make
your history tell the story of how you built the thing.

Use [Conventional Commits](https://www.conventionalcommits.org/) — it's what
most professional teams use:

```bash
git commit -m "feat(dynamic-array): add constructor with initial capacity"
git commit -m "feat(dynamic-array): implement add() with bounds check"
git commit -m "feat(dynamic-array): grow backing array on overflow"
git commit -m "test(dynamic-array): cover empty, full, and resize cases"
git commit -m "fix(dynamic-array): off-by-one in remove() shift loop"
git commit -m "docs(dynamic-array): record resize derivation in PROGRESS.md"
```

Prefixes you'll use most: `feat` (new behavior), `fix` (bug), `test` (tests),
`docs` (notes/READMEs), `refactor` (cleanup, no behavior change).

---

## Merge when the project is done

A project is "done" when it passes its tests *and* you've done a code review.

```bash
git checkout main
git merge --no-ff project/01-dynamic-array
git tag p01-complete                       # bookmark the milestone
```

`--no-ff` forces a merge commit so the history visibly groups each project's
work together. The tag lets you jump back to "the day I finished project N."

---

## A typical day

```bash
git status                  # what have I changed?
git diff                    # review my own changes before committing
git add projects/01-dynamic-array/src/DynamicArray.java
git commit -m "feat(dynamic-array): implement remove(index)"
```

**Review your own diff before every commit.** It's the cheapest code review you
will ever get, and it catches stray debug prints and half-finished thoughts.

---

## Optional: push to GitHub

If you want a backup and a portfolio:

```bash
# create an empty repo on GitHub first, then:
git remote add origin git@github.com:<you>/dsa-from-scratch.git
git push -u origin main
git push origin project/01-dynamic-array
```

A green commit graph that shows steady, small commits over 16 weeks tells a
better story than any résumé bullet.

---

## A .gitignore to start with

Create `.gitignore` at the repo root:

```
# compiled output
out/
bin/
*.class

# IDE
.idea/
*.iml
.vscode/

# OS
.DS_Store
Thumbs.db
```

You don't want compiled `.class` files in version control — only source.

# Phase 0 — Java Refresher

> **Goal:** make sure the *language* never gets in the way of the *ideas*. The
> rest of this course assumes you can confidently write classes, generics,
> interfaces, and exception handling. This phase is a checklist, not a tutorial —
> for each topic, you should be able to write a tiny example from scratch. If one
> feels shaky, that's exactly what to drill before Project 1.

You will write a small amount of code here, but the point is *fluency*, not a
finished artifact. Treat each section as: "can I produce this without looking?"

---

## Why these specific topics?

Every data structure you build will be:

- **a class** (`DynamicArray`) — so you need classes, fields, constructors.
- **generic** (`DynamicArray<E>` holds any type) — so you need generics.
- **an implementation of an interface** (your own `List`, `Queue`, etc.) — so you
  need interfaces.
- **backed by an array** (`E[] elements`) — so you need arrays, including the
  awkward bits of generic arrays.
- **defensive** (throwing on bad input) — so you need exceptions.

Master these five and the language disappears, leaving only the CS.

---

## 1. Classes, Objects, Constructors

A class is a blueprint; an object is an instance built from it; a constructor is
the code that runs when you build one.

```java
public class Point {
    private final int x;   // field (instance state)
    private final int y;

    public Point(int x, int y) {   // constructor
        this.x = x;                // `this` disambiguates field from parameter
        this.y = y;
    }

    public int x() { return x; }   // accessor
    public int y() { return y; }

    @Override
    public String toString() {     // overriding Object's default
        return "(" + x + ", " + y + ")";
    }
}
```

```java
Point p = new Point(3, 4);   // `new` calls the constructor
System.out.println(p);       // (3, 4)  — toString() is called implicitly
```

**Check yourself:**
- What's the difference between a field and a local variable?
- What does `final` on a field guarantee? On a local variable?
- What happens if you write *no* constructor? (Java gives you a default no-arg
  one — but only if you write none yourself.)
- Why is `private` the default you should reach for? (Encapsulation: hide state,
  expose behavior.)

---

## 2. Interfaces

An interface is a **contract**: a set of method signatures with no bodies (mostly).
A class that `implements` an interface promises to provide those methods. This is
how you'll define what a "List" or "Queue" *is*, separately from *how* it works.

```java
public interface Container<E> {
    void add(E item);
    E get(int index);
    int size();
    boolean isEmpty();   // we'll give this a default below
}
```

Interfaces can provide **default methods** — shared behavior derived from the
others:

```java
public interface Container<E> {
    void add(E item);
    int size();

    default boolean isEmpty() {   // implemented in terms of size()
        return size() == 0;
    }
}
```

**Check yourself:**
- How is an interface different from an abstract class?
- Why does the JDK have a `List` *interface* and several *implementations*
  (`ArrayList`, `LinkedList`)? (Spoiler: this is the whole point of your course —
  same contract, different trade-offs.)
- Can a class implement more than one interface? (Yes. More than one superclass?
  No.)

---

## 3. Generics

Generics let one class work with *any* type while keeping **compile-time type
safety**. Instead of a container of `Object` (which forces casts and allows
mistakes), you write a container of `E` and the compiler enforces it.

```java
public class Box<E> {            // E is a type parameter
    private E value;
    public void set(E value) { this.value = value; }
    public E get() { return value; }
}
```

```java
Box<String> b = new Box<>();
b.set("hello");
String s = b.get();   // no cast needed; compiler knows it's a String
// b.set(42);         // compile error — type safety in action
```

**Conventional type-parameter names:** `E` (element), `K`/`V` (key/value),
`T` (type), `N` (number). Use them; they signal intent to other readers.

**Bounded types** (you'll need this for trees and heaps that compare elements):

```java
// E must implement Comparable, so we can call e1.compareTo(e2)
public class SortedBox<E extends Comparable<E>> { ... }
```

**Check yourself:**
- Why is `Box<String>` safer than a `Box` holding `Object`?
- What does `<E extends Comparable<E>>` buy you?
- What is **type erasure**, and why does it make `new E[10]` illegal? (This is a
  real wrinkle you'll hit in Project 1 — see the next section.)

---

## 4. Arrays (including the generic-array wrinkle)

An array is a fixed-size, contiguous block of elements with O(1) indexed access.
It is the lowest-level building block — your Dynamic Array, HashMap, and Heap are
all *backed by* arrays.

```java
int[] nums = new int[5];     // length 5, all zeros
nums[0] = 10;
nums[4] = 50;
// nums[5] = 1;              // ArrayIndexOutOfBoundsException at runtime
System.out.println(nums.length);   // 5  (a field, not a method!)
```

Arrays of objects hold **references**, default-initialized to `null`:

```java
String[] words = new String[3];   // [null, null, null]
```

### The generic-array wrinkle (important — you'll meet it in Project 1)

Because of **type erasure**, this does *not* compile:

```java
E[] data = new E[10];   // ERROR: generic array creation
```

The standard workaround the JDK itself uses is to create an `Object[]` and cast,
accepting one "unchecked" warning:

```java
@SuppressWarnings("unchecked")
E[] data = (E[]) new Object[10];   // the idiom you'll use
```

You don't need to *deeply* understand erasure yet — just recognize the idiom and
why it's there. We'll revisit it with full context in Project 1.

**Check yourself:**
- What's the time complexity of reading `arr[i]`? Why is it O(1)? (Hint: address
  arithmetic — `base + i * elementSize`.)
- Can you change an array's length after creation? (No. That limitation is the
  entire reason Project 1 exists.)
- Difference between `arr.length` (array) and `str.length()` (String)?

---

## 5. Exceptions

Exceptions signal that something went wrong, separating error handling from the
happy path. Your structures will **throw** them to reject bad input.

```java
public E get(int index) {
    if (index < 0 || index >= size) {
        throw new IndexOutOfBoundsException("index " + index + ", size " + size);
    }
    return data[index];
}
```

```java
try {
    list.get(99);
} catch (IndexOutOfBoundsException e) {
    System.out.println("caught: " + e.getMessage());
} finally {
    // runs whether or not an exception was thrown
}
```

**Checked vs. unchecked** (a Java-specific distinction):
- **Unchecked** (`RuntimeException` and subclasses, e.g. `IllegalArgumentException`,
  `IndexOutOfBoundsException`, `NullPointerException`) — programming errors; you
  are *not* forced to declare or catch them. You'll throw these constantly.
- **Checked** (`Exception` but not `RuntimeException`, e.g. `IOException`) — must
  be declared with `throws` or caught. You'll rarely need these in this course.

**Check yourself:**
- Which exception type fits "caller asked for an index that doesn't exist"?
  (`IndexOutOfBoundsException`.) "Caller passed a negative capacity"?
  (`IllegalArgumentException`.)
- Why throw early with a clear message instead of letting a `null` blow up three
  methods later? (Fail fast — the message points straight at the caller's bug.)

---

## Warm-up exercise (write this yourself)

Build a **fixed-capacity** `IntBox` to exercise all five topics at once. Do *not*
use any `java.util` collections.

**Spec:**
- `IntBox(int capacity)` — throws `IllegalArgumentException` if capacity < 1.
- `void push(int value)` — appends; throws `IllegalStateException` if full.
- `int get(int index)` — throws `IndexOutOfBoundsException` if out of range.
- `int size()` — number of items currently stored.
- `boolean isFull()`.

Then make it generic: `Box<E>` using the `(E[]) new Object[capacity]` idiom.
Write a tiny `main` that exercises every method, including the error cases.

> This is intentionally a miniature of Project 1. If you can write `Box<E>`
> comfortably, you're ready. If the generic array or the exceptions tripped you
> up, that's the signal to re-read those sections before moving on.

---

## Reflection questions

1. Why does Java separate *interface* (contract) from *class* (implementation)?
   How will that separation help you compare `ArrayList` vs `LinkedList` later?
2. Generics give compile-time safety, but type erasure removes the type at
   runtime. What's the trade-off Java made, and what does it cost you?
3. Why are arrays fixed-size at the language level? What would it take to make
   them grow — and isn't that exactly Project 1?

---

## When you're ready

You're ready for **Project 1 — Dynamic Array** when you can, from a blank file:

- [ ] Write a generic class with a constructor and private fields.
- [ ] Define and implement an interface with a default method.
- [ ] Create a backing array using the `(E[]) new Object[n]` idiom and explain
      why the cast is needed.
- [ ] Throw the right exception with a helpful message for bad input.
- [ ] Compile and run a `main` from the command line with `javac` + `java`.

→ Continue to [`../01-dynamic-array/`](../01-dynamic-array/).

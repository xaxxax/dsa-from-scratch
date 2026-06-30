/**
 * DynamicArray<E> — YOUR growable, array-backed implementation of List<E>.
 *
 * This is a SCAFFOLD. The signatures, fields, and milestone markers are here to
 * keep you oriented. Every method body is yours to write. Work milestone by
 * milestone (see README §4), compiling and testing after each one.
 *
 * The whole structure rests on ONE invariant you must never break:
 *
 *      0 <= size <= data.length          (size = items in use; data.length = capacity)
 *
 * If a test fails, the first question is almost always: "did I violate that?"
 */
public class DynamicArray<E> implements List<E> {

    // ---- Fields -------------------------------------------------------------
    // TODO (M1): the backing array. Remember the generic-array idiom from Phase 0:
    //            you cannot write `new E[n]`; use (E[]) new Object[n] with a
    //            contained @SuppressWarnings("unchecked").
    // private E[] data;

    // TODO (M1): the number of elements currently in use. This is DISTINCT from
    //            data.length. Conflating them is the #1 bug in this project.
    // private int size;

    // TODO (M1): pick a default initial capacity and justify it (README §3 Q1).
    // private static final int DEFAULT_CAPACITY = ?;


    // ---- Constructors (M1) --------------------------------------------------

    /** Create an empty list with the default initial capacity. */
    public DynamicArray() {
        // TODO (M1): delegate to the capacity constructor with DEFAULT_CAPACITY,
        //            or allocate directly. Set size = 0.
        throw new UnsupportedOperationException("TODO M1: constructor");
    }

    /**
     * Create an empty list with the given initial capacity.
     * @throws IllegalArgumentException if initialCapacity < 1 (decide your policy)
     */
    public DynamicArray(int initialCapacity) {
        // TODO (M1): validate input, allocate the backing array, set size = 0.
        throw new UnsupportedOperationException("TODO M1: constructor(capacity)");
    }


    // ---- Core operations ----------------------------------------------------

    @Override
    public void add(E item) {
        // TODO (M3): easy case — assume room exists: write at index `size`, size++.
        // TODO (M5): before writing, if size == data.length, call resize().
        throw new UnsupportedOperationException("TODO M3/M5: add");
    }

    @Override
    public E get(int index) {
        // TODO (M4): bounds-check (valid range is 0..size-1), then return data[index].
        throw new UnsupportedOperationException("TODO M4: get");
    }

    @Override
    public E set(int index, E item) {
        // TODO (M6): bounds-check, save old value, overwrite, return old value.
        throw new UnsupportedOperationException("TODO M6: set");
    }

    @Override
    public E remove(int index) {
        // TODO (M7): bounds-check; save the value to return; shift the tail left;
        //            decrement size; null the now-vacated last slot (README FAQ).
        throw new UnsupportedOperationException("TODO M7: remove");
    }

    @Override
    public boolean contains(E item) {
        // TODO (M8): linear scan over 0..size-1. Be null-safe in the comparison.
        throw new UnsupportedOperationException("TODO M8: contains");
    }

    @Override
    public int size() {
        // TODO (M2): return the count of elements in use.
        throw new UnsupportedOperationException("TODO M2: size");
    }


    // ---- Growth (M5) --------------------------------------------------------

    /**
     * Grow the backing array so there is room for more elements.
     * Decide your growth factor (README §3 Q2) — and be ready to defend why
     * doubling beats adding a constant (README §6).
     */
    private void resize() {
        // TODO (M5): allocate a larger array, copy 0..size-1 across, reassign data.
        throw new UnsupportedOperationException("TODO M5: resize");
    }


    // ---- Stretch ------------------------------------------------------------

    @Override
    public void add(int index, E item) {
        // TODO (M10, optional): validate 0..size; resize if full; shift right; insert.
        throw new UnsupportedOperationException("TODO M10: add(index, item)");
    }

    @Override
    public String toString() {
        // TODO (M9): produce "[a, b, c]". Implement this early — it makes every
        //            other bug visible.
        return super.toString();
    }
}

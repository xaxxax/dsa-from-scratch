/**
 * List<E> — the CONTRACT for an ordered, index-addressable collection.
 *
 * This interface intentionally mirrors a slice of java.util.List. You will
 * implement it twice in this course: once with a dynamic array (Project 1) and
 * once with a linked list (Project 2). Same contract, opposite trade-offs —
 * that contrast is a core lesson, so keep this interface stable.
 *
 * You generally do NOT need to change this file. Implement DynamicArray<E>.
 */
public interface List<E> {

    /** Append item to the end of the list. */
    void add(E item);

    /**
     * Insert item at the given index, shifting later elements right.
     * Valid index range: 0..size (inclusive of size == append).
     * (Stretch milestone M10 — you may leave this unimplemented at first.)
     */
    void insert(int index, E item);

    /** Return the element at index. Throws IndexOutOfBoundsException if invalid. */
    E get(int index);

    /** Replace the element at index, returning the previous value. */
    E set(int index, E item);

    /**
     * Remove and return the element at index, shifting later elements left.
     * Throws IndexOutOfBoundsException if invalid.
     */
    E remove(int index);

    /** Number of elements currently stored (NOT the backing capacity). */
    int size();

    /** Linear search; true if an equal element is present. */
    boolean contains(E item);

    /** Returns the current capacity */
    int capacity();

    /**
     * True when there are no elements.
     *
     * This is a DEFAULT method: it is defined in terms of size(), so every
     * implementer gets it for free. (See Phase 0 §2 — interface default methods.)
     */
    default boolean isEmpty() {
        return size() == 0;
    }
}

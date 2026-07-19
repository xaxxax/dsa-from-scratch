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

    private E[] data;
    private int length;
    private static final int DEFAULT_CAPACITY = 15;
    private static final int GROWTH_FACTOR = 2;

    // Default capacity of 15 can potentially lead to unneeded memory allocation, but with also exponentially lead to
    // less unneeded copying, which is a bigger bottleneck to the memory than unneeded allocation.
    // We also grow on a geometric sequence, with a growth factor of 2. So next allocation will be
    // 30, 60, 120, 240, ... 

    @SuppressWarnings("unchecked")
    public DynamicArray() {
        this.data = (E[]) new Object[DEFAULT_CAPACITY];
        this.length = 0;
    }

    /**
     * Create an empty list with the given initial capacity.
     * @throws IllegalArgumentException if initialCapacity < 1 (decide your policy)
     */
    @SuppressWarnings("unchecked")
    public DynamicArray(int initialCapacity) {
        if (initialCapacity < 1) {
            throw new IllegalArgumentException("Capacity cannot be less than 1.");
        }

        this.data = (E[]) new Object[initialCapacity];
        this.length = 0;
    }

    @Override
    public void add(E item) {

        if(size() == capacity()) {
            resize();
        }

        this.data[size()] = item;
        length++;
    }

    @Override
    public E get(int index) {
        if (index < 0 || index >= size()) {
            throw new IndexOutOfBoundsException("Index " + index + " is out of bounds for " + size());
        }

        return data[index];
    }

    @Override
    public E set(int index, E item) {
        if (index < 0 || index >= size()) {
            throw new IndexOutOfBoundsException("Index " + index + " is out of bounds for " + size());
        }

        E valueToOverwrite = data[index];

        data[index] = item;

        return valueToOverwrite;
    }

    @Override
    public E remove(int index) {
        if (index < 0 || index >= size()) {
            throw new IndexOutOfBoundsException("Index " + index + " is out of bounds for " + size());
        }

        E valueToDelete = data[index];

        for (int i = index; i < size() - 1; i++){
            data[i] = data[i + 1];
        }

        data[size() - 1] = null;
        length--;

        return valueToDelete;
    }

    @Override
    public boolean contains(E item) {
        for(int i = 0; i < size(); i++){
            if (item == null){
                if (data[i] == null) return true;
            }

            else if(item.equals(data[i])) {
                return true;
            }
        }
        return false;
    }

    /* Size returns the number of elements in the array, with "length" being the variable
       This is not the same as data.length, as data.length will return the capacity of the given array.
       See below method for difference. */
    @Override
    public int size() {
        return length;
    }

    @Override
    public int capacity() {
        return data.length;
    }

    @SuppressWarnings("unchecked")
    private void resize() {
        int currentCapacity = this.capacity();

        E[] newArray = (E[]) new Object[currentCapacity * GROWTH_FACTOR];

        for (int i = 0; i < size(); i++){
            newArray[i] = data[i];
        }

        this.data = newArray;
    }

    @Override
    public void insert(int index, E item) {
        if (index < 0 || index > size()) {
            throw new IndexOutOfBoundsException("Index " + index + " is out of bounds for " + size());
        }

        if(size() == capacity()) {
            resize();
        }

        for(int i = size() - 1; i >= index; i--) {
           data[i + 1] = data[i];
        }

        data[index] = item;
        length++;
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder("[");

        for (int i = 0; i < size(); i++) {
            if (i > 0) sb.append(", ");
            sb.append(data[i]);
        }
        sb.append("]");
        return sb.toString();
    }
}

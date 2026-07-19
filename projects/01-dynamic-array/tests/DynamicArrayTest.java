/**
 * DynamicArrayTest — YOUR test harness for Project 1.
 *
 * SETUP: copy ../../docs/Assert.java into THIS tests/ folder so it compiles
 * alongside this file. Then:
 *
 *     cd projects/01-dynamic-array
 *     javac -d out src/*.java tests/*.java
 *     java  -cp out DynamicArrayTest
 *
 * Write tests AS YOU GO, one milestone at a time — not all at the end. A few
 * starters are filled in to show the rhythm; the rest are TODOs mapped to the
 * milestones and edge cases in README §5. Make them all pass.
 */
public class DynamicArrayTest {

    public static void main(String[] args) {

        // --- M1/M2: construction, size, isEmpty -----------------------------
        DynamicArray<Integer> a = new DynamicArray<>();
        Assert.equals(0, a.size(), "new list has size 0");
        Assert.isTrue(a.isEmpty(), "new list isEmpty");

        // --- M3/M4: add + get ----------------------------------------------
        a.add(10);
        a.add(20);
        a.add(30);
        Assert.equals(3, a.size(), "size is 3 after three adds");
        Assert.equals(10, a.get(0), "get(0) == 10");
        Assert.equals(30, a.get(2), "get(2) == 30");
        Assert.isFalse(a.isEmpty(), "non-empty after adds");

        // --- M4: bounds checking -------------------------------------------
        Assert.throwsException(() -> a.get(3), IndexOutOfBoundsException.class,
                "get(size) throws (off-by-one guard)");
        Assert.throwsException(() -> a.get(-1), IndexOutOfBoundsException.class,
                "get(-1) throws");

        // --- M6: set --------------------------------------------------------
        Assert.equals(20, a.set(1, 99), "set method returns old value of 20");
        Assert.equals(99, a.get(1), "the newly insert value 99 at index 1 should be returned");

        // --- M7: remove -----------------------------------------------------
        Assert.equals(10, a.remove(0), "the item at index 0 is removed, value that was removed is returned");
        Assert.equals(99, a.get(0), "item that was at i=1 should now be at i=0, expect previously set value of 99");
        Assert.equals(2, a.size(), "expect size to be 2 and not 3");
        a.remove(0);
        Assert.equals(30, a.remove(0), "expect last element to remove to pass, expect 30 return");
        Assert.throwsException(() -> a.remove(0), IndexOutOfBoundsException.class,
                "remove when no items throws exception");

        // --- M8: contains ---------------------------------------------------
        a.add(10);
        a.add(20);
        a.add(30);

        Assert.isTrue(a.contains(30), "list should return true for contains 30");
        Assert.isFalse(a.contains(12345), "list should return false for contains 12345");

        // --- M5: resize across the capacity boundary ------------------------
        DynamicArray<Integer> resizeTest = new DynamicArray<>(2);
        resizeTest.add(1);
        resizeTest.add(2);
        resizeTest.add(3);
        resizeTest.add(4);
        resizeTest.add(5);

        Assert.equals("[1, 2, 3, 4, 5]", resizeTest.toString(), "expect [1,2,3,4,5] array to become a string as [1, 2, 3, 4, 5]");
        Assert.equals(5, resizeTest.size(), "expect size to be 5");

        // --- M9: toString ---------------------------------------------------
        DynamicArray<Integer> toStringTest = new DynamicArray<>();
        toStringTest.add(1);
        toStringTest.add(2);
        toStringTest.add(3);
        Assert.equals("[1, 2, 3]", toStringTest.toString(), "expect [1,2,3] array to become a string as [1, 2, 3]");

        // ====================================================================
        // ADDITIONAL TESTS
        // ====================================================================

        // --- M10: insert — basic middle insert ------------------------------
        DynamicArray<Integer> ins = new DynamicArray<>();
        ins.add(1);
        ins.add(3);
        ins.insert(1, 2);  // [1, 2, 3]
        Assert.equals(3, ins.size(), "insert: size is 3 after insert");
        Assert.equals(1, ins.get(0), "insert: get(0) == 1 (unchanged)");
        Assert.equals(2, ins.get(1), "insert: get(1) == 2 (newly inserted)");
        Assert.equals(3, ins.get(2), "insert: get(2) == 3 (shifted right)");

        // insert at index 0 shifts everything right
        ins.insert(0, 0);  // [0, 1, 2, 3]
        Assert.equals(0, ins.get(0), "insert at 0: new element at front");
        Assert.equals(1, ins.get(1), "insert at 0: old first element shifted right");
        Assert.equals(4, ins.size(), "insert at 0: size is now 4");

        // insert at size() is equivalent to add
        ins.insert(ins.size(), 99);  // [0, 1, 2, 3, 99]
        Assert.equals(99, ins.get(ins.size() - 1), "insert at size(): appends to end");
        Assert.equals(5, ins.size(), "insert at size(): size increments");

        // --- M10: insert — bounds checking ----------------------------------
        Assert.throwsException(() -> ins.insert(-1, 42), IndexOutOfBoundsException.class,
                "insert(-1, x) throws");
        Assert.throwsException(() -> ins.insert(ins.size() + 1, 42), IndexOutOfBoundsException.class,
                "insert(size+1, x) throws");

        // --- M10: insert — on empty list ------------------------------------
        DynamicArray<Integer> insEmpty = new DynamicArray<>();
        insEmpty.insert(0, 7);
        Assert.equals(1, insEmpty.size(), "insert(0) on empty list: size is 1");
        Assert.equals(7, insEmpty.get(0), "insert(0) on empty list: value is accessible");
        Assert.throwsException(() -> insEmpty.insert(2, 7), IndexOutOfBoundsException.class,
                "insert beyond size on near-empty list throws");

        // --- M10: insert — triggers resize ----------------------------------
        DynamicArray<Integer> insResize = new DynamicArray<>(2);
        insResize.add(1);
        insResize.add(2);
        insResize.insert(1, 99);  // full array → resize, then insert: [1, 99, 2]
        Assert.equals(3, insResize.size(), "insert after resize: size is 3");
        Assert.equals(1, insResize.get(0), "insert after resize: get(0) == 1");
        Assert.equals(99, insResize.get(1), "insert after resize: get(1) == 99 (inserted)");
        Assert.equals(2, insResize.get(2), "insert after resize: get(2) == 2 (shifted)");

        // --- Edge cases: operations on empty list ---------------------------
        DynamicArray<Integer> empty = new DynamicArray<>();
        Assert.throwsException(() -> empty.get(0), IndexOutOfBoundsException.class,
                "get(0) on empty list throws");
        Assert.throwsException(() -> empty.remove(0), IndexOutOfBoundsException.class,
                "remove(0) on empty list throws");
        Assert.throwsException(() -> empty.set(0, 1), IndexOutOfBoundsException.class,
                "set(0, x) on empty list throws");
        Assert.isFalse(empty.contains(42), "contains on empty list returns false");
        Assert.equals("[]", empty.toString(), "toString on empty list returns '[]'");

        // --- set — bounds checking ------------------------------------------
        DynamicArray<Integer> setTest = new DynamicArray<>();
        setTest.add(10);
        Assert.throwsException(() -> setTest.set(-1, 5), IndexOutOfBoundsException.class,
                "set(-1, x) throws");
        Assert.throwsException(() -> setTest.set(1, 5), IndexOutOfBoundsException.class,
                "set(size, x) throws (off-by-one guard)");
        Assert.equals(10, setTest.set(0, 55), "set(0) at valid index returns old value");
        Assert.equals(55, setTest.get(0), "set(0) updates value correctly");

        // --- Constructor edge cases -----------------------------------------
        Assert.throwsException(() -> new DynamicArray<>(0), IllegalArgumentException.class,
                "capacity 0 throws IllegalArgumentException");
        Assert.throwsException(() -> new DynamicArray<>(-1), IllegalArgumentException.class,
                "capacity -1 throws IllegalArgumentException");

        // capacity 1 should work and resize on second add
        DynamicArray<Integer> capOne = new DynamicArray<>(1);
        capOne.add(42);
        Assert.equals(1, capOne.size(), "capacity-1 list accepts one element");
        Assert.equals(42, capOne.get(0), "capacity-1 list: get(0) == 42");
        capOne.add(99);  // triggers resize
        Assert.equals(2, capOne.size(), "capacity-1 list resizes and accepts second element");
        Assert.equals(99, capOne.get(1), "capacity-1 list after resize: get(1) == 99");

        // --- capacity() method ----------------------------------------------
        DynamicArray<Integer> capDefault = new DynamicArray<>();
        Assert.equals(15, capDefault.capacity(), "default capacity is 15");

        DynamicArray<Integer> capCustom = new DynamicArray<>(7);
        Assert.equals(7, capCustom.capacity(), "custom initial capacity is 7");
        for (int i = 0; i < 7; i++) capCustom.add(i);
        Assert.equals(7, capCustom.size(), "size == capacity before resize trigger");
        capCustom.add(99);  // triggers resize
        Assert.equals(14, capCustom.capacity(), "capacity doubles after resize (7 → 14)");
        Assert.equals(8, capCustom.size(), "size is correct after resize-triggered add");
        Assert.equals(99, capCustom.get(7), "value added during resize is accessible");

        // --- Multiple resizes: all elements survive -------------------------
        DynamicArray<Integer> multi = new DynamicArray<>(1);
        for (int i = 0; i < 100; i++) multi.add(i);
        Assert.equals(100, multi.size(), "100 elements survive multiple resizes");
        Assert.equals(0, multi.get(0), "first element correct after multiple resizes");
        Assert.equals(99, multi.get(99), "last element correct after multiple resizes");
        Assert.equals(50, multi.get(50), "middle element correct after multiple resizes");

        // --- remove: middle removal shifts correctly -------------------------
        DynamicArray<Integer> removeTest = new DynamicArray<>();
        removeTest.add(1);
        removeTest.add(2);
        removeTest.add(3);
        removeTest.add(4);
        removeTest.remove(1);  // removes 2 → [1, 3, 4]
        Assert.equals(3, removeTest.size(), "size decreases after remove from middle");
        Assert.equals(1, removeTest.get(0), "remove middle: element before is unchanged");
        Assert.equals(3, removeTest.get(1), "remove middle: next element shifted left");
        Assert.equals(4, removeTest.get(2), "remove middle: last element shifted left");

        // remove last element
        removeTest.remove(removeTest.size() - 1);  // removes 4 → [1, 3]
        Assert.equals(2, removeTest.size(), "size decreases after removing last element");
        Assert.equals(3, removeTest.get(1), "new last element is correct after removing old last");

        // --- contains: duplicates and post-remove ---------------------------
        DynamicArray<Integer> dupTest = new DynamicArray<>();
        dupTest.add(5);
        dupTest.add(5);
        dupTest.add(5);
        Assert.isTrue(dupTest.contains(5), "contains returns true when duplicates exist");
        dupTest.remove(0);
        dupTest.remove(0);
        Assert.isTrue(dupTest.contains(5), "contains still true with one duplicate remaining");
        dupTest.remove(0);
        Assert.isFalse(dupTest.contains(5), "contains false after all duplicates removed");

        // --- toString: edge cases -------------------------------------------
        Assert.equals("[]", new DynamicArray<>().toString(), "toString on fresh empty list is '[]'");

        DynamicArray<String> singleEl = new DynamicArray<>();
        singleEl.add("only");
        Assert.equals("[only]", singleEl.toString(), "toString with single element");

        // --- Null elements --------------------------------------------------
        DynamicArray<String> nullFirst = new DynamicArray<>();
        nullFirst.add(null);
        Assert.equals(1, nullFirst.size(), "add(null) increases size");
        Assert.isNull(nullFirst.get(0), "get(0) after add(null) returns null");
        Assert.isTrue(nullFirst.contains(null), "contains(null) when null is present returns true");
        Assert.isFalse(nullFirst.contains("x"), "contains('x') when only null present returns false");

        // null removed → contains(null) should return false
        nullFirst.remove(0);
        Assert.isFalse(nullFirst.contains(null), "contains(null) after null is removed returns false");

        // contains(null) when null is not the first element
        DynamicArray<String> nullMiddle = new DynamicArray<>();
        nullMiddle.add("first");
        nullMiddle.add(null);
        nullMiddle.add("third");
        Assert.isTrue(nullMiddle.contains(null), "contains(null) when null is not first element");
        Assert.isTrue(nullMiddle.contains("first"), "contains works with null in list");
        Assert.isFalse(nullMiddle.contains("missing"), "contains false for absent value when null in list");

        // --- String list (non-Integer generic type) -------------------------
        DynamicArray<String> strList = new DynamicArray<>();
        strList.add("alpha");
        strList.add("beta");
        strList.add("gamma");
        Assert.equals(3, strList.size(), "string list: size is 3");
        Assert.equals("beta", strList.get(1), "string list: get(1) == 'beta'");
        Assert.isTrue(strList.contains("gamma"), "string list: contains 'gamma'");
        Assert.isFalse(strList.contains("delta"), "string list: does not contain 'delta'");
        Assert.equals("[alpha, beta, gamma]", strList.toString(), "string list: toString correct");
        Assert.equals("beta", strList.remove(1), "string list: remove(1) returns 'beta'");
        Assert.equals("[alpha, gamma]", strList.toString(), "string list: toString after remove");

        Assert.summary();
    }
}

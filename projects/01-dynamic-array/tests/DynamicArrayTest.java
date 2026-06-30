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
        // TODO: assert set(1, 99) returns the old value (20), then get(1) == 99.

        // --- M7: remove -----------------------------------------------------
        // TODO: assert remove(0) returns 10, then get(0) == 20, size == 2.
        // TODO: edge case — remove the LAST element; remove down to empty.

        // --- M8: contains ---------------------------------------------------
        // TODO: assert contains(30) is true, contains(12345) is false.

        // --- M5: resize across the capacity boundary ------------------------
        // TODO: create a DynamicArray<>(2), add 5 items, assert all 5 survive
        //       in order and size == 5. This proves resize copied correctly.

        // --- M9: toString ---------------------------------------------------
        // TODO: build a small list and assert toString() == "[1, 2, 3]".

        // --- Edge cases (README §5) -----------------------------------------
        // TODO: empty-list get/remove/set all throw.
        // TODO: constructor with capacity 0 / -1 behaves per your policy.
        // TODO: null elements — add(null), contains(null) without crashing.

        Assert.summary();
    }
}

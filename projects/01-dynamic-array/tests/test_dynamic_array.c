/*
 * test_dynamic_array.c — YOUR test harness for Project 1.
 *
 * Run it with:   make test        (from projects/01-dynamic-array/)
 * Leak audit:    make && valgrind --leak-check=full --error-exitcode=1 ./out_test
 *
 * The suite is grouped by milestone (README §4/§5). Everything here is a
 * REACHABLE value check — no test deliberately trips an assert(), because an
 * assert aborts the whole binary and can't be caught (see the note at the end).
 *
 * Reading the output: each line is PASS/FAIL with the message. A FAIL prints the
 * expected vs. actual value and the file:line, so start at the FIRST failure and
 * work down — later failures are often just fallout from the first.
 */
#include "test_utils.h"      /* found via the Makefile's -I../../docs */
#include "dynamic_array.h"   /* found via the Makefile's -Isrc */

int main(void) {

    /* ====================================================================== */
    /* M1/M2: construction, size, is_empty, capacity, clean free              */
    /* ====================================================================== */

    DynamicArray *a = da_new();
    CHECK_INT(0, da_size(a),        "new list has size 0");
    CHECK_INT(1, da_is_empty(a),    "new list: is_empty returns exactly 1");
    CHECK(da_capacity(a) > 0,       "new list has a positive capacity");
    CHECK(da_size(a) <= da_capacity(a), "invariant holds on a new list: size <= capacity");

    /* da_with_capacity should honor the number you ask for. */
    DynamicArray *c = da_with_capacity(5);
    CHECK_INT(0, da_size(c),        "with_capacity(5): starts empty");
    CHECK_INT(5, da_capacity(c),    "with_capacity(5): capacity is exactly 5");
    da_free(c);

    /* Your capacity-0 policy: with_capacity(0) falls back to the default. It must
     * NOT hand back a 0-capacity buffer that can never hold anything. */
    DynamicArray *z = da_with_capacity(0);
    CHECK(da_capacity(z) > 0,       "with_capacity(0): falls back to a usable capacity");
    da_add(z, 7);                   /* must not overflow a zero-length buffer */
    CHECK_INT(1, da_size(z),        "with_capacity(0): can add after the fallback");
    CHECK_INT(7, da_get(z, 0),      "with_capacity(0): the added value is intact");
    da_free(z);

    /* da_free must be safe on NULL (no crash, no-op). */
    da_free(NULL);
    CHECK(1, "da_free(NULL) is a safe no-op");

    /* ====================================================================== */
    /* M3/M4: add + get                                                       */
    /* ====================================================================== */

    da_add(a, 10);
    da_add(a, 20);
    da_add(a, 30);
    CHECK_INT(3,  da_size(a),  "size is 3 after three adds");
    CHECK_INT(10, da_get(a, 0), "get(0) == 10");
    CHECK_INT(20, da_get(a, 1), "get(1) == 20");
    CHECK_INT(30, da_get(a, 2), "get(2) == 30");
    CHECK_INT(0,  da_is_empty(a), "is_empty returns exactly 0 after adds");

    /* EDGE: storing the value 0. da_elem is an int, and 0 is a perfectly legal
     * value. If add rejects 0 (e.g. an `item != NULL` guard), this aborts/fails. */
    DynamicArray *zeros = da_new();
    da_add(zeros, 0);
    da_add(zeros, 0);
    da_add(zeros, 0);
    CHECK_INT(3, da_size(zeros), "can store the value 0 (size counts it)");
    CHECK_INT(0, da_get(zeros, 1), "stored 0 reads back as 0");
    da_free(zeros);

    /* ====================================================================== */
    /* M6: set                                                                */
    /* ====================================================================== */

    CHECK_INT(20, da_set(a, 1, 99), "set(1,99) returns the OLD value 20");
    CHECK_INT(99, da_get(a, 1),     "get(1) == 99 after set");
    CHECK_INT(3,  da_size(a),       "set does NOT change size");
    CHECK_INT(10, da_set(a, 0, 11), "set at the first index returns old 10");
    CHECK_INT(30, da_set(a, 2, 33), "set at the last index returns old 30");

    /* ====================================================================== */
    /* M7: remove (shift-left, returns removed value, shrinks size)           */
    /* ====================================================================== */

    /* list is now [11, 99, 33] */
    CHECK_INT(11, da_remove(a, 0),  "remove(0) returns removed value 11");
    CHECK_INT(2,  da_size(a),       "size is 2 after remove");
    CHECK_INT(99, da_get(a, 0),     "elements shifted left: get(0) == 99");
    CHECK_INT(33, da_get(a, 1),     "elements shifted left: get(1) == 33");

    /* remove from the MIDDLE: [50, 60, 70] -> remove(1) -> [50, 70] */
    DynamicArray *m = da_new();
    da_add(m, 50); da_add(m, 60); da_add(m, 70);
    CHECK_INT(60, da_remove(m, 1),  "remove from middle returns 60");
    CHECK_INT(2,  da_size(m),       "size 2 after middle remove");
    CHECK_INT(50, da_get(m, 0),     "middle remove: get(0) == 50");
    CHECK_INT(70, da_get(m, 1),     "middle remove: get(1) == 70 (shifted)");

    /* remove the LAST element: [50, 70] -> remove(1) -> [50] */
    CHECK_INT(70, da_remove(m, 1),  "remove last returns 70");
    CHECK_INT(1,  da_size(m),       "size 1 after removing the last");
    CHECK_INT(50, da_get(m, 0),     "surviving element still readable");

    /* remove down to EMPTY, then add again (buffer must still be valid) */
    CHECK_INT(50, da_remove(m, 0),  "remove to empty returns 50");
    CHECK_INT(0,  da_size(m),       "size 0 after removing everything");
    CHECK_INT(1,  da_is_empty(m),   "is_empty is 1 after removing everything");
    da_add(m, 123);                 /* reuse after empty must work */
    CHECK_INT(1,   da_size(m),      "can add again after emptying");
    CHECK_INT(123, da_get(m, 0),    "re-added value is intact");
    da_free(m);

    /* ====================================================================== */
    /* M8: contains                                                           */
    /* ====================================================================== */

    /* a is currently [99, 33] */
    CHECK_INT(1, da_contains(a, 99),    "contains(99) true — first element");
    CHECK_INT(1, da_contains(a, 33),    "contains(33) true — LAST element (off-by-one guard)");
    CHECK_INT(0, da_contains(a, 12345), "contains(12345) false — absent");

    /* EDGE: contains on an EMPTY list must be a clean 0, not an underflow read.
     * If the loop bound is `i < size - 1` with size==0, size-1 wraps huge. */
    DynamicArray *e = da_new();
    CHECK_INT(0, da_contains(e, 42), "contains on empty list is false (no underflow)");
    da_free(e);

    da_free(a);   /* <-- must leave ASan/valgrind with ZERO leaks */

    /* ====================================================================== */
    /* M5: growth across the capacity boundary                                */
    /* ====================================================================== */

    DynamicArray *g = da_with_capacity(2);
    CHECK_INT(2, da_capacity(g), "growth start: capacity is exactly 2");
    for (int i = 0; i < 100; i++) da_add(g, i);
    CHECK_INT(100, da_size(g),      "100 elements survive many grows");
    CHECK(da_capacity(g) >= 100,    "capacity grew to hold at least 100");
    CHECK(da_size(g) <= da_capacity(g), "invariant after growth: size <= capacity");
    CHECK_INT(0,  da_get(g, 0),     "first element intact after grows");
    CHECK_INT(50, da_get(g, 50),    "middle element intact after grows");
    CHECK_INT(99, da_get(g, 99),    "last element intact after grows");
    da_free(g);

    /* EDGE: a LARGE requested capacity must allocate a matching buffer. If the
     * constructor allocates a fixed default but reports the requested capacity,
     * writing past the default overruns the heap — AddressSanitizer will catch it
     * here even though the plain asserts wouldn't. */
    DynamicArray *big = da_with_capacity(100);
    CHECK_INT(100, da_capacity(big), "with_capacity(100): capacity is exactly 100");
    for (int i = 0; i < 100; i++) da_add(big, i * 2);
    CHECK_INT(100, da_size(big),     "filled a large-capacity list without growing");
    CHECK_INT(0,   da_get(big, 0),   "large buffer: first element intact");
    CHECK_INT(198, da_get(big, 99),  "large buffer: last element intact (no overflow)");
    da_free(big);

    /* ---------------------------------------------------------------------- */
    /* NOT TESTED HERE — abort paths.                                         */
    /*   da_get(a, (size_t)-1), da_get on an out-of-range index, etc. trip an  */
    /*   assert(), which aborts the whole binary — you can't catch that in a   */
    /*   unit test. Verify those by hand, OR switch to return-code error       */
    /*   signaling and then assert on the codes with CHECK_INT.                */
    /* ---------------------------------------------------------------------- */

    return test_summary();
}

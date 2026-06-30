/**
 * Assert — a tiny, zero-dependency test helper for this course.
 *
 * WHY THIS EXISTS:
 *   You could pull in JUnit, but then you'd spend your first hour fighting build
 *   files instead of learning data structures. This single file gives you enough
 *   to test rigorously: value checks, boolean checks, and exception checks, plus
 *   a pass/fail summary at the end.
 *
 * HOW TO USE IT:
 *   1. Copy this file into your project's tests/ folder.
 *   2. In your test class, call Assert methods, then Assert.summary() at the end.
 *
 *   Example (in DynamicArrayTest.java):
 *
 *       public class DynamicArrayTest {
 *           public static void main(String[] args) {
 *               DynamicArray<Integer> a = new DynamicArray<>();
 *               Assert.equals(0, a.size(), "new array is empty");
 *               a.add(42);
 *               Assert.equals(1, a.size(), "size grows after add");
 *               Assert.equals(42, a.get(0), "stored value is retrievable");
 *               Assert.throwsException(() -> a.get(99),
 *                   IndexOutOfBoundsException.class, "out-of-range get throws");
 *               Assert.summary();   // prints results, exits non-zero if any failed
 *           }
 *       }
 *
 *   Compile & run:
 *       javac -d out src/*.java tests/*.java
 *       java  -cp out DynamicArrayTest
 *
 * GRADUATION GOAL:
 *   Once you're comfortable, rewrite a project's tests in JUnit 5. Knowing what a
 *   framework does for you (and doing without it first) is part of the education.
 */
public final class Assert {

    private static int passed = 0;
    private static int failed = 0;

    private Assert() { } // utility class: no instances

    /** Pass if expected and actual are equal (null-safe). */
    public static void equals(Object expected, Object actual, String message) {
        boolean ok = (expected == null) ? actual == null : expected.equals(actual);
        if (ok) {
            pass(message);
        } else {
            fail(message + "  [expected <" + expected + "> but got <" + actual + ">]");
        }
    }

    /** Pass if the condition is true. */
    public static void isTrue(boolean condition, String message) {
        if (condition) pass(message);
        else fail(message + "  [expected true]");
    }

    /** Pass if the condition is false. */
    public static void isFalse(boolean condition, String message) {
        isTrue(!condition, message);
    }

    /** Pass if the value is null. */
    public static void isNull(Object value, String message) {
        if (value == null) pass(message);
        else fail(message + "  [expected null but got <" + value + ">]");
    }

    /** Pass if the value is not null. */
    public static void notNull(Object value, String message) {
        if (value != null) pass(message);
        else fail(message + "  [expected non-null]");
    }

    /**
     * Pass if running the given code throws an exception of the expected type
     * (or a subclass). Use a lambda:  Assert.throwsException(() -> a.get(99), ...)
     */
    public static void throwsException(Runnable code,
                                       Class<? extends Throwable> expectedType,
                                       String message) {
        try {
            code.run();
            fail(message + "  [expected " + expectedType.getSimpleName()
                    + " but nothing was thrown]");
        } catch (Throwable t) {
            if (expectedType.isInstance(t)) {
                pass(message);
            } else {
                fail(message + "  [expected " + expectedType.getSimpleName()
                        + " but got " + t.getClass().getSimpleName() + "]");
            }
        }
    }

    private static void pass(String message) {
        passed++;
        System.out.println("  PASS  " + message);
    }

    private static void fail(String message) {
        failed++;
        System.out.println("  FAIL  " + message);
    }

    /** Print the tally. Call once at the very end of your test's main(). */
    public static void summary() {
        System.out.println("------------------------------------------------------------");
        System.out.println(passed + " passed, " + failed + " failed.");
        if (failed > 0) {
            System.out.println("SOME TESTS FAILED.");
            System.exit(1); // non-zero exit so CI / scripts notice
        } else {
            System.out.println("ALL TESTS PASSED.");
        }
    }
}

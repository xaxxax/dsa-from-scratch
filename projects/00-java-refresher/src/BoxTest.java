public class BoxTest {

    public static void main(String[] args) {
        IntBox<Integer> box = new IntBox<Integer>(4);

        try {
            box.get(0);
            box.push(4);
            box.push(6);
            box.push(7);
            box.get(-5);
            //box.get(5);
            //box.push(100);
            box.push(100);
            System.out.println(box.get(1));
            System.out.println(box.size());

            //IntBox invalid = new IntBox<Integer>(0);
        } catch (Error e) {
            System.out.println(e);
        }

    }

}

interface Box<E> {
        public void push(E value);
        public E get(int index);
        public int size();
        public boolean isFull();
    }

    @SuppressWarnings("unchecked")
    class IntBox<E> implements Box<E> {
        private final int capacity;
        private E[] data;
        private int length;

        public IntBox(int capacity) {
            if (capacity < 1) {
                throw new IllegalArgumentException("Capacity must be greater than 0");
            }

            this.capacity = capacity;
            this.data = (E[]) new Object[capacity];
        }

        public int getCapacity() {
            return capacity;
        }

        public void push(E value) {
            if(isFull()) {
                throw new IllegalStateException("Box is full"); 
            }

            data[length] = value;
            length++;
        }

        public E get(int index) {
            if (index < 0 || index >= capacity || index >= length) {
                throw new IndexOutOfBoundsException("Index " + (index) + " is out of bounds for capacity " + capacity); 
            }

            return data[index];
        }

        public int size() {
            return length;
        }

        public boolean isFull() {
            return length == capacity;
        }
    }

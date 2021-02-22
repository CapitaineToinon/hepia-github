import java.util.Iterator;
import java.util.NoSuchElementException;
import java.util.Optional;

public class LinkedListInt implements ListInt {
    private Optional<Node> head = Optional.empty();
    private int size = 0;

    private Optional<Node> head() {
        return this.head;
    }

    @Override
    public void insert(Integer value) {
        this.size += 1;

        if (this.head.isEmpty()) {
            this.head = Optional.of(Node.tail(value));
            return;
        }

        var tail = this.head.get();

        while (tail.next.isPresent())
            tail = tail.next.get();

        tail.next = Optional.of(Node.tail(value));
    }

    @Override
    public void insertAll(Integer first, Integer... others) {
        insert(first);
        for (Integer i : others)
            insert(i);
    }

    @Override
    public Integer get(int index) {
        if (index >= size)
            throw new IndexOutOfBoundsException("Index out of bounds");

        var element = this.head.get();
        while (index-- > 0)
            element = element.next.get();

        return element.value();
    }

    @Override
    public int size() {
        return this.size;
    }

    @Override
    public void clear() {
        this.head = Optional.empty();
        this.size = 0;
    }

    @Override
    public Iterator<Integer> iterator() {
        var that = this;

        return new Iterator<Integer>() {
            private Optional<Node> current = that.head();

            @Override
            public boolean hasNext() {
                return this.current.isPresent();
            }

            @Override
            public Integer next() {
                if (!this.hasNext())
                    throw new NoSuchElementException("No Such Element");

                var value = this.current.get().value();
                this.current = this.current.get().next();
                return value;
            }
        };
    }

    // Private node class used for the linked list
    private static class Node {
        private Integer value;
        private Optional<Node> next;

        private Node(Integer value, Optional<Node> next) {
            this.value = value;
            this.next = next;
        }

        private Integer value() {
            return this.value;
        }

        private Optional<Node> next() {
            return this.next;
        }

        private static Node tail(Integer value) {
            return new Node(value, Optional.empty());
        }
    }
}

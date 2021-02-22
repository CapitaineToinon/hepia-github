package ch.hepia.counter;

public class Counter implements Comparable<Counter> {
    private int count;
    private int max;

    public Counter() {
        this(0, 9999);
    }

    private Counter(int count, int max) {
        this.count = count;
        this.max = max;
    }

    private void setCount(int value) {
        count = (value > max) ? 0 : value;
    }

    private int getCount() {
        return count % (max + 1);
    }

    public int value() {
        return getCount();
    }

    public void click() {
        setCount(count + 1);
    }

    public void init() {
        count = 0;
    }

    @Override
    public String toString() {
        return String.format("%04d", value());
    }

    @Override
    public boolean equals(Object o) {
        if (o == this)
            return true;

        if (o == null)
            return false;

        if (getClass() != ch.hepia.counter.Counter.class)
            return false;

        Counter c = (Counter) o;
        return c.compareTo(c) == 0;
    }

    @Override
    public int compareTo(Counter o) {
        return Integer.compare(value(), o.value());
    }

    @Override
    public int hashCode() {
        return value();
    }
}

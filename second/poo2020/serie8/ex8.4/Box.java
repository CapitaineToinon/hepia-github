import java.util.function.Function;

public class Box<T extends Comparable<T>> implements Comparable<T> {
    private final T t;

    public Box(T t) {
        this.t = t;
    }

    public T get() {
        return this.t;
    }

    @Override
    public String toString() {
        return String.format("[%s]", t);
    }

    public <R extends Comparable<R>> Box<R> map(Function<T, R> f) {
        return new Box<R>(f.apply(this.t));
    }

    @Override
    public int compareTo(T o) {
        return t.compareTo(o);
    }

    public static <T extends Comparable<T>> boolean isBigger(Box<T> b1, Box<T> b2) {
        return b1.compareTo(b2.get()) == 1;
    }
}

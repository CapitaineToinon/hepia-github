import java.util.function.Function;

public class Pair<T, E> {
    private T left;
    private E right;

    @FunctionalInterface
    interface Function2<T, E, X, Y> {
        public Pair<X, Y> accept(T left, E right);
    }

    public Pair(T left, E right) {
        this.left = left;
        this.right = right;
    }

    public T left() {
        return this.left;
    }

    public E right() {
        return this.right;
    }

    public <R> R mapLeft(Function<T, R> f) {
        return f.apply(this.left);
    }

    public <R> R mapRight(Function<E, R> f) {
        return f.apply(this.right);
    }

    public <X, Y> Pair<X, Y> map(Function2<T, E, X, Y> f) {
        return f.accept(this.left, this.right);
    }
}

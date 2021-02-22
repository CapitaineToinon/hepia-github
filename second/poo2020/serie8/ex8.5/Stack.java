import java.util.Optional;
import java.util.function.Consumer;

public interface Stack<T> {
    T pop();

    default Optional<T> popOption() {
        return isEmpty() ? Optional.empty() : Optional.of(pop());
    }

    default void ifHeadIsPresent(Consumer<T> consumer) {
        peekOption().ifPresent(consumer);
    }

    void push(T i);

    int size();

    T peek();

    default Optional<T> peekOption() {
        return isEmpty() ? Optional.empty() : Optional.of(peek());
    }

    default boolean isEmpty() {
        return size() == 0;
    }
}

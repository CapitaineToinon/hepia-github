import java.util.Optional;
import java.util.function.Consumer;

public interface IntegerStack {
    int pop();

    default Optional<Integer> popOption() {
        try {
            return Optional.of(pop());
        } catch (EmptyIntegerStack e) {
            return Optional.empty();
        }
    }

    default void ifHeadIsPresent(Consumer<Integer> consumer) {
        var maybe = peekOption();
        if (maybe.isPresent()) {
            consumer.accept(maybe.get());
        }
    }

    void push(int i);

    int size();

    int peek();

    default Optional<Integer> peekOption() {
        try {
            return Optional.of(peek());
        } catch (EmptyIntegerStack e) {
            return Optional.empty();
        }
    }

    default boolean isEmpty() {
        return size() == 0;
        // or
        // return peekOption().isEmpty();
    }
}

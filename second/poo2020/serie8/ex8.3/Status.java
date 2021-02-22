import java.util.function.Consumer;
import java.util.function.Function;
import java.util.function.Supplier;

interface Status<T, E> {
    boolean isOn();

    boolean isOff();

    boolean isError();

    T get();

    E getError();

    void accept(Consumer<T> consumerIfOn, Runnable runnableIfOff, Consumer<E> consumerIfError);

    T getOrElse(Supplier<T> supplierIfOff, Function<E, T> functionIfError);

    public static <T, S> Status<T, S> onWith(T status) {
        return new On<T, S>(status);
    }

    public static <T, S> Status<T, S> off() {
        return new Off<T, S>();
    }

    public static <T, S> Status<T, S> errorWith(S error) {
        return new Error<T, S>(error);
    }
}
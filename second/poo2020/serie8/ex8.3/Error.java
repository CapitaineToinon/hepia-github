import java.util.function.Consumer;
import java.util.function.Function;
import java.util.function.Supplier;

class Error<T, E> implements Status<T, E> {
    private E error;

    public Error(E error) {
        this.error = error;
    }

    @Override
    public boolean isOn() {
        return false;
    }

    @Override
    public boolean isOff() {
        return false;
    }

    @Override
    public boolean isError() {
        return true;
    }

    @Override
    public T get() {
        throw new RuntimeException("There are no value to get.");
    }

    @Override
    public E getError() {
        return this.error;
    }

    @Override
    public void accept(Consumer<T> consumerIfOn, Runnable runnableIfOff, Consumer<E> consumerIfError) {
        consumerIfError.accept(this.error);
    }

    @Override
    public T getOrElse(Supplier<T> supplierIfOff, Function<E, T> functionIfError) {
        return functionIfError.apply(this.error);
    }
}
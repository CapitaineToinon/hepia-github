import java.util.function.Consumer;
import java.util.function.Function;
import java.util.function.Supplier;

class On<T, E> implements Status<T, E> {
    private T status;

    public On(T status) {
        this.status = status;
    }

    @Override
    public boolean isOn() {
        return true;
    }

    @Override
    public boolean isOff() {
        return false;
    }

    @Override
    public boolean isError() {
        return false;
    }

    @Override
    public T get() {
        return this.status;
    }

    @Override
    public E getError() {
        throw new RuntimeException("Status of type On don't have an error.");
    }

    @Override
    public void accept(Consumer<T> consumerIfOn, Runnable runnableIfOff, Consumer<E> consumerIfError) {
        consumerIfOn.accept(this.status);
    }

    @Override
    public T getOrElse(Supplier<T> supplierIfOff, Function<E, T> functionIfError) {
        return this.status;
    }
}
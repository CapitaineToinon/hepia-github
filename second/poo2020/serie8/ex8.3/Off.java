import java.util.function.Consumer;
import java.util.function.Function;
import java.util.function.Supplier;

class Off<T, E> implements Status<T, E> {
    @Override
    public boolean isOn() {
        return false;
    }

    @Override
    public boolean isOff() {
        return true;
    }

    @Override
    public boolean isError() {
        return false;
    }

    @Override
    public T get() {
        return null;
    }

    @Override
    public E getError() {
        return null;
    }

    @Override
    public void accept(Consumer<T> consumerIfOn, Runnable runnableIfOff, Consumer<E> consumerIfError) {
        runnableIfOff.run();
    }

    @Override
    public T getOrElse(Supplier<T> supplierIfOff, Function<E, T> functionIfError) {
        return supplierIfOff.get();
    }
}
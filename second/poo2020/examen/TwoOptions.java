import java.util.ArrayList;
import java.util.List;
import java.util.function.Consumer;

public class TwoOptions<T> {
    private List<T> options = new ArrayList<T>(2);

    public TwoOptions() {

    }

    public TwoOptions(T first) {
        this.options.add(0, first);
    }

    public TwoOptions(T first, T second) {
        this.options.add(0, first);
        this.options.add(1, second);
    }

    public boolean isEmpty() {
        return this.options.isEmpty();
    }

    public boolean isFirstValuePresent() {
        return this.options.size() >= 1;
    }

    public boolean isSecondValuePresent() {
        return this.options.size() >= 2;
    }

    public T firstValue() {
        if (!isFirstValuePresent())
            throw new RuntimeException("First value is not present.");

        return this.options.get(0);
    }

    public T secondValue() {
        if (!isSecondValuePresent())
            throw new RuntimeException("Second value is not present.");

        return this.options.get(1);
    }

    public void apply(Consumer<? super T> consumer) {
        this.options.forEach(consumer);
    }
}

import java.util.Optional;
import java.util.function.Supplier;

public class Main {
    public static void main(String[] args) {
        System.out.println(test(() -> 3));
        System.out.println(test(() -> 3 + 4));
        System.out.println(test(() -> 3 / 0));
        System.out.println(test(() -> null));
        System.out.println(test(() -> {
            throw new RuntimeException();
        }));
    }

    public static Optional<Integer> test(Supplier<Integer> supplier) {
        try {
            return Optional.of(supplier.get());
        } catch (Exception e) {
            return Optional.empty();
        }
    }
}
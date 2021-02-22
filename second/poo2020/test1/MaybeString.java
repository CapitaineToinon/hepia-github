public interface MaybeString {
    default public String get() {
        throw new RuntimeException("There is nothing to get.");
    }

    default public boolean exists() {
        return false;
    }
}

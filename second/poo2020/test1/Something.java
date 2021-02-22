public class Something implements MaybeString {
    private String value;

    public Something(String value) {
        if (value == null)
            throw new RuntimeException("Value cannot be null");

        this.value = value;
    }

    @Override
    public boolean exists() {
        return true;
    }

    @Override
    public String get() {
        return value;
    }
}

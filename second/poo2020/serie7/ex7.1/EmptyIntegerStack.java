public class EmptyIntegerStack extends RuntimeException {
    private static final long serialVersionUID = 1L;

    public EmptyIntegerStack() {
        super("Stack is empty.");
    }
}

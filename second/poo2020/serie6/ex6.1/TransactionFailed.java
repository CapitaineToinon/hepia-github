public class TransactionFailed implements Transaction {
    private String message;

    public TransactionFailed(String message) {
        this.message = message;
    }

    @Override
    public void ifSucceedOrElse(Success success, Failure failure) {
        failure.accept(message);
    }
}

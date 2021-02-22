public class TransactionSuccess implements Transaction {
    private Account origin;
    private Account destination;
    private int amount;

    public TransactionSuccess(Account origin, Account destination, int amount) {
        this.origin = origin;
        this.destination = destination;
        this.amount = amount;
    }

    @Override
    public void ifSucceedOrElse(Success success, Failure failure) {
        success.accept(origin, destination, amount);
    }
}

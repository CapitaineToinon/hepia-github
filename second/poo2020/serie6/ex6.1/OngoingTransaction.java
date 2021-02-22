public class OngoingTransaction {
    private Account origin;
    private Account destination;

    public OngoingTransaction(Account origin, Account destination) {
        this.origin = origin;
        this.destination = destination;
    }

    public Transaction amount(int amount) {
        try {
            this.origin.withdraw(amount);
            this.destination.deposit(amount);
            return new TransactionSuccess(origin, destination, amount);
        } catch (RuntimeException e) {
            return new TransactionFailed(e.getMessage());
        }
    }
}
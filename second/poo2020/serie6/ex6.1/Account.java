public class Account {
    private static final int MIN_BALANCE = -2000;

    private String name;
    private int amount;

    private Account(String name) {
        this.name = name;
    }

    public String name() {
        return this.name;
    }

    public int amount() {
        return this.amount;
    }

    public Account withAmount(int amount) {
        Account.checkPositiveOrThrow(amount);
        this.amount = amount;
        return this;
    }

    public Account deposit(int amount) {
        this.amount += amount;
        return this;
    }

    public Account withdraw(int amount) {
        Account.checkPositiveOrThrow(amount);

        if (this.amount() - amount < Account.MIN_BALANCE)
            throw new RuntimeException("Withdraw impossible.");

        this.amount -= amount;
        return this;
    }

    public OngoingTransaction transferTo(Account destination) {
        return new OngoingTransaction(this, destination);
    }

    private static void checkPositiveOrThrow(int i) {
        if (i < 0)
            throw new RuntimeException("Amount should be positive");
    }

    public static Account withName(String name) {
        return new Account(name);
    }
}

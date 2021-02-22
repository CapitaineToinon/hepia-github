public class Account implements Comparable<Account> {
    private static final int MIN_BALANCE = -2000;

    private String firstname;
    private String lastname;
    private int amount;

    public String fullName() {
        return String.format("%s %s", firstname, lastname);
    }

    public int amount() {
        return this.amount;
    }

    public Account(String firstname, String lastname, int amount) {
        this.firstname = firstname;
        this.lastname = lastname;
        this.deposit(amount);
    }

    public Account(String firstname, String lastname) {
        this(firstname, lastname, 0);
    }

    public boolean hasAtLeast(int value) {
        return amount() >= value;
    }

    private static void checkPositiveOrThrow(int i) {
        if (i < 0)
            throw new RuntimeException("Amount should be positive");
    }

    public void deposit(int amountToPlace) {
        Account.checkPositiveOrThrow(amountToPlace);
        this.amount += amountToPlace;
    }

    public void withdraw(int amountToWithdraw) {
        Account.checkPositiveOrThrow(amountToWithdraw);

        if (this.amount() - amountToWithdraw < Account.MIN_BALANCE)
            throw new RuntimeException("withdraw unpossible");

        this.amount -= amountToWithdraw;
    }

    public void transferTo(Account destination, int amountToTransfer) {
        this.withdraw(amountToTransfer);
        destination.deposit(amountToTransfer);
    }

    @Override
    public int compareTo(Account o) {
        int cmpNames = o.fullName().compareTo(this.fullName());
        int cmpBalances = Integer.compare(o.amount, this.amount);

        if (cmpNames == 0 && cmpBalances == 0) {
            return 0;
        }

        if (cmpNames == 0) {
            return cmpBalances;
        }

        return cmpNames;
    }

    @Override
    public boolean equals(Object o) {
        if (o == this) {
            return true;
        }

        if (o.getClass() != this.getClass()) {
            return false;
        }

        return this.compareTo((Account) o) == 0;
    }
}
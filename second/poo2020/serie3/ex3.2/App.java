public class App {
    public static void main(String[] args) {
        // new account without money using ctor
        {
            Account account = new Account("Antoine", "Sutter");
        }

        // new account with money using ctor. Can't be bellow -2000
        {
            Account account = new Account("Antoine", "Sutter", 2000);

            try {
                Account failed = new Account("Antoine", "Sutter", -5000);
            } catch (Exception e) {
                System.out.println(e);
            }
        }

        // deposite, withdraw, transfer, name, equal
        {
            Account BillGates = new Account("Bill", "Gates", 1000);
            Account me = new Account("Antoine", "Sutter", 0);

            me.deposit(1000);
            me.withdraw(50);
            System.out.println(me.amount());

            BillGates.transferTo(me, 1000);
            System.out.println(me.amount());

            System.out.println(me.fullName());
            System.out.println(me.equals(BillGates));

            Account me2 = new Account("Antoine", "Sutter", me.amount());
            System.out.println(me.equals(me2));
        }
    }
}

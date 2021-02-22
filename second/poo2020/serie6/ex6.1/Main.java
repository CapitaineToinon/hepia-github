public class Main {
    public static void main(String[] args) {
        Account stephanie = Account.withName("Stéphanie").withAmount(1000);
        Account julia = Account.withName("Julia");
        Account stephanie2 = stephanie.deposit(1000);

        // this works
        stephanie2.transferTo(julia).amount(300).ifSucceedOrElse((origin, dest, amount) -> {
            System.out.println("All good");
            System.out.println(String.format("%s now has %d", origin.name(), origin.amount()));
            System.out.println(String.format("%s now has %d", dest.name(), dest.amount()));
        }, error -> {
            System.out.println(error);
        });

        // this fails because stephanie only has 1700 left
        stephanie2.transferTo(julia).amount(1_000_000).ifSucceedOrElse((origin, dest, amount) -> {
            System.out.println("All good");
            System.out.println(String.format("%s now has %d", origin.name(), origin.amount()));
            System.out.println(String.format("%s now has %d", dest.name(), dest.amount()));
        }, error -> {
            System.out.println(error);
        });
    }
}
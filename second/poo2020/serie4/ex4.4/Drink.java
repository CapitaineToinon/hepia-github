public enum Drink {
    BiereBlance("Bière Blance", 4.5, 4.0), BiereAmbree("Bière Ambrée", 4.5, 4.0), SodaCola("Soda Cola", 4.5, 4.0),
    Whisky("Whisky", 4.5, 4.0);

    private final String name;
    private final double percent;
    private final double price;

    private Drink(String name, double percent, double price) {
        this.name = name;
        this.percent = percent;
        this.price = price;
    }

    public boolean hasAlcohol() {
        return this.Percent() > 0;
    }

    public double Percent() {
        return this.percent;
    }

    public double Price() {
        return this.price;
    }

    public String Name() {
        return this.name;
    }

    public static void checkAge(Drink drink) {
        if (!drink.hasAlcohol()) {
            System.out.println("Bravo");
        } else {
            var age = (drink.Percent() > 6.0) ? "18" : "16";
            System.out.println(String.format("You need to be over %s.", age));
        }

        System.out.println(String.format("Name: %s - Price: %s", drink.Name(), drink.Price()));
    }
}

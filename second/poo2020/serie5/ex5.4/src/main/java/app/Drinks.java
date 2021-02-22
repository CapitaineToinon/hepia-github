package app;

public enum Drinks implements Drink {
    WhiteBeer(3, 4), Soda(1, 3);

    private final int socialBenefit;
    private final int price;

    private Drinks(int socialBenefit, int price) {
        this.socialBenefit = socialBenefit;
        this.price = price;
    }

    @Override
    public int socialBenefit() {
        return this.socialBenefit;
    }

    @Override
    public int price() {
        return this.price;
    }
}

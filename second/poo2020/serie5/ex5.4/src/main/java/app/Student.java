package app;

public class Student implements Socializer {
    private double socialScore;
    private int money;

    public Student() {
        this.socialScore = 0;
        this.money = 20;
    }

    @Override
    public double getSocialScore() {
        return this.socialScore;
    }

    @Override
    public void take(Drink drink) {
        if (this.money >= drink.price()) {
            this.socialScore += drink.socialBenefit();
            this.money -= drink.price();
        }
    }
}

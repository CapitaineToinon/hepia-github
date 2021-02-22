package app;

public class Teacher implements Socializer {
    private double socialScore;

    public Teacher() {
        this.socialScore = 0;
    }

    @Override
    public double getSocialScore() {
        return this.socialScore;
    }

    @Override
    public void take(Drink drink) {
        this.socialScore += ((double) drink.socialBenefit() / 2.0);
    }
}

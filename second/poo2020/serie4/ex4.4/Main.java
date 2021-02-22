import java.util.List;
import java.util.ArrayList;

public class Main {
    public static void main(String[] args) {
        List<Drink> drinks = new ArrayList<>();
        drinks.add(Drink.BiereBlance);
        drinks.add(Drink.BiereAmbree);
        drinks.add(Drink.SodaCola);
        drinks.add(Drink.Whisky);

        drinks.stream().forEach(drink -> Drink.checkAge(drink));
    }
}
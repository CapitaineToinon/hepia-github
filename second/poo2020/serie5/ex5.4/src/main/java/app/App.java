package app;

/**
 * Hello world!
 *
 */
public class App {
    public static void main(String[] args) {
        Student antoine = new Student();
        Teacher bob = new Teacher();

        System.out.println("Antoine score: " + antoine.getSocialScore());
        System.out.println("Bob score: " + bob.getSocialScore());

        for (int i = 0; i < 100; i++) {
            antoine.take(Drinks.Soda); // will run out of money
            bob.take(Drinks.Soda);
        }

        System.out.println("Antoine score: " + antoine.getSocialScore());
        System.out.println("Bob score: " + bob.getSocialScore());
    }
}

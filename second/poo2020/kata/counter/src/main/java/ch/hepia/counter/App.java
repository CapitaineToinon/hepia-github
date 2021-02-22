package ch.hepia.counter;

/**
 * Hello world!
 *
 */
public class App {
    public static void main(String[] args) {
        Counter object = new Counter();

        object.click();
        object.click();
        object.click();

        System.out.println(object.value());
        System.out.println(object);

        object = new Counter();

        object.click();
        object.click();
        object.click();

        System.out.println(object.value());
        System.out.println(object);
    }
}

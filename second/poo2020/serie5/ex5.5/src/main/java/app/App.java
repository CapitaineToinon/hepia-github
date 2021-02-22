package app;

/**
 * Hello world!
 *
 */
public class App {
    public static void main(String[] args) {
        Status s1 = Status.process();

        if (s1.isOn()) {
            System.out.println("The device is on.");
        } else if (s1.isOff()) {
            System.out.println("The device is off.");
        } else {
            System.out.println("The device is unstable " + s1.getErrorMessage());
        }

        Status s2 = Status.makeError("Oups !!!");
        System.out.println("The device is unstable " + s2.getErrorMessage());
    }
}

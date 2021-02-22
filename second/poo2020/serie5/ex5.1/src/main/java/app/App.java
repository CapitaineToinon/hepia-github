package app;

/**
 * Hello world!
 *
 */
public class App {
    public static void main(String[] args) {
        Properties properties = new Properties();
        properties.addProperty("IP", "127.0.0.1");
        properties.addProperty("name", "Antoine");
        properties.addProperty("bob", "test");

        System.out.println(properties.getProperty("IP"));
        System.out.println(properties.getPropertyOrElse("IP", "lmao"));
        System.out.println(properties.getPropertyOrElse("lmao", "IP"));
        System.out.println(properties.keys());
        System.out.println(properties.values());
        System.out.println(properties.allProperties());
    }
}

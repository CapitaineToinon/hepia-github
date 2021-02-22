package app;

/**
 * Hello world!
 *
 */
public class App {
    public static void main(String[] args) {
        ListInt list = new ArrayListInt();
        list.insert(0);
        list.insert(3);
        list.insertAll(2, 1);

        System.out.println("Size is " + list.size());

        for (int i = 0; i < list.size(); i++) {
            int v = list.get(i);
            System.out.println("Value of v : " + v);
        }

        list.clear();
        System.out.println("Size is " + list.size());
    }
}

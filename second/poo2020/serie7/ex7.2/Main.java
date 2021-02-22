import java.util.NoSuchElementException;

public class Main {
    public static void main(String[] args) {
        ListInt list = new LinkedListInt();
        list.insert(0);
        list.insert(3);
        list.insertAll(2, 1);

        System.out.println("Size is " + list.size());

        for (int i = 0; i < list.size(); i++) {
            int v = list.get(i);
            System.out.println("Value of v : " + v);
        }

        try {
            list.get(list.size());
        } catch (IndexOutOfBoundsException e) {
            System.out.println(e.getMessage());
        }

        {
            System.out.println("Using iterator");
            var it = list.iterator();
            while (it.hasNext()) {
                System.out.println(it.next());
            }
        }

        try {
            System.out.println("Using iterator but next when list is empty");
            var it = list.iterator();
            while (true) {
                System.out.println(it.next());
            }
        } catch (NoSuchElementException e) {
            System.out.println(e.getMessage());
        }

        list.clear();
        System.out.println("Size is " + list.size());
    }
}

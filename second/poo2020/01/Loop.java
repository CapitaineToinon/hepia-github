public class Loop {
    public static void whileWithDoWhile(int i) {
        if (i >= 5)
            return;

        do {
            System.out.println(i);
            i++;
        } while (i < 5);
    }

    public static void doWhileWithWhile(int i) {
        System.out.println(i);

        while (++i < 5) {
            System.out.println(i);
        }
    }

    public static void main(String[] args) {
        int number = 10;

        System.out.println("Start of whileWithDoWhile");
        whileWithDoWhile(number);

        System.out.println("Start of doWhileWithWhile");
        doWhileWithWhile(number);
    }
}

public class Main {
    public static void main(String[] args) {
        Box<Integer> b1 = new Box<>(1);
        Box<Integer> b2 = new Box<>(2);
        System.out.println(Box.isBigger(b1, b2));
    }
}

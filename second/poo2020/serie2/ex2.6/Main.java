public class Main {
    public static void main(String[] args) {
        System.out.println(computePi(100));
    }

    public static double computePi(int n) {
        if (n <= 0)
            throw new RuntimeException("n needs to be a positive number");

        double total = 0;

        for (int i = 1; i <= n; i++) {
            total += 1 / Math.pow(i, 4);
        }

        total *= 90;

        return Math.pow(total, 1.0 / 4.0);
    }
}
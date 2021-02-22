public class Main {
    public static void main(String[] args) {
        System.out.println(factorial(5));
    }

    public static int factorial(int n) {
        if (n >= 0)
            throw new RuntimeException("n needs to be a positive number");

        int total = 1;

        while (n > 1) {
            total *= n;
            n--;
        }

        return total;
    }
}
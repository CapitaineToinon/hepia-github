public class Main {
    public static void main(String[] args) {
        printSapin(5);
    }

    public static void printSapin(int height) {
        if (height < 0)
            throw new RuntimeException("height cannot be smaller than 0");

        for (int i = 0, j = height - 1; i < height; i++, j--)
            System.out.println(" ".repeat(j) + "*".repeat((2 * i) + 1));
    }
}

public class Main {
    public static void main(String[] args) {
        printTriangle(4);
    }

    public static void printTriangle(int height) {
        if (height < 0)
            throw new RuntimeException("height cannot be smaller than 0");

        for (int i = 0; i < height; i++)
            System.out.println("*".repeat(i + 1));
    }
}

import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        int snakeSize = 1;

        char increase = '+';
        char decrease = '-';
        char b = ' ';
        Scanner s = new Scanner(System.in);

        while (true) {
            printSnake(snakeSize);

            do {
                b = ' ';
                System.out.print("Direction: ");
                String str = s.nextLine();

                if (str.length() > 0)
                    b = str.charAt(0);
            } while (Character.compare(increase, b) != 0 && Character.compare(decrease, b) != 0);

            if (Character.compare(increase, b) == 0) {
                snakeSize++;
            }

            if (Character.compare(decrease, b) == 0) {
                snakeSize--;
            }

            if (snakeSize <= 0) {
                System.out.println("Au revoir!");
                break;
            }
        }

        s.close();
    }

    public static void printSnake(int size) {
        String snake = String.format("%s%s", "*".repeat(size - 1), "X");
        System.out.println(snake);
    }
}
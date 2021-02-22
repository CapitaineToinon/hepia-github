package askint;

import java.util.Scanner;

public class App {
    private static Scanner scanner = new Scanner(System.in);

    public static void main(String[] args) {
        System.out.println("La méthode a retourné " + askInt());
        System.out.println("La méthode a retourné " + askInt2());
        scanner.close();
    }

    private static int askInt() {
        int num = 0;
        boolean loop = true;

        while (loop) {
            System.out.print("Enter a number: ");

            if (scanner.hasNextInt()) {
                num = scanner.nextInt();
                loop = false;
            } else {
                System.out.println("Sorry, couldn't understand you!");
                scanner.next();
            }
        }

        return num;
    }

    private static int askInt2() {
        while (true) {
            int num = 0;
            String input = "";

            do {
                System.out.print("Enter a number: ");
                input = scanner.nextLine();
            } while (input.length() <= 0);

            try {
                num = Integer.parseInt(input);
            } catch (Exception e) {
                System.out.println("Sorry, couldn't understand you!");
                continue;
            }

            return num;
        }
    }
}

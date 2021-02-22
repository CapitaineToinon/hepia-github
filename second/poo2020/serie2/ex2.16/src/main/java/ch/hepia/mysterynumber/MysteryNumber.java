package ch.hepia.mysterynumber;

import java.util.Random;
import java.util.Scanner;

public class MysteryNumber implements AutoCloseable {
    private Random random;
    private Scanner scanner;
    private int min;
    private int max;

    public MysteryNumber(int min, int max) {
        if (min >= max)
            throw new RuntimeException("min needs to be smaller than max");

        this.min = min;
        this.max = max;
        this.random = new Random();
        this.scanner = new Scanner(System.in);
    }

    public void close() {
        this.scanner.close();
    }

    public void Play() {
        int attempts = 0;
        int secret = getNewRandom();
        int guess;

        while (true) {
            attempts++;
            guess = askNextInt();

            if (guess < min || guess > max) {
                System.out.println(String.format("The number needs to be between %s and %s.", min, max));
                continue;
            }

            if (guess > secret) {
                System.out.println("The mystery number is smaller.");
                continue;
            }

            if (guess < secret) {
                System.out.println("The mystery number is bigger.");
                continue;
            }

            if (guess == secret) {
                System.out.println(String.format("Congratz, you found the mystery number in %s attempt%s.", attempts,
                        (attempts > 1) ? "s" : ""));
                break;
            }
        }
    }

    public void ComputerPlay() {
        int attempts = 0;
        int middle;
        int currentMin = min;
        int currentMax = max;
        int previous = max + 1;

        char higher = '+';
        char lower = '-';
        char correct = 'o';
        char c = ' ';

        while (true) {
            attempts++;
            middle = currentMin + ((currentMax - currentMin) / 2);

            if (previous == middle) {
                System.out.println("I think you're either lying or confused. Please play again from the start.");
                return;
            }

            do {
                c = ' ';
                String msg = String.format("Is your mystery number %s? [%c/%c/%c] ", middle, correct, higher, lower);
                System.out.print(msg);
                String str = this.scanner.nextLine();

                if (str.length() > 0)
                    c = str.charAt(0);

            } while (Character.compare(higher, c) != 0 && Character.compare(lower, c) != 0
                    && Character.compare(correct, c) != 0);

            previous = middle;

            if (Character.compare(higher, c) == 0) {
                currentMin = middle;
                continue;
            }

            if (Character.compare(lower, c) == 0) {
                currentMax = middle;
                continue;
            }

            if (Character.compare(correct, c) == 0) {
                System.out.println(String.format("I found your mystery number is %s attempts.", attempts));
                return;
            }

        }
    }

    private int askNextInt() {
        int num = 0;
        boolean loop = true;

        while (loop) {
            System.out.print("Enter a number: ");

            if (this.scanner.hasNextInt()) {
                num = this.scanner.nextInt();
                loop = false;
            } else {
                System.out.println("Sorry, couldn't understand you!");
                this.scanner.next();
            }
        }

        return num;
    }

    private int getNewRandom() {
        return random.nextInt(this.max - this.min) + this.min;
    }
}

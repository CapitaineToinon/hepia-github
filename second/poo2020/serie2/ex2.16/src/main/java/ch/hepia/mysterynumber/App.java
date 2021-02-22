package ch.hepia.mysterynumber;

/**
 * Hello world!
 *
 */
public class App {
    public static void main(String[] args) {
        if (args.length < 1) {
            printHelp();
            return;
        }

        int max;
        boolean cpu = false;

        if (args.length > 0) {
            switch (args[0]) {
                case "help":
                    printHelp();
                    return;
                case "guess":
                    cpu = true;

                    if (args.length <= 1)
                        throw new RuntimeException("2nd argument needs to be a number that the CPU will try to guess");

                    try {
                        max = (int) Integer.parseInt(args[1]);
                    } catch (NumberFormatException e) {
                        throw new RuntimeException(
                                "2nd arguement needs to be a int. Use the help command to learn more.");
                    }

                    break;
                default:
                    cpu = false;

                    try {
                        max = (int) Integer.parseInt(args[0]);
                    } catch (NumberFormatException e) {
                        throw new RuntimeException(
                                "1st arguement needs to be a int or the word 'guess' then followed by a int. Use the help command to learn more.");
                    }

                    break;
            }

            MysteryNumber game = new MysteryNumber(0, max);

            if (cpu)
                game.ComputerPlay();
            else
                game.Play();

            game.close();
        }
    }

    private static void printHelp() {
        System.out.println("****************************");
        System.out.println("To guess a number the computer generated between 0 and n:");
        System.out.println("usage: ./mysterynumber n");
        System.out.println("For example, guess a number between 0 and 100:");
        System.out.println("Example: ./mysterynumber 100");
        System.out.println("****************************");
        System.out.println("To have the computer guess a number you chose between 0 and n:");
        System.out.println("usage: ./mysterynumber n");
        System.out.println("For example, guess a number between 0 and 1024:");
        System.out.println("Example: ./mysterynumber guess 1024");
        System.out.println("****************************");
    }
}

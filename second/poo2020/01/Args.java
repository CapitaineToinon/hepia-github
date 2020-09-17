public class Args {
    /**
     * Main for exercice 1.6
     * 
     * @param args
     */
    public static void main(String[] args) {
        // Do this
        for (String arg : args) {
            System.out.println(arg);
        }

        // or this
        for (int i = 0; i < args.length; i++) {
            System.out.println(args[i]);
        }
    }
}

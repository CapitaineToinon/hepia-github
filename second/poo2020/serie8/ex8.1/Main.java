public class Main {
    public static void main(String[] args) {
        var pair = new Pair<Integer, String>(25, "Antoine");
        System.out.println(pair.left());
        System.out.println(pair.right());

        String s = pair.mapLeft(i -> Integer.toString(i));
        String upper = pair.mapRight(string -> string.toUpperCase());

        Pair<Double, Integer> out = pair.map((Integer left, String right) -> {
            return new Pair<Double, Integer>((double) left, right.length());
        });

        System.out.println(out.left());
        System.out.println(out.right());
    }
}
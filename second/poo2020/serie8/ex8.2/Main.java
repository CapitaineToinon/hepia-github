public class Main {
    public static void main(String[] args) {
        var pair = new Pair<Integer, String>(25, "Antoine");
        System.out.println(pair.key());
        System.out.println(pair.value());

        String newKey = pair.mapKey(key -> Integer.toString(key));
        Integer newValue = pair.mapValue((String value) -> value.length());

        System.out.println(newKey);
        System.out.println(newValue);

        var newPair = pair.map((Integer key) -> Integer.toString(key), (String value) -> value.length());
        System.out.println(newPair.key());
        System.out.println(newPair.value());

        var unique = new UniquePair<Integer>(10, 15);
        var numers = new NumberPair(10, 15);
    }
}
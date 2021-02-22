import java.util.List;
import java.util.stream.Collectors;

public class ListHelper {
    public static void main(String[] args) {
        System.out.println(doubleThat(List.of(1, 2, 3, 4)));
    }

    /* Retourne une nouvelle liste où toutes les valeurs sont doublées */
    public static List<Integer> doubleThat(List<Integer> is) {
        return is.stream().map(n -> n * 2).collect(Collectors.toList());
    }

    /*
     * Retourne une nouvelle liste où toutes les valeurs sont plus grandes ou égale
     * à un seuil
     */
    public static List<Integer> filterGreaterOrEqual(List<Integer> is, int value) {
        return is.stream().filter(n -> n >= value).collect(Collectors.toList());
    }

    /*
     * Retourne une nouvelle liste où toutes les valeurs positives sont multipliées
     * par deux. Les valeurs négatives sont omises
     */
    public static List<Integer> filterPositiveAndThenDoubleThem(List<Integer> is) {
        return doubleThat(filterGreaterOrEqual(is, 0));
    }
}
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class App {
    public static void main(String[] args) {
        System.out.println(isNumeric("42"));
        System.out.println(isNumeric(" 22 "));
        System.out.println(isNumeric(" -33 "));
        System.out.println(isNumeric(" 22.0 "));
        System.out.println(isNumeric("2f3"));

        for (int i : indexes("maison", 'i'))
            System.out.println(i);
    }

    /*
     * Détermine si un nombre qui se trouve dans une chaîne de caractères contient
     * un entier Note: prend en compte le signe, mais ne prend pas en compte les
     * espaces Exemples: isNumeric("42") -> true isNumeric(" 22 ") -> true
     * isNumeric(" -33 ") -> true * isNumeric(" 22.0") -> false isNumeric("2f3") ->
     * false
     */
    public static boolean isNumeric(String term) {
        try {
            Integer.parseInt(term.trim());
            return true;
        } catch (NumberFormatException e) {
            return false;
        }
    }

    /*
     * Retourne un tableau d'indices où chaque valeur indique la position d'un *
     * caractère dans une chaîne Exemples: indexes("maison", 'i') -> {2}
     * indexes("tralala", 'a') -> {2,4,6} indexes("coucou", 'x') -> {}
     */
    public static int[] indexes(String term, char c) {
        char[] chars = term.toCharArray();
        List<Integer> indexes = new ArrayList<>();

        for (int i = 0; i < chars.length; i++)
            if (chars[i] == c)
                indexes.add(i);

        return indexes.stream().mapToInt(i -> i).toArray();
    }

    /*
     * Détermine si une chaîne est en majuscule * Conseil: cf. docs String Note:
     * peut tenir sur une ligne Exemples: isUpper("ABC") -> true isUpper("AbC") ->
     * false
     */
    public static boolean isUpper(String term) {
        return term.toUpperCase() == term;
    }

    /*
     * Retourne une version triée d'une chaine de caractères * Conseil: cf. docs
     * String + Arrays Exemples: sorted("lkjlkjKJk9") -> "9JKjjkkkll"
     */
    public static String sorted(String term) {
        char tempArray[] = term.toCharArray();
        Arrays.sort(tempArray);
        return new String(tempArray);
    }
}
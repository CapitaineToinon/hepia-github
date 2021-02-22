public class Test1 {
    /**
     * Supprime une lettre d'une chaîne de caractères
     * removeFirstOccurrence("abracadabra", 'a') retournerait "bracadabra"
     * removeFirstOccurrence("babar", 'r') retournerait "baba"
     * removeFirstOccurrence("babar", 'z') retournerait "babar"
     * removeFirstOccurrence("", 'r') retournerait ""
     */
    public static String removeFirstOccurrence(String s, char letter) {
        /* méthode fournie */
        /* Rien à faire ici */
        String regex = String.format("%s", letter);
        return s.replaceFirst(regex, "");
    }

    /**
     * Retourne true si la lettre se trouve dans le mot contains("babar", 'z')
     * retournerait false contains("babar", 'a') retournerait true contains("", 'a')
     * retournerait false
     */
    public static boolean contains(String s, char letter) {
        /* méthode fournie */
        return s.indexOf(letter) != -1;
    }

    /**
     * Supprime toutes les occurrences d'une lettre dans un mot. Retourne la version
     * sans occurrence. removeAll("abracadabra", 'a') retournerait "brcdbr" *
     * removeAll("salsa", 's') retournerait "ala" removeAll("biere", 's')
     * retournerait "biere"
     */
    public static String removeAll(String s, char letter) {
        String copy = s;

        while (contains(copy, letter)) {
            copy = removeFirstOccurrence(copy, letter);
        }

        return copy;
    }

    public static void main(String[] args) {
        /* Rien à faire ici */
        System.out.println(removeAll("abracadabra", 'a'));
        System.out.println(removeAll("salsa", 's'));
        System.out.println(removeAll("biere", 's'));
    }
}
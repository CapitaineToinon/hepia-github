import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;

public class dummy {

  static HashMap<String, Integer> words = new HashMap<String, Integer>();
  static HashMap<String, HashSet<Integer>> lines = new HashMap<String, HashSet<Integer>>();

  public static void main(String[] args) {
    test("Hello", 1);
    test("Hello", 1);
    test("Bonjour", 1);
    test("BonjPuteour", 2);
    test("Hello", 2);

    System.out.println(words.get("Hello"));
    System.out.println(lines.get("Hello"));

    System.out.println(words.get("Bonjour"));
    System.out.println(lines.get("Bonjour"));

    System.out.println(words.get("BonjPuteour"));
    System.out.println(lines.get("BonjPuteour"));

    for (Map.Entry<String, Integer> word : words.entrySet()) {
      HashSet<Integer> wordLines = lines.get(word.getKey());
      System.out.printf("%s %d %s\n", word.getKey(), word.getValue(), wordLines.toString());
    }
  }

  public static void test(String word, Integer line) {
    Integer count = words.getOrDefault(word, 0);
    words.put(word, count + 1);

    HashSet<Integer> wordLines = lines.getOrDefault(word, new HashSet<Integer>());
    wordLines.add(line);
    lines.put(word, wordLines);
  }
}

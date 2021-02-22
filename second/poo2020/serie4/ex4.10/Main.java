import java.util.Arrays;

public class Main {
    public static void main(String[] args) {
        int[] is = { 1, 2, 3, 4, 5 };

        for (int i : append(is, 10))
            System.out.println(i);
    }

    public static int[] append(int[] array, int item) {
        int[] copy = Arrays.copyOf(array, array.length + 1);

        for (int i = copy.length - 1; i > 0; i--)
            copy[i] = copy[i - 1];

        copy[0] = item;
        return copy;
    }
}
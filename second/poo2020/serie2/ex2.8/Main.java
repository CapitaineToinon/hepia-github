import java.util.ArrayList;

public class Main {

    public static void printArray(double[] array) {
        System.out.print(String.format("double[%s] { ", array.length));
        for (double v : array)
            System.out.print(String.format("%s, ", v));
        System.out.print("}\n");
    }

    public static void main(String[] args) {
        /* add */
        printArray(add(new double[] { 1.0, 2.0, 2.0 }, new double[] { 2.0, 1.0, -5.0 }));
    }

    public static double[] add(double[] v1, double[] v2) {
        if (v1.length != v2.length)
            throw new RuntimeException("v1 and v2 needs to be of the same length");

        double[] sum = new double[v1.length];

        for (int i = 0; i < v1.length; i++) {
            sum[i] = v1[i] + v2[i];
        }

        return sum;
    }
}
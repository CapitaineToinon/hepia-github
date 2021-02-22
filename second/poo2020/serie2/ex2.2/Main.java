import java.util.ArrayList;
import java.util.List;

public class Main {
    public static void printSection(String section) {
        System.out.println("------- " + section + "-------");
    }

    public static void main(String[] args) {
        printSection("minMaxRange");
        System.out.println(minMaxRange(1.0, 0.0, 3.0));
        System.out.println(minMaxRange(5.0, 0.0, 3.0));
        System.out.println(minMaxRange(-4.3, 0.0, 3.0));

        printSection("random");
        for (int i = 0; i < 10; i++)
            System.out.println(random(0, 100));

        printSection("normalize");
        System.out.println(normalize(0.0, 0.0, 1.0, 500.0, 600.0));
        System.out.println(normalize(1.0, 0.0, 1.0, 500.0, 600.0));
        System.out.println(normalize(0.5, 0.0, 1.0, 500.0, 600.0));
        System.out.println(normalize(0.0, 0.0, 1.0, 500.0, 600.0));

        printSection("altitudeToShadesOfGray");
        List<List<Double>> altitudes = List.of(List.of(500.0, 550.0, 600.00), List.of(570.0, 510.0));
        System.out.println(altitudeToShadesOfGray(altitudes));
    }

    public static double minMaxRange(double value, double minValue, double maxValue) {
        if (maxValue < minValue)
            throw new RuntimeException("maxValue cannot be smaller than minValue");

        return Math.max(Math.min(value, maxValue), minValue);
    }

    public static double normalize(double value, double minSource, double maxSource, double minTarget,
            double maxTarger) {

        if (maxSource < minSource)
            throw new RuntimeException("minSource cannot be smaller than maxSource");

        if (maxTarger < minTarget)
            throw new RuntimeException("minTarget cannot be smaller than maxTarger");

        double sourceRange = maxSource - minSource;
        double targetRange = maxTarger - minTarget;

        return minTarget + (targetRange * ((value - minSource) / sourceRange));
    }

    public static double random(double minValue, double maxValue) {
        if (maxValue < minValue)
            throw new RuntimeException("maxValue cannot be smaller than minValue");

        double range = maxValue - minValue;
        return minValue + (Math.random() * range);
    }

    public static int random(int minValue, int maxValue) {
        if (maxValue < minValue)
            throw new RuntimeException("maxValue cannot be smaller than minValue");

        int range = maxValue - minValue;
        return minValue + (int) (Math.random() * range);
    }

    public static List<Double> random(int nb, double minValue, double maxValue) {
        if (nb < 0)
            throw new RuntimeException("nb cannot be smaller than 0");

        List<Double> list = List.of();

        for (int i = 0; i < nb; i++)
            list.add(random(minValue, maxValue));

        return list;
    }

    public static List<Integer> random(int nb, int minValue, int maxValue) {
        if (nb < 0)
            throw new RuntimeException("nb cannot be smaller than 0");

        List<Integer> list = List.of();

        for (int i = 0; i < nb; i++)
            list.add(random(minValue, maxValue));

        return list;
    }

    public static List<List<Double>> altitudeToShadesOfGray(List<List<Double>> altitudes) {
        List<List<Double>> myList = new ArrayList<List<Double>>();

        double minAltitude = Double.MAX_VALUE;
        double maxAltitude = Double.MIN_NORMAL;

        for (List<Double> vector : altitudes) {
            for (double altitude : vector) {
                if (altitude < minAltitude)
                    minAltitude = altitude;

                if (altitude > maxAltitude)
                    maxAltitude = altitude;
            }
        }

        for (List<Double> vector : altitudes) {
            List<Double> newVector = new ArrayList<Double>();

            for (double altitude : vector) {
                newVector.add(normalize(altitude, minAltitude, maxAltitude, 0.0, 255.0));
            }

            myList.add(newVector);
        }

        return myList;
    }
}
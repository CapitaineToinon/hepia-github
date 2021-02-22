public class Main {
    public static void main(String[] args) {
        for (Day d : Day.values())
            System.out.println(d + " is the day number: " + d.dayOfTheWeek());
    }
}
public class Main {
    public static void main(String[] args) {
        double d = 0.0;
        float f = 0.0f;
        int i = 0;
        byte b = 0;

        A ab = new B();
        A ac = new C();

        ab.test(b); // B: short
        ac.test(b); // A: short
        System.out.println("***");

        ab.test(d); // A: double
        ac.test(d); // A: double
        System.out.println("***");

        ab.test(f); // A: double
        ac.test(f); // A: double
        System.out.println("***");

        ab.test(i); // A: double
        ac.test(i); // A: double
    }
}
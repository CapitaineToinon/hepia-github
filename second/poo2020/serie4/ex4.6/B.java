public class B extends A {
    @Override
    public void f(short s) {
        System.out.println("Called from B");
    }

    // surcharge
    public void f(byte b) {
        System.out.println("Called from B");
    }

    @Override
    public void f(int i) {
        System.out.println("Called from B");
    }
}

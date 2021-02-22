import java.io.Serializable;

public class A {
    public void f(int i) {
        System.out.println("Called from A");
    }

    public void f(short s) {
        System.out.println("Called from A");
    }

    public Serializable g(Integer i) {
        System.out.println("Called from A");
        return null;
    }
}

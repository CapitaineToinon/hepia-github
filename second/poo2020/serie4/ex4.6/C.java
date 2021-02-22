import java.io.Serializable;

public class C extends A {
    @Override
    public String g(Integer i) {
        return null;
    }

    // surcharge
    // but cannot do that because it already exists here ^
    // public Serializable g(Integer i) {
    // return null;
    // }

    // surcharge
    public Serializable g(Object o) {
        return null;
    }
}

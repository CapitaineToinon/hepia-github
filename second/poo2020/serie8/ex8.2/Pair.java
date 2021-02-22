import java.util.function.Function;

public class Pair<K, V> {
    private K key;
    private V value;

    public Pair(K key, V value) {
        this.key = key;
        this.value = value;
    }

    public K key() {
        return this.key;
    }

    public V value() {
        return this.value;
    }

    public <R> R mapKey(Function<K, R> f) {
        return f.apply(this.key);
    }

    public <R> R mapValue(Function<V, R> f) {
        return f.apply(this.value);
    }

    public <RK, RV> Pair<RK, RV> map(Function<K, RK> fk, Function<V, RV> fv) {
        return new Pair<RK, RV>(fk.apply(this.key), fv.apply(this.value));
    }
}

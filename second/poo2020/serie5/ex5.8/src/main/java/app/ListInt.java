package app;

public interface ListInt {
    public void insert(Integer value);

    public void insertAll(Integer first, Integer... others);

    public Integer get(int index);

    public int size();

    public void clear();

    default public boolean isEmpty() {
        return size() == 0;
    }

    default public void addAll(Integer first, Integer... others) {
        insertAll(first, others);
    }
}

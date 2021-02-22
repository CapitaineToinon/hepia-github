package app;

import java.util.Arrays;

public class ArrayListInt implements ListInt {
    private final int CHUNK_SIZE = 10;
    private Integer[] list = new Integer[CHUNK_SIZE];;
    private int nextIndex = 0;

    public ArrayListInt() {

    }

    public void insert(Integer value) {
        if (this.nextIndex > this.list.length - 1) {
            this.list = Arrays.copyOf(this.list, this.list.length + CHUNK_SIZE);
        }

        this.list[nextIndex] = value;
        this.nextIndex++;
    }

    public void insertAll(Integer first, Integer... others) {
        this.insert(first);
        for (Integer i : others)
            this.insert(i);
    }

    public Integer get(int index) {
        if (index > this.nextIndex)
            throw new RuntimeException("Index out of bound range");
        return this.list[index];
    }

    public int size() {
        return this.nextIndex;
    }

    public void clear() {
        this.list = new Integer[CHUNK_SIZE];
        this.nextIndex = 0;
    }
}

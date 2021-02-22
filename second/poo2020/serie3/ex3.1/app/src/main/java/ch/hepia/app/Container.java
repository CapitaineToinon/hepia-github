package ch.hepia.app;

import java.util.List;

public class Container {
    int quantity;
    int maxCapacity;

    public void setQuantity(int value) {
        this.quantity = (value > this.maxCapacity) ? this.maxCapacity : value;
    }

    public int quantity() {
        return this.quantity;
    }

    public Container(int capacity) {
        this.maxCapacity = capacity;
        this.setQuantity(0);
    }

    public void fillToTheMax() {
        this.setQuantity(this.maxCapacity);
    }

    public void fillWith(int amount) {
        this.setQuantity(amount);
    }

    public void fillTo(Container first, Container... others) {
        int value;

        List<Container> containers = ContainerHelpers.mergeContainers(first, others);

        for (Container c : containers) {
            value = Math.min(c.maxCapacity, this.quantity());
            value = Math.min(value, c.remaining());

            c.fillWith(c.quantity() + value);
            this.remove(value);

            if (this.remaining() == 0)
                return;
        }
    }

    public boolean isFull() {
        return this.remaining() == 0;
    }

    public int remaining() {
        return this.maxCapacity - this.quantity;
    }

    public void remove(int amount) {
        this.setQuantity(this.quantity() - amount);
    }

    public void flush() {
        this.setQuantity(0);
    }

    public static Container withCapacity(int capacity) {
        return new Container(capacity);
    }
}

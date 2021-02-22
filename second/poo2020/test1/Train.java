import java.util.ArrayList;
import java.util.List;

public class Train {
    private int capacity;
    private List<Integer> wagons;

    private Train(int capacity) {
        this.capacity = capacity;
        this.wagons = new ArrayList<>();
    }

    public void insert(int quantity) {
        if (quantity > this.capacity)
            throw new RuntimeException("The quantity is bigger than the capacity of a single wagon.");

        if (this.wagons.isEmpty()) {
            this.wagons.add(quantity);
            return;
        }

        for (int i = 0; i < this.wagons.size(); i++) {
            int quantityLast = this.wagons.get(i);

            if (quantityLast + quantity <= capacity) {
                this.wagons.set(i, quantityLast + quantity);
                return;
            }
        }

        // needs to add a new wagon
        this.wagons.add(quantity);
    }

    public int nbWagons() {
        return this.wagons.size();
    }

    public int totalWeight() {
        return this.wagons.stream().reduce(0, Integer::sum);
    }

    @Override
    public String toString() {
        return String.format("Train(%s)", this.wagons.toString());
    }

    public static Train withCapacity(int capacity) {
        return new Train(capacity);
    }
}

package ch.hepia.app;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class ContainerHelpers {
    public static List<Container> mergeContainers(Container first, Container[] others) {
        List<Container> containers = new ArrayList<Container>();
        containers.add(first);
        containers.addAll(Arrays.asList(others));
        return containers;
    }

    public static void transfer(Container origin, Container first, Container... others) {
        int value;

        List<Container> containers = ContainerHelpers.mergeContainers(first, others);

        for (Container c : containers) {
            value = Math.min(c.maxCapacity, origin.quantity());
            value = Math.min(value, c.remaining());

            c.fillWith(c.quantity() + value);
            origin.remove(value);

            if (origin.remaining() == 0)
                return;
        }
    }
}
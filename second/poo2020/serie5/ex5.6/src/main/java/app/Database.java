package app;

import java.util.List;

public interface Database {
    public void add(Device d);

    Device device(String id);

    List<Device> devices();

    default boolean exists(String id) {
        return this.devices().stream().filter(e -> e.id().equals(id)).findAny().orElse(null) != null;
    }

    public default int count() {
        return this.devices().size();
    }

    public Device remove(String id);
}

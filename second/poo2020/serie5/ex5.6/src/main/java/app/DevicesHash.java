package app;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

public class DevicesHash implements Database {
    private HashMap<String, Device> map;

    public DevicesHash() {
        this.map = new HashMap<>();
    }

    @Override
    public void add(Device d) {
        this.map.put(d.id(), d);
    }

    @Override
    public Device device(String id) {
        Device d = this.map.get(id);

        if (d == null)
            throw new RuntimeException("Device not found");

        return d;
    }

    @Override
    public List<Device> devices() {
        return new ArrayList<>(this.map.values());
    }

    @Override
    public Device remove(String id) {
        Device d = this.device(id);
        this.map.remove(id);
        return d;
    }
}

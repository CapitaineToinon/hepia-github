package app;

import java.util.ArrayList;
import java.util.List;

public class DevicesList implements Database {
    private List<Device> devices;

    public DevicesList() {
        this.devices = new ArrayList<>();
    }

    @Override
    public void add(Device d) {
        this.devices.add(d);
    }

    @Override
    public Device device(String id) {
        Device d = this.devices.stream().filter(e -> e.id().equals(id)).findAny().orElse(null);

        if (d == null)
            throw new RuntimeException("Device not found");

        return d;
    }

    @Override
    public List<Device> devices() {
        return this.devices;
    }

    @Override
    public Device remove(String id) {
        Device d = this.device(id);
        this.devices.remove(d);
        return d;
    }
}

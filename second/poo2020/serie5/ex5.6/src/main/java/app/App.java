package app;

import java.util.ArrayList;
import java.util.List;

public class App {
    public static void main(String[] args) {
        Database db = new DevicesList();
        populate(db, 100);

        System.out.println("They are " + db.devices().size() + " devices in the database.");

        Pair<Integer> result = stabilize(db);

        System.out.println(String.format("Took %d start and %d reboot", result.first, result.second));
    }

    public static void populate(Database db, int count) {
        for (int i = 0; i < count; i++)
            db.add(new Computer());
    }

    public static Pair<Integer> stabilize(Database db) {
        List<List<Device>> partioned = partition(db);

        int start = 0;
        int reboot = 0;

        while (partioned.get(0).size() != db.devices().size()) {
            List<Device> off = partioned.get(1);
            List<Device> err = partioned.get(2);

            start += off.size();
            reboot += err.size();

            off.forEach(d -> d.start());
            err.forEach(d -> d.reboot());
            partioned = partition(db);
        }

        return new Pair<Integer>(start, reboot);
    }

    public static List<List<Device>> partition(Database db) {
        List<Device> on = new ArrayList<>();
        List<Device> off = new ArrayList<>();
        List<Device> err = new ArrayList<>();

        for (Device device : db.devices()) {
            if (device.status().isOn())
                on.add(device);
            if (device.status().isOff())
                off.add(device);
            if (device.status().isError())
                err.add(device);
        }

        return List.of(on, off, err);
    }
}

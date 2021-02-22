package app;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import ch.hepia.devices.database.*;
import ch.hepia.devices.status.*;
import ch.hepia.devices.device.*;

public class App {
    public static void main(String[] args) {
        Database db = new InMemoryDatabase();
        populate(db, 10000);

        Map<Class<? extends Status>, List<Device>> devices = partition(db);
        List<Device> on = devices.getOrDefault(Status.On.class, new ArrayList<>());
        List<Device> off = devices.getOrDefault(Status.Off.class, new ArrayList<>());
        List<Device> err = devices.getOrDefault(Status.Err.class, new ArrayList<>());

        System.out.println(String.format("There are %s devices on, %s off and %s in an error state.", on.size(),
                off.size(), err.size()));
    }

    public static void populate(Database db, int count) {
        for (int i = 0; i < count; i++)
            db.add(StateDevice.random());
    }

    public static Map<Class<? extends Status>, List<Device>> partition(Database db) {
        Map<Class<? extends Status>, List<Device>> map = new HashMap<>();

        for (Device d : db.devices()) {
            List<Device> devices = map.getOrDefault(d.status().getClass(), new ArrayList<Device>());
            devices.add(d);
            map.put(d.status().getClass(), devices);
        }

        return map;
    }
}

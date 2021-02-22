package app;

import java.util.UUID;

public abstract class Device {
    private String id;
    private Status status;

    public Device() {
        this.id = UUID.randomUUID().toString();

        RandomCollection<Status> collection = new RandomCollection<>();
        collection.add(50, new On());
        collection.add(30, new Off());
        collection.add(20, new Error("undefined error"));

        this.status = collection.next();
    }

    public String id() {
        return this.id;
    }

    public Status status() {
        return this.status;
    }

    public void start() {
        if (!this.status.isOn()) {
            RandomCollection<Status> collection = new RandomCollection<>();
            collection.add(70, new On());
            collection.add(30, new Error("undefined error"));
            this.status = collection.next();
        }
    }

    public void reboot() {
        if (this.status.isOn() || this.status.isError()) {
            RandomCollection<Status> collection = new RandomCollection<>();
            collection.add(70, new On());
            collection.add(30, new Error("undefined error"));
            this.status = collection.next();
        }
    }

    public void shutdown() {
        if (!this.status.isOff())
            this.status = new Off();
    }
}

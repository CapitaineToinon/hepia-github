package app;

import java.util.Random;

public interface Status {
    default public boolean isOn() {
        return false;
    }

    default public boolean isOff() {
        return false;
    }

    default public boolean isError() {
        return false;
    }

    default public String getErrorMessage() {
        throw new RuntimeException("undefined error");
    }

    public static Status process() {
        switch (new Random().nextInt(3)) {
            case 0:
                return new On();
            case 1:
                return new Off();
            case 2:
            default:
                return new app.Error("oups");
        }
    }

    public static Status makeError(String message) {
        return new app.Error(message);
    }
}

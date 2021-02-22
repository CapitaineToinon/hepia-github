package app;

public class Off implements Status {
    @Override
    public boolean isOff() {
        return true;
    }

    @Override
    public String getErrorMessage() {
        throw new RuntimeException("Status of type on has no error message.");
    }
}

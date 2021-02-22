package app;

public class On implements Status {
    @Override
    public boolean isOn() {
        return true;
    }

    @Override
    public String getErrorMessage() {
        throw new RuntimeException("Status of type on has no error message.");
    }
}

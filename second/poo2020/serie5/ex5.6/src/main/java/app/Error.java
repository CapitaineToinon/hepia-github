package app;

public class Error implements Status {
    private String message;

    public Error(String message) {
        this.message = message;
    }

    @Override
    public boolean isError() {
        return true;
    }

    @Override
    public String getErrorMessage() {
        return this.message;
    }
}

public interface Transaction {
    @FunctionalInterface
    interface Failure {
        public void accept(String message);
    }

    @FunctionalInterface
    interface Success {
        public void accept(Account origin, Account destination, int amount);
    }

    public default void ifSucceedOrElse(Success success, Failure failure) {
        throw new RuntimeException("Not implemented");
    }
}

import java.util.UUID;
import java.util.function.Consumer;

public class Result {
    private Result() {

    }

    public UUID resultId() {
        return UUID.randomUUID();
    }

    public static Result merge(Task t1, Task t2, String param) {
        return new Result();
    }

    public void save(String path, Runnable finished) {

    }

    public void onFinish(Consumer<Result> consumer) {

    }
}

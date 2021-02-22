import java.util.ArrayList;
import java.util.List;

public class IntegerArrayStack implements Stack<Integer> {
    private List<Integer> stack;

    public IntegerArrayStack() {
        this.stack = new ArrayList<Integer>();
    }

    @Override
    public Integer pop() {
        if (stack.isEmpty())
            throw new EmptyIntegerStack();

        int index = stack.size() - 1;
        int value = stack.get(index);
        stack.remove(index);
        return value;
    }

    @Override
    public void push(Integer i) {
        stack.add(i);
    }

    @Override
    public int size() {
        return stack.size();
    }

    @Override
    public Integer peek() {
        if (stack.isEmpty())
            throw new EmptyIntegerStack();

        return stack.get(stack.size() - 1);
    }
}

public class Main {
    public static void main(String[] args) {
        Stack<Integer> stack = new IntegerArrayStack();
        stack.push(1);
        stack.push(2);
        stack.push(3);
        stack.ifHeadIsPresent(i -> System.out.println(i));
    }
}

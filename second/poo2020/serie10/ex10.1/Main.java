public class Main {
    public static void main(String[] args) {
        {
            Box<? extends B> box = new Box<D>(new D());
            // D d = box.get(); <- Doesn't work, get returns B
            B b = box.get();
            // box.set(new E()); <- fucked
        }

        {
            Box<? super B> box = new Box<A>(new C());
            // D d = box.get(); <- Not working cause get returns Object
            // B b = box.get(); <- Not working cause get returns Object
            box.set(new D());
        }

        {
            Box<A> aa = new Box<A>(new B());
            // Box<B> bd = new Box<D>(new D());
            // Missmatch of declarations, even if D extends B

            // Box<? extends C> ca = new Box<A>(new G()); // A doesn't extends C
            Box<? extends C> cgi = new Box<G>(new I());
            Box<? extends C> cgh = new Box<G>(new H());
            Box<? super G> g = new Box<A>(new B());
            Box<? super D> d = new Box<B>(new E());
            // Box<? super D> da = new Box<C>(new C()); // C doesn't extend D
        }
    }
}

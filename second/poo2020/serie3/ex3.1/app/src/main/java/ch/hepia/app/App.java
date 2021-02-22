package ch.hepia.app;

public class App {
    public static void main(String[] args) {
        /**
         * EXERCICE 3.1.1
         */
        {
            System.out.println("**************** 3.1.1 ****************");
            Container origin = Container.withCapacity(10);
            Container dest1 = Container.withCapacity(3);
            Container dest2 = Container.withCapacity(3);
            Container dest3 = Container.withCapacity(3);

            origin.fillToTheMax();
            ContainerHelpers.transfer(origin, dest1, dest2, dest3);

            System.out.println("dest1.quantity() -> " + dest1.quantity());
            System.out.println("dest2.quantity() -> " + dest2.quantity());
            System.out.println("dest3.quantity() -> " + dest3.quantity());
        }

        /**
         * EXERCICE 3.1.1
         */
        {
            System.out.println("**************** 3.1.1 ****************");
            Container origin = Container.withCapacity(10);
            Container dest1 = Container.withCapacity(3);
            Container dest2 = Container.withCapacity(3);
            Container dest3 = Container.withCapacity(3);
            Container dest4 = Container.withCapacity(3);

            origin.fillToTheMax();
            ContainerHelpers.transfer(origin, dest1, dest2, dest3, dest4);

            System.out.println("dest1.quantity() -> " + dest1.quantity());
            System.out.println("dest2.quantity() -> " + dest2.quantity());
            System.out.println("dest3.quantity() -> " + dest3.quantity());
            System.out.println("dest4.quantity() -> " + dest4.quantity());
        }

        /**
         * EXERCICE 3.1.1
         */
        {
            System.out.println("**************** 3.1.1 ****************");
            Container origin = Container.withCapacity(5);
            Container dest1 = Container.withCapacity(3);

            origin.fillToTheMax();
            ContainerHelpers.transfer(origin, dest1);

            System.out.println("dest1.quantity() -> " + dest1.quantity());
        }

        /**
         * EXERCICE 3.1.1
         */
        {
            System.out.println("**************** 3.1.1 ****************");
            Container origin = Container.withCapacity(0);
            Container dest1 = Container.withCapacity(3);

            origin.fillToTheMax();
            ContainerHelpers.transfer(origin, dest1);

            System.out.println("dest1.quantity() -> " + dest1.quantity());
        }

        /**
         * EXERCICE 3.1.1
         */
        {
            System.out.println("**************** 3.1.1 ****************");
            Container origin = Container.withCapacity(100);
            Container dest1 = Container.withCapacity(10);
            Container dest2 = Container.withCapacity(40);
            Container dest3 = Container.withCapacity(30);
            Container dest4 = Container.withCapacity(50);

            origin.fillToTheMax();
            ContainerHelpers.transfer(origin, dest1, dest2, dest3, dest4);

            System.out.println("dest1.quantity() -> " + dest1.quantity());
            System.out.println("dest2.quantity() -> " + dest2.quantity());
            System.out.println("dest3.quantity() -> " + dest3.quantity());
            System.out.println("dest4.quantity() -> " + dest4.quantity());
        }

        /**
         * EXERCICE 3.1.2
         */
        {
            System.out.println("**************** 3.1.2 ****************");
            Container origin = Container.withCapacity(10);
            origin.fillToTheMax();
            Container dest1 = new Container(5);
            dest1.fillWith(2);
            Container dest2 = Container.withCapacity(3);
            Container dest3 = Container.withCapacity(10);
            origin.fillTo(dest1, dest2, dest3);
            System.out.println("dest1.isFull() -> " + dest1.isFull());
            System.out.println("dest2.isFull() -> " + dest2.isFull());
            System.out.println("dest2.remaining() -> " + (dest2.remaining() == 0));
            dest2.remove(2);
            System.out.println("dest2.remaining() == 2 -> " + (dest2.remaining() == 2));
            System.out.println("!dest3.isFull() -> " + !dest3.isFull());
            System.out.println("dest3.quantity() == 4 -> " + (dest3.quantity() == 4));
            dest3.flush();
            System.out.println("dest3.quantity() == 0 -> " + (dest3.quantity() == 0));
        }
    }
}

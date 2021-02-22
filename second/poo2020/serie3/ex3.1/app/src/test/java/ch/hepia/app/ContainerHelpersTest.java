package ch.hepia.app;

import static org.junit.Assert.assertTrue;

import org.junit.Test;

public class ContainerHelpersTest {
    @Test
    public void Transfer_1() {
        Container origin = Container.withCapacity(10);
        Container dest1 = Container.withCapacity(3);
        Container dest2 = Container.withCapacity(3);
        Container dest3 = Container.withCapacity(3);

        origin.fillToTheMax();
        ContainerHelpers.transfer(origin, dest1, dest2, dest3);

        assertTrue(dest1.quantity() == 3);
        assertTrue(dest2.quantity() == 3);
        assertTrue(dest3.quantity() == 3);
    }

    @Test
    public void Transfer_2() {
        Container origin = Container.withCapacity(10);
        Container dest1 = Container.withCapacity(3);
        Container dest2 = Container.withCapacity(3);
        Container dest3 = Container.withCapacity(3);
        Container dest4 = Container.withCapacity(3);

        origin.fillToTheMax();
        ContainerHelpers.transfer(origin, dest1, dest2, dest3, dest4);

        assertTrue(dest1.quantity() == 3);
        assertTrue(dest2.quantity() == 3);
        assertTrue(dest3.quantity() == 3);
        assertTrue(dest4.quantity() == 1);
    }

    @Test
    public void Transfer_3() {
        Container origin = Container.withCapacity(5);
        Container dest1 = Container.withCapacity(3);

        origin.fillToTheMax();
        ContainerHelpers.transfer(origin, dest1);

        assertTrue(dest1.quantity() == 3);
    }

    @Test
    public void Transfer_4() {
        Container origin = Container.withCapacity(0);
        Container dest1 = Container.withCapacity(3);

        origin.fillToTheMax();
        ContainerHelpers.transfer(origin, dest1);

        assertTrue(dest1.quantity() == 0);
    }

    @Test
    public void Transfer_5() {
        Container origin = Container.withCapacity(100);
        Container dest1 = Container.withCapacity(10);
        Container dest2 = Container.withCapacity(40);
        Container dest3 = Container.withCapacity(30);
        Container dest4 = Container.withCapacity(50);

        origin.fillToTheMax();
        ContainerHelpers.transfer(origin, dest1, dest2, dest3, dest4);

        assertTrue(dest1.quantity() == 10);
        assertTrue(dest2.quantity() == 40);
        assertTrue(dest3.quantity() == 30);
        assertTrue(dest4.quantity() == 20);
    }
}

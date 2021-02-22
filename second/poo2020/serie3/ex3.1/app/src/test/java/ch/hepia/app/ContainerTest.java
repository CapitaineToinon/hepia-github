package ch.hepia.app;

import static org.junit.Assert.assertTrue;

import org.junit.Test;

/**
 * Unit test for simple App.
 */
public class ContainerTest {
    @Test
    public void ContainerTest_1() {
        Container origin = Container.withCapacity(10);
        origin.fillToTheMax();
        Container dest1 = new Container(5);
        dest1.fillWith(2);
        Container dest2 = Container.withCapacity(3);
        Container dest3 = Container.withCapacity(10);
        origin.fillTo(dest1, dest2, dest3);

        assertTrue(dest1.isFull());
        assertTrue(dest2.isFull());
        assertTrue(dest2.remaining() == 0);

        dest2.remove(2);
        assertTrue(dest2.remaining() == 2);
        assertTrue(!dest3.isFull());
        assertTrue(dest3.quantity() == 4);

        dest3.flush();
        assertTrue(dest3.quantity() == 0);
    }
}

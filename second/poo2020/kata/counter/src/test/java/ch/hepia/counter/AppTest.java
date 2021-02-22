package ch.hepia.counter;

import static org.junit.Assert.assertTrue;

import org.junit.Test;

/**
 * Unit test for simple App.
 */
public class AppTest {
    /**
     * Rigorous Test :-)
     */
    @Test
    public void shouldAnswerWithTrue() {
        Counter counter = new Counter();
        assertTrue(counter.value() == 0);
        counter.click();
        counter.click();
        counter.click();
        assertTrue(counter.value() == 3);
        counter.init();
        assertTrue(counter.value() == 0);
        while (counter.value() < 9999) {
            counter.click();
        }
        assertTrue(counter.value() == 9999);
        counter.click();
        assertTrue(counter.value() == 0);
    }
}

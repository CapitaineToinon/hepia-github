public class Conversion {
    public static void g(int n, float x) {
        // nothing
    }

    public static void h(short s) {
        // nothing
    }

    public static void main(String[] args) {
        // Exercice 1.2
        {
            short s = 10;
            // byte b = s; <- doesn't work, cannot convert numbers to byte implicitly
            int i = s;
            long l = s;
        }

        // Exercice 1.3
        {
            short s = 10;
            // both doesn't work because java isn't able to do math with numbers
            // smaller than int so it converts the result to int, failing the assignement
            // because s is a short which is smaller than int

            // s = s + s;
            // s = s * 2;
        }

        // Exercice 1.4
        {
            short s = Short.MAX_VALUE; // 32767
            int i = s++;
            System.out.println(i); // 32767 because the ++ is executed after the assignement so i isn't affected
        }

        // Exercice 1.4
        {
            short s = Short.MAX_VALUE; // 32767
            int i = ++s;
            System.out.println(i); // -32768 because the ++ is executed before the assignement, producing an
                                   // underflow and affecting i
        }

        // Exercice 1.5
        {
            int i = 0;
            byte b = 'a';
            float f = 0;
            double d = 1.0;

            g(i, f);
            g(b + 1, f);
            g(b, f);
            // g(i, d); cannot convert double to float
            g(i, i);
            g(i, 2 * f);
            // g(i, 2.0 * f); even if f is float, multiplying it by 2.0 converts it todouble

            h(b);
            // h(b + 1); doing +1 converts it to int
            // h(5); cannot convert int to byte
            // h(5.0); cannot convert double to byte
        }
    }
}

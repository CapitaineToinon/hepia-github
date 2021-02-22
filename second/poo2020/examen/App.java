public class App {
    public static void main(String[] args) {
        {
            TwoOptions<String> o = new TwoOptions<>("22", "v'là les flics");

            if (o.isSecondValuePresent()) {
                String one = o.firstValue(); // "22"
                String two = o.secondValue(); // "v'là les flics"
                o.apply(v -> System.out.println(v));
            }
        }

        {
            TwoOptions<String> o = new TwoOptions<>("22");

            if (o.isSecondValuePresent()) {
                String one = o.firstValue(); // "22"
                String two = o.secondValue(); // "v'là les flics"
                System.out.println(one);
                System.out.println(two);
            }
        }

        {
            TwoOptions<Integer> o = new TwoOptions<>(5, 10);

            if (o.isSecondValuePresent()) {
                Integer one = o.firstValue(); // "22"
                Integer two = o.secondValue(); // "v'là les flics"
                o.apply((Number n) -> System.out.println(n));
            }
        }

        {
            ClassRoom<? extends Academic> cr = new ClassRoom<Professor>(new Professor());
            Professor p = cr.getRandomly();
            Academic a = cr.getRandomly();
            cr.add(new Academic());
            ClassRoom<? super Person> cr = new ClassRoom<String>("Hi guys !");
        }

        {
            Task t1 = new FileTask("/tmp/bigfile.csv");
            Task t2 = new FileTask("/tmp/bigfile2.csv");
            t1.run();
            System.out.println("Exec time for t1: " + t1.durationInMinutes());
            t2.run();
            if (t1.isSuccess() && t2.isSuccess()) {
                Result res = Result.merge(t1, t2, "result_id_1");
                res.save("/tmp/test.pdf", () -> System.out.println("result saved !"));
            }
            Result.merge(t1, t2, "result_id_2").onFinish((Result r) -> System.out.println(r.resultId()));
        }
    }
}

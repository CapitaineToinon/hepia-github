import java.util.List;
import java.util.ArrayList;
import java.time.LocalDate;

// record Patient(String name, LocalDate birthday, List<Consultation> consults) {
// }

record Consultation(LocalDate date, int duration) {
}

public class App {
    // private static List<Patient> origin = new ArrayList<>();

    public static void main(String[] args) {
        // Patient p1 = new Patient("Marc", LocalDate.of(2000, 10, 10),
        // List.of(new Consultation(LocalDate.now().plusDays(1), 30)));
        // origin.add(p1);

    }
}

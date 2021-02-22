import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

public class Smurf {
    public List<Smurf> friends;
    String name;

    public Smurf(String name) {
        this.friends = new ArrayList<>();
        this.name = name;
    }

    public void addFriend(Smurf friend) {
        this.friends.add(friend);
    }

    public int nbFriends() {
        return this.friends.size();
    }

    @Override
    public int hashCode() {
        return Objects.hash(this.name, this.friends);
    }

    @Override
    public boolean equals(Object o) {
        if (this == o)
            return true;

        if (o == null || o.getClass() != this.getClass())
            return false;

        Smurf that = (Smurf) o;
        return this.friends.equals(that.friends) && this.name.equals(that.name);
    }

    @Override
    public String toString() {
        return String.format("Smurf(%s)", this.name.toUpperCase());
    }
}
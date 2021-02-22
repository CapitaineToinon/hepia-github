import java.util.Objects;

class Article {
    private String name;
    private int quantity;
    private double price;
    private Category category;

    private enum Category {
        
    }

    // A compléter
    @Override
    public boolean equals(Object o) {
        if (this == o)
            return true;

        if (o == null || o.getClass() != this.getClass())
            return false;

        Article that = (Article) o;
        return this.name.equals(that.name) && this.category.equals(this.category);
    }
}
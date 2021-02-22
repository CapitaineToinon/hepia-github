/**************************************************************************
 * Test final - groupe soir
 * @author: Sutter Antoine
 *
 * ~ Travail individuel (50% de la note finale) ~
 *
 * Remarques
 * - Un oral peut valider votre note finale
 * - le nombre de points total est de 50
 * - 5 points de pénalités sont déduites pour chaque violation d'une consigne
 *
 * Avertissement
 * - La fraude, participation ou la tentative de fraude, de même que le 
 *   plagiat peut entraîner l'échec du module et peut faire l'objet de 
 *   sanctions disciplinaires.
 *
 * Consignes importantes (5 points de pénalités par contraintes)
 * 1. restez connecté sur Discord durant toute la durée du test
 * 2. indiquez votre nom suivi de votre prénom à la troisième ligne
 * 3. renommez ce fichier "examen-b-nom.java" où nom est votre nom de famille. 
 *    s'il est composé, utilisez des tirets et non des espaces.
 * 4. répondez directement sur ce document dans les blocs "à compléter"
 * 5. réalisez le maximum de commit 
 *   - environ un commit par fonctionnalité. Minimum de 5-6 commits pour les
 *     exercices les plus longs, 1-2 pour les plus courts.
 * 6. déposez ce document dans votre repo poo2020exercices/examen/examen-b-nom.java
 *   - dès que ce document est déposé, réalisé votre premier commit/push 
 *     appelé "version initiale" et vérifiez rapidement le succès de l'opération.
 *   - votre dernier commit doit s'appeler "version finale". Une fois poussé, 
 *     aucun autre commit ne sera pris en compte !
 * 
 * Documentation
 * 
 * Exceptions connues:
 * `IOException` (`extends Exception`), `RuntimeException`, `NullPointerException`, 
 * `IndexOutOfBoundsException`, `ClassCastException`, `ArithmeticException`, 
 * `IllegalStateException`, `NoSuchElementException`, 
 * `UnsupportedOperationException`, `TypeNotPresentException`, `EventException`
 * 
 * Signature de Lambdas/interfaces fonctionnelles connues:
 * | java.util.function | signature      | utilisation         |
 * |:-------------------|:---------------|:--------------------|
 * | `Function<T,R>`    | `T -> R`       | `R apply(T t)`      |
 * | `Consumer<T>`      | `T -> ()`      | `void accept(T t)`  |
 * | `Supplier<T>`      | `() -> T`      | `T get()`           |
 * | `Predicate<T>`     | `T -> boolean` | `boolean test(T t)` |
 * | `Runnable`         | `() -> ()`     | `void run()`        |
 * ************************************************************************/


/* ************************************************************************
 * Ex 1 - (5 pts)
 *
 * L'extrait de la classe `Article`  ci-dessous représente un article 
 * stocké dans un inventaire. Redéfinissez la méthode `equals()` 
 * uniquement, pour permettre de comparer l'égalité entre deux articles. 
 * Deux articles sont égaux s'ils ont le même nom et la même catégorie. 
 * Le type `Category` est une `enum`. 
 * ************************************************************************/

// à compléter (ex1) -------------------------------------------------------
class Article {
  private String name;
  private int quantity;
  private double price;
  private Category category;

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
// -------------------------------------------------------------------------

/* ************************************************************************
 * Ex 2 - (5 pts)
 *
 * Ecrivez l'interface fonctionnelle `Merger` permettant d'utiliser
 * la méthode `merge` de `Manager`. 
 *
 * La dernière ligne de l'extrait doit compiler !
 * ************************************************************************/

// extrait -----------------------------------------------------------------
class Manager {
  private Account a1;
  private Account a2;
  public String merge(Merger m) {
    return m.fusion(a1, a2);
  }
}
...
Manager oneManager = ...;
String s = oneManager.merge( (Account acc1, Account acc2) -> acc1.owner() + acc2.owner() );
// -------------------------------------------------------------------------

// à compléter (ex2) -------------------------------------------------------
@FunctionalInterface 
interface Merger {
    public String fusion(Account a1, Account a2);
}
// -------------------------------------------------------------------------

/* ************************************************************************
 * Ex 3 - (15 pts)
 * 
 * Nous avons vu dans le cours deux types de containers. Les collections 
 * qui peuvent contenir **0 à n** éléments de même type et l'`Optional` 
 * qui peut contenir **0 ou 1** élément. Dans cet exercice, vous allez 
 * créer un container pouvant contenir **0 à 2** éléments de même type 
 * que nous nommerons `TwoOptions`.
 * 
 * Vous devez **impérativement** respecter les consignes suivantes:
 * 
 * - `TwoOptions<T>` est une **classe** générique qui prend un paramètre `T`
 * - La mise en oeuvre doit se faire avec une `List` **uniquement**. Donc 
 *   seul un champ de type `List` **est autorisé !**
 * - Elle est immutable, une fois l'objet créé, il est impossible de modifier, 
 *   d'ajouter ou de supprimer les éléments
 * - Trois constructeurs doivent être proposés pour construire le container 
 *   selon le nombre d'éléments
 *   - construction d'un container vide,
 *   - construction d'un container d'un élément,
 *   - construction d'un containter de deux éléments.
 * - Les méthodes suivantes doivent être offertes:
 *   - `isEmpty` qui précise si le container est vide
 *   - `isFirstValuePresent` qui précise si le container contient au moins une valeur 
 *   - `isSecondValuePresent` qui précise si le container contient deux valeurs
 *   - `firstValue` qui retourne la première valeur si elle existe ou lève une 
 *     (*unchecked*) exception quelconque si aucune valeur ne peut être retournée
 *   - `secondValue` qui retourne la seconde valeur si elle existe ou lève une 
 *     (*unchecked*) exception quelconque si aucune valeur ne peut être retournée
 *   - `apply` qui prend en argument une action qui est appliquée sur chaque 
 *     élément existant
 *     - par exemple: `twoOpt.apply( v -> System.out.println(v.toString()) );`
 *   - appliquez la variance où cela est nécessaire.
 * - extrait d'utilisation:
 * ************************************************************************/

// extrait -----------------------------------------------------------------
TwoOptions<String> o = new TwoOptions<>("22", "v'là les flics");
if (o.isSecondValuePresent()) {
  String one = o.firstValue(); // "22"
  String two = o.secondValue(); // "v'là les flics"
}
// -------------------------------------------------------------------------

// à compléter (ex3) -------------------------------------------------------
// complétez le code et les ...
import java.util.ArrayList;
import java.util.List;
import java.util.function.Consumer;

public class TwoOptions<T> {
    private List<T> options = new ArrayList<T>(2);

    public TwoOptions() {

    }

    public TwoOptions(T first) {
        this.options.add(0, first);
    }

    public TwoOptions(T first, T second) {
        this.options.add(0, first);
        this.options.add(1, second);
    }

    public boolean isEmpty() {
        return this.options.isEmpty();
    }

    public boolean isFirstValuePresent() {
        return this.options.size() >= 1;
    }

    public boolean isSecondValuePresent() {
        return this.options.size() >= 2;
    }

    public T firstValue() {
        if (!isFirstValuePresent())
            throw new RuntimeException("First value is not present.");

        return this.options.get(0);
    }

    public T secondValue() {
        if (!isSecondValuePresent())
            throw new RuntimeException("Second value is not present.");

        return this.options.get(1);
    }

    public void apply(Consumer<? super T> consumer) {
        this.options.forEach(consumer);
    }
}
// -------------------------------------------------------------------------

/* ************************************************************************
 * Ex 4 - (15 pts)
 *
 * Réalisez le design **minimaliste** pour que le code 
 * ci-dessous **compile**. Ne rendez publiques que les fonctionnalités 
 * visibles ci-dessous. Inutile de redéfinir les méthodes `toString`, 
 * `hashCode` ou `equals`.
 *
 * Appliquez la variance où cela est nécessaire.
 * ************************************************************************/

// extrait -----------------------------------------------------------------
Task t1 = new FileTask("/tmp/bigfile.csv");
Task t2 = new FileTask("/tmp/bigfile2.csv");
t1.run();
System.out.println("Exec time for t1: " + t1.durationInMinutes());
t2.run();
if (t1.isSuccess() && t2.isSuccess()) {
  Result res = Result.merge(t1, t2, "result_id_1");
  res.save("/tmp/test.pdf", () -> System.out.println("result saved !"));
}
Result.merge(t1, t2, "result_id_2").onFinish( (Result r) -> System.out.println( r.resultId() ) );
// -------------------------------------------------------------------------

// à compléter (ex4) -------------------------------------------------------
public interface Task {
    public void run();
    public int durationInMinutes();
    public boolean isSuccess();
}

public class FileTask implements Task {
    public FileTask(String path) { }

    @Override
    public void run() { }

    @Override
    public int durationInMinutes() {
        return 0;
    }

    @Override
    public boolean isSuccess() {
        return false;
    }
}

import java.util.UUID;
import java.util.function.Consumer;

public class Result {
    private Result() {

    }

    public UUID resultId() {
        return UUID.randomUUID();
    }

    public static Result merge(Task t1, Task t2, String param) {
        return new Result();
    }

    public void save(String path, Runnable finished) { }
    public void onFinish(Consumer<Result> consumer) { }
}

// -------------------------------------------------------------------------

/* ************************************************************************
 * Ex 5 - (10 pts)
 * 
 * Indiquez quelle(s) ligne(s) compile(nt) 
 *  - oui = compile/ne pose aucun problème
 *  - non = pose un problème de compilation. Dans ce cas, expliquez pourquoi
 * ************************************************************************/

// extrait -----------------------------------------------------------------
/*      Person
 *        |    
 *     Academic 
 *     /      \ 
 * Professor   Student       */
public class ClassRoom<T extends Person> {
  public ClassRoom(T t) { ... }
  public T getRandomly() { ... }
  public void add(T t) { ... }
}
...

/*[ligne 1]*/ ClassRoom<? extends Academic> cr = new ClassRoom<Professor>( new Professor() );
/*[ligne 2]*/ Professor p = cr.getRandomly();
/*[ligne 3]*/ Academic a = cr.getRandomly();
/*[ligne 4]*/ cr.add( new Student() );
/*[ligne 5]*/ ClassRoom<? super Person> cr = new ClassRoom<String>( "Hi guys !" );
// -------------------------------------------------------------------------

// à compléter (ex5) -------------------------------------------------------
// remplacez les ... par votre réponse
- ligne1 [oui/non et pourquoi]
Oui
- ligne2 [oui/non et pourquoi]
Non parce que getRandomly retourne du Academic alors que le type Professor est une sous class de Academic.
Le code aurait donc pu soit être :
Academic a = cr.getRandomly();

ou alors :
Person a = cr.getRandomly();
- ligne3 [oui/non et pourquoi]
Oui
- ligne4 [oui/non et pourquoi]
Non, ne compile pas ! cela corromprait l'objet référencé, celui-ci ne peut contenir que des Professor !
- ligne5 [oui/non et pourquoi]
Non parce que le type String n'est pas un super type de Person
(et aussi parce que la variable cr a déjà été definie à la ligne 1)
// -------------------------------------------------------------------------

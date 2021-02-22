import java.util.List;

public class App {
    public static void main(String[] args) {
        System.out.println("***************** Exercice 1 *****************");
        Ex1();
        System.out.println("***************** Exercice 3 *****************");
        Ex3();
        System.out.println("***************** Exercice 4 *****************");
        Ex4();
    }

    /*
     * ************************************************************************ Ex 1
     * - (15 pts) La classe `Smurf` comporte plusieurs petites erreurs ou mauvaises
     * pratiques (env. cinq) 1. corrigez-les 2. complétez ensuite le code pour que
     * l'extrait ci-dessous puisse fonctionner correctement.
     ************************************************************************/
    public static void Ex1() {
        Smurf s1 = new Smurf("Schtroumpfette");
        System.out.println(s1); // ==> affiche: Smurf@4b9af9a9
                                // TODO: devrait affichier: Smurf(SCHTROUMPFETTE)
        Smurf s2 = new Smurf("Grognon");
        List<Smurf> smurfs = List.of(s1, s2);
        System.out.println(smurfs.contains(new Smurf("Grognon"))); // ==> false !!!
        // TODO: aurait du retourner true
    }

    /*
     * ************************************************************************ Ex 3
     * - (30 pts) Vous devez réaliser une structure de données qui représente le
     * chargement d'un convoi ferroviaire. A chaque chargement, calculé en tonnage,
     * nous cherchons un wagon qui peut contenir celui-ci. Si aucun wagon ne peut
     * accepter le chargement, un nouveau wagon est ajouté au convoi.
     *
     * Lors de la création d'un convoi, représenter par un train, nous informons la
     * capacité maximale de chaque wagon. Réalisez la classe `Train` pour que le
     * code ci-dessous s'exécute correctement.
     *
     * N'exposez publiquement que les fonctionnalités visibles dans l'extrait du
     * code ci-dessous. Rien de plus !! En outre, inutile de redéfinir les méthodes
     * de comparaison.
     *
     * A côté de chaque instruction, l'état du train avec ses wagons est représenté
     * à titre indicatif. Ex: Train([2,2,1]) indique que le convoi à trois wagons
     * avec des chargements respectifs de 2 tonnes, 2 tonnes et 1 tonne.
     ************************************************************************/
    public static void Ex3() {
        Train train = Train.withCapacity(10); // Train([])

        // Ajoute du chargement
        train.insert(3); // Train([3])
        train.insert(3); // Train([6])
        train.insert(3); // Train([9])
        train.insert(3); // Train([9, 3]) ; plus de place dans le premier wagon, ajout d'un second
        train.insert(2); // Train([9, 5])
        train.insert(1); // Train([10, 5])
        train.insert(8); // Train([10, 5, 8])

        System.out.println(train); // affiche: Train([10, 5, 8])
        System.out.println(train.nbWagons()); // nb = 3
        System.out.println(train.totalWeight()); // totalWeight = 23

        try {
            train.insert(15); // lève une exception ! 15 est plus grand que la capacité d'un wagon (10)
        } catch (RuntimeException e) {
            System.out.println(e.getMessage());
        }
    }

    /*
     * ************************************************************************ Ex 4
     * - (15 pts) Réalisez le code minimaliste pour que l'extrait ci-dessous
     * fonctionne. Inutile de redéfinir les méthodes de Object (toString, equals...)
     * 
     * Une référence de type MaybeString a deux états. Soit il contient un String et
     * il est possible de le récupérer. Soit il ne contient rien et il ne devrait
     * donc pas être possible de récupérer une valeur.
     ************************************************************************/
    public static void Ex4() {
        MaybeString m1 = new Something("hello");
        MaybeString m2 = new Nothing();
        if (m1.exists()) { // ==> true car m1 référence quelquechose qui contient "hello"
            String greeting = m1.get(); // ==> greeting = "hello"
            System.out.println(greeting);
        } else {
            System.out.println("m1 n'a aucune valeur");
        }
        System.out.println(m2.exists()); // ==> affiche `false`

        try {
            String badCall = m2.get(); // ==> lève une exception ! il n'y a rien à récupérer...
        } catch (RuntimeException e) {
            System.out.println(e.getMessage());
        }
    }
}
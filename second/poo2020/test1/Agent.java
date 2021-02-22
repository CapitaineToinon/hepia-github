import java.util.List;

interface Agent {

    boolean isSleeping();

    // un agent travaille lorsqu'il ne dort pas et qu'il a au moins une tâche
    // à réaliser
    default boolean isWorking() {
        return !isSleeping() && nbTasksToComplete() > 0;
    }

    /*
     * réveil un agent. Lève une exception si l'agent est déjà en train de
     * travailler !
     */
    void wakeUp();

    // retourne le nombre de tâches que l'agent doit encore réaliser
    default int nbTasksToComplete() {
        return currentTasksToComplete().size();
    }

    // retourne la liste des tâches que l'agent doit encore réaliser
    List<Integer> currentTasksToComplete();
}
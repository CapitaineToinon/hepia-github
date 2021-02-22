import java.util.List;

interface AgentManager {

    // retourne tous les agents
    List<Agent> allAgents();

    // réveil tous les agents qui dorment et qui ont une seule tâche à réaliser
    default void wakeUpSleepingAndBusyAgents() {
        for (Agent agent : allAgents())
            if (agent.isSleeping() && agent.nbTasksToComplete() == 1)
                agent.wakeUp();
    }
}
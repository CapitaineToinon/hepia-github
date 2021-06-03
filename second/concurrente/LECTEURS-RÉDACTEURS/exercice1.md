```c
int N = 100;
int food = N;
int readers = 0;
mutex_t mutex;
sem_t available;
sem_t refill;
sem_t refilled;

void cannibal() {
    while (true) {
        lock(mutex);
        readers++;
        if (readers == 1)
            wait(available);
        
        get_serving_from_pot();
        food--;
        unlock(mutex);

        eat();

        if (food == 0) {
            post(refill)
            wait(refilled);
        }

        lock(mutex);
        readers--;
        if (readers == 0)
            post(available);
        unlock(mutex);
    }
}

void chef() {
    while (true) {
        wait(refill);
        lock(mutex);
        put_servings_in_pot(N);
        unlock(mutex);
        post(refilled);
    }
}

int main() {
    mutex = mutex();
    available = sem_init(1);
    refill = sem_init(0);
    refilled = sem_init(0);
}
```
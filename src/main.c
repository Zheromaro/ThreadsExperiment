#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <semaphore.h>

#define TH_NUM 1

pthread_mutex_t mutexFuel;
sem_t semFuel;
int *fuel;

void* routine(void* arg) {
    *fuel += 50;
    printf("Current value is %d\n", *fuel);
    sem_post(&semFuel);
}

int main(int argc, char *argv[]) {
    pthread_t th[TH_NUM];
    fuel = malloc(sizeof(int));
    *fuel = 50;

    pthread_mutex_init(&mutexFuel, NULL);
    sem_init(&semFuel, 0, 0);
    for (int i = 0; i < TH_NUM; i++) {
        if (pthread_create(th + i, NULL, &routine, NULL) != 0)
            perror("failed to create thread!");
    }
    sem_wait(&semFuel);
    free(fuel);
    printf("Deallocated fuel.\n");
    for (int i = 0; i < TH_NUM; i++) {
        if (pthread_join(th[i], NULL) != 0)
            perror("failed to join thread!");
    }
    pthread_mutex_destroy(&mutexFuel);
    sem_destroy(&semFuel);

    return 0;
}

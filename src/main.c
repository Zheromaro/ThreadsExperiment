#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>

#define TH_NUM 8

pthread_mutex_t mutexFuel;
int fuel = 50;

void* routine(void* arg) {
    pthread_mutex_lock(&mutexFuel);
    pthread_mutex_lock(&mutexFuel);
    pthread_mutex_lock(&mutexFuel);
    fuel += 50;
    printf("Incremented fuel to: %d\n", fuel);
    pthread_mutex_unlock(&mutexFuel);
    pthread_mutex_unlock(&mutexFuel);
    pthread_mutex_unlock(&mutexFuel);
}

int main(int argc, char *argv[]) {
    pthread_t th[TH_NUM];
    pthread_mutexattr_t recursiveMutex;

    pthread_mutexattr_init(&recursiveMutex);
    pthread_mutexattr_settype(&recursiveMutex, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&mutexFuel, &recursiveMutex);
    for (int i = 0; i < TH_NUM; i++) {
        if (pthread_create(th + i, NULL, routine, NULL) != 0) {
            perror("failed to create thread!");
        }
    }
    for (int i = 0; i < TH_NUM; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("failed to join thread!");
        }
    }
    printf("Fuel: %d\n", fuel);
    pthread_mutexattr_destroy(&recursiveMutex);
    pthread_mutex_destroy(&mutexFuel);

    return 0;
}

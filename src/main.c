#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

pthread_mutex_t mutexFuel;
pthread_cond_t condFuel;
int fuel = 0;

void* fuel_filling(void* arg) {
    for (int i = 0; i < 5; i++) {
        pthread_mutex_lock(&mutexFuel);
        fuel += 60;
        printf("filling fuel... %d\n", fuel);
        pthread_mutex_unlock(&mutexFuel);
        // pthread_cond_signal()    → wakes ONE arbitrary waiting thread
        // pthread_cond_broadcast() → wakes ALL waiting threads
        pthread_cond_broadcast(&condFuel);
        sleep(1);
    }
}
void* car(void* arg) {
    pthread_mutex_lock(&mutexFuel);
    while (fuel < 40) {
        printf("no fuel. Waiting...\n");
        pthread_cond_wait(&condFuel, &mutexFuel);
    }
    fuel -= 40;
    printf("using fuel. left: %d\n", fuel);
    pthread_mutex_unlock(&mutexFuel);
}

int main(int argc, char *argv[]) {
    int th_num = 6;
    pthread_t th[th_num];

    pthread_mutex_init(&mutexFuel, NULL);
    pthread_cond_init(&condFuel, NULL);
    for (int i = 0; i < th_num; i++) {
        switch (i) {
            case 0:
            case 1:
            case 2:
            case 3:
                if (pthread_create(th + i, NULL, car, NULL) != 0) {
                    perror("failed to create thread!");
                    return 1;
                }
            break;
            case 4:
            case 5:
                if (pthread_create(th + i, NULL, fuel_filling, NULL) != 0) {
                    perror("failed to create thread!");
                    return 1;
                }
            break;
        }
    }
    for (int i = 0; i < th_num; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("failed to join thread!");
            return 2;
        }
    }
    pthread_mutex_destroy(&mutexFuel);
    pthread_cond_destroy(&condFuel);

    return 0;
}

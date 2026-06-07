#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <semaphore.h>

#define TH_NUM 16

sem_t semaphore;

void* routine(void* arg) {
    printf("(%d) Waiting in the login queue\n", *((int*)arg));
    sem_wait(&semaphore);
    printf("(%d) Logged in\n", *((int*)arg));
    sleep(rand() % 5 + 1);
    printf("(%d) Logged out\n", *((int*)arg));
    sem_post(&semaphore);
    free(arg);

    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t th[TH_NUM];

    sem_init(&semaphore, 0, 12);
    for (int i = 0; i < TH_NUM; i++) {
        int* a = malloc(sizeof(int));
        *a = i;
        if (pthread_create(th + i, NULL, &routine, a) != 0) {
            perror("failed to create thread!");
        }
    }
    for (int i = 0; i < TH_NUM; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("failed to join thread!");
        }
    }
    sem_destroy(&semaphore);

    return 0;
}

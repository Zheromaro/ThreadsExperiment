#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <semaphore.h>

#define TH_NUM 4

pthread_mutex_t mutexFuel;
sem_t sem;

void* routine(void* arg) {
    int index = *((int*)arg);
    int semVal;
    sem_wait(&sem);
    sleep(index+1);
    sem_getvalue(&sem, &semVal);
    printf("(%d) Current semaphore value after wait is %d\n", index, semVal);
    sem_post(&sem);
    sem_getvalue(&sem, &semVal);
    printf("(%d) Current semaphore value after post is %d\n", index, semVal);
    free(arg);
}

int main(int argc, char *argv[]) {
    pthread_t th[TH_NUM];

    pthread_mutex_init(&mutexFuel, NULL);
    sem_init(&sem, 0, 4);
    for (int i = 0; i < TH_NUM; i++) {
        int* a = malloc(sizeof(int));
        *a = i;
        if (pthread_create(th + i, NULL, &routine, a) != 0)
            perror("failed to create thread!");
    }
    for (int i = 0; i < TH_NUM; i++) {
        if (pthread_join(th[i], NULL) != 0)
            perror("failed to join thread!");
    }
    pthread_mutex_destroy(&mutexFuel);
    sem_destroy(&sem);

    return 0;
}

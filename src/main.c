#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <semaphore.h>

#define TH_NUM 8

pthread_mutex_t mutexBuffer;
sem_t semEmpty;
sem_t semFull;
int buffer[10];
int count = 0;

void* producer(void* arg) {
    while (1) {
        // Produce
        int x = rand() % 100;

        // Add to the buffer7
        sem_wait(&semEmpty);
        pthread_mutex_lock(&mutexBuffer);
        buffer[count] = x;
        count++;
        pthread_mutex_unlock(&mutexBuffer);
        sem_post(&semFull);
    }
}
void* consumer(void* arg) {
    while (1) {
        // Remove from the buffer
        sem_wait(&semFull);
        pthread_mutex_lock(&mutexBuffer);
        int y = buffer[count - 1];
        count--;
        pthread_mutex_unlock(&mutexBuffer);
        sem_post(&semEmpty);

        // Consume
        printf("Got %d\n", y);
        sleep(1);
    }
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    pthread_t th[TH_NUM];

    pthread_mutex_init(&mutexBuffer, NULL);
    sem_init(&semEmpty, 0, 10);
    sem_init(&semFull, 0, 0);
    for (int i = 0; i < TH_NUM; i++) {
        if (i % 2 == 0) {
            if (pthread_create(th + i, NULL, &producer, NULL) != 0)
                perror("failed to create thread!");
        }
        else
        {
            if (pthread_create(th + i, NULL, &consumer, NULL) != 0)
                perror("failed to create thread!");
        }
    }
    for (int i = 0; i < TH_NUM; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("failed to join thread!");
        }
    }
    sem_destroy(&semEmpty);
    sem_destroy(&semFull);
    pthread_mutex_destroy(&mutexBuffer);

    return 0;
}

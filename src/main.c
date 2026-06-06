#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>

#define TH_NUM 8

pthread_barrier_t barrierRolledDice;
pthread_barrier_t barrierCalculated;
int diceValues[8];
int status[8] = {0};

void* rollDice(void* arg) {
    int index = *((int*)arg);
    diceValues[index] = rand() % 6 + 1;
    pthread_barrier_wait(&barrierRolledDice);
    pthread_barrier_wait(&barrierCalculated);
    if (status[index] == 1) {
        printf("(%d rolled %d)I won\n", index, diceValues[index]);
    }
    else
    {
        printf("(%d rolled %d)I lost\n", index, diceValues[index]);
    }
    free(arg);
}

int main(int argc, char *argv[]) {
    pthread_t th[TH_NUM];
    int max = 0;

    pthread_barrier_init(&barrierRolledDice, NULL, TH_NUM + 1);
    pthread_barrier_init(&barrierCalculated, NULL, TH_NUM + 1);
    for (int i = 0; i < TH_NUM; i++) {
        int* a = malloc(sizeof(int));
        *a = i;
        if (pthread_create(th + i, NULL, rollDice, a) != 0) {
            perror("failed to create thread!");
            return 1;
        }
    }
    pthread_barrier_wait(&barrierRolledDice);
    for (int i = 0; i < TH_NUM; i++) {
        if (diceValues[i] > max) max = diceValues[i];
    }
    for (int i = 0; i < TH_NUM; i++) {
        if (diceValues[i] == max) {
             status[i] = 1;
        }
        else {
            status[i] = 0;
        }
    }
    pthread_barrier_wait(&barrierCalculated);
    for (int i = 0; i < TH_NUM; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("failed to join thread!");
            return 2;
        }
    }
    pthread_barrier_destroy(&barrierRolledDice);
    pthread_barrier_destroy(&barrierCalculated);

    return 0;
}

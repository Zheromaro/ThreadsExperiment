#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>

#define TH_NUM 4

typedef struct Task {
    void (*taskFunction)(int, int);
    int arg1, arg2;
} Task;
Task taskQueue[256];
int taskCount = 0;

pthread_mutex_t mutexQueue;
pthread_cond_t condQueue;

void sum(int a, int b) {
    usleep(50000);
    int sum = a + b;
    printf("the sum of %d and %d is %d\n", a, b, sum);
}
void product(int a, int b) {
    usleep(50000);
    int prod = a + b;
    printf("the product of %d and %d is %d\n", a, b, prod);
}

void executeTask(Task* task) {
    task->taskFunction(task->arg1, task->arg2);
}

void submitTask(Task task) {
    pthread_mutex_lock(&mutexQueue);
    taskQueue[taskCount] = task;
    taskCount++;
    pthread_mutex_unlock(&mutexQueue);
    pthread_cond_signal(&condQueue);
}

void* startTread(void *arg) {
    Task task;
    int found = 0;
    while (1) {
        pthread_mutex_lock(&mutexQueue);
        while (taskCount == 0) {
            pthread_cond_wait(&condQueue, &mutexQueue);
        }

        task = taskQueue[0];
        for (int i = 0; i < taskCount-1; i++)
            taskQueue[i] = taskQueue[i+1];
        taskCount--;
        pthread_mutex_unlock(&mutexQueue);
        executeTask(&task);
    }
}


int main(int argc, char *argv[]) {
    pthread_t th[TH_NUM];

    pthread_mutex_init(&mutexQueue, NULL);
    pthread_cond_init(&condQueue, NULL);
    for (int i = 0; i < TH_NUM; i++) {
        int* a = malloc(sizeof(int));
        *a = i;
        if (pthread_create(th + i, NULL, &startTread, a) != 0)
            perror("failed to create thread!");
    }

    srand(time(NULL));
    for (int i = 0; i < 100; i++) {
        Task t = {
            .taskFunction = (i % 2 == 0) ? &sum : &product,
            .arg1 = rand() % 100,
            .arg2 = rand() % 100
        };
        submitTask(t);
    }

    for (int i = 0; i < TH_NUM; i++) {
        if (pthread_join(th[i], NULL) != 0)
            perror("failed to join thread!");
    }
    pthread_mutex_destroy(&mutexQueue);
    pthread_cond_destroy(&condQueue);

    return 0;
}

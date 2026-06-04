#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

pthread_mutex_t mutex;

void* routine(void* arg) {
    if (pthread_mutex_trylock(&mutex) == 0) {
        printf("Got locked\n");
        sleep(1);
        pthread_mutex_unlock(&mutex);
    } else {
        printf("Didn't get locked\n");
    }
}

int main(int argc, char *argv[]) {
    int th_num = 4;
    pthread_t th[th_num];

    pthread_mutex_init(&mutex, NULL);
    for (int i = 0; i < th_num; i++) {
        if (pthread_create(th + i, NULL, routine, NULL) != 0) {
            perror("failed to create thread!");
            return 1;
        }
    }
    for (int i = 0; i < th_num; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("failed to join thread!");
            return 2;
        }
    }
    pthread_mutex_destroy(&mutex);

    return 0;
}

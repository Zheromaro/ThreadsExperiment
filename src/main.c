#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>

#define TH_NUM 10
#define MU_NUM 4

pthread_mutex_t stoveMutexes[MU_NUM];
int stoveFuel[4] = {100, 100, 100, 100};

void* routine(void* arg) {
    for (int i = 0; i <MU_NUM; i++) {
        if (pthread_mutex_trylock(stoveMutexes + i) == 0) {
            int fuelNeeded = (rand() % 30) + 1;
            if (stoveFuel[i] < fuelNeeded) {
                printf("no more fuel... time to go home\n");
            }
            else
            {
                stoveFuel[i] -= fuelNeeded;
                usleep(500000);
                printf("fuel left %d\n", stoveFuel[i]);
            }
            pthread_mutex_unlock(stoveMutexes + i);
            break;
        }
        else
        {
            if (i == 3) {
                printf("no stove available yet, waiting...\n");
                usleep(300000);
                i = 0;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    pthread_t th[TH_NUM];

    for (int i = 0; i < MU_NUM; i++) {
        pthread_mutex_init(stoveMutexes + i, NULL);
    }
    for (int i = 0; i < TH_NUM; i++) {
        if (pthread_create(th + i, NULL, routine, NULL) != 0) {
            perror("failed to create thread!");
            return 1;
        }
    }
    for (int i = 0; i < TH_NUM; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("failed to join thread!");
            return 2;
        }
    }
    for (int i = 0; i < MU_NUM; i++) {
        pthread_mutex_destroy(stoveMutexes + i);
    }

    return 0;
}

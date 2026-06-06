#include <bits/pthreadtypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>

#define TH_NUM 2

void* routine(void* arg) {
    sleep(1);
    printf("finished execution.\n");
}

int main(int argc, char *argv[]) {
    pthread_t th[TH_NUM];
    pthread_attr_t detachedThread;

    pthread_attr_init(&detachedThread);
    pthread_attr_setdetachstate(&detachedThread, PTHREAD_CREATE_DETACHED);
    for (int i = 0; i < TH_NUM; i++) {
        if (pthread_create(th + i, &detachedThread, routine, NULL) != 0) {
            perror("failed to create thread!");
        }
        // pthread_detach(th[i]); -> set the thread from to a detached state if it not
    }
    // no need to wasting resources for waiting and joining threads
    // pthread_join() is still necessary to get the return of the thread
    pthread_attr_destroy(&detachedThread);

    pthread_exit(0);
}

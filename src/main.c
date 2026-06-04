#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

int primes[10] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29 };

void* routine(void* arg) {
    int *sum = malloc(sizeof(int));
    *sum = 0;
    for (int i = 0; i < 5; i++) {
        *sum += ((int*)arg)[i];
    }
    printf("Thread sum : %d\n", *sum);
    return sum;
}

int main(int argc, char *argv[]) {
    int sum = 0;
    int th_num = 2;
    pthread_t th[th_num];

    for (int i = 0; i < th_num; i++) {
        if (pthread_create(th + i, NULL, routine, primes+(5*i) ) != 0) {
            perror("failed to create thread!");
            return 1;
        }
    }
    for (int i = 0; i < th_num; i++) {
        int* resault;
        if (pthread_join(th[i], (void**)&resault) != 0) {
            perror("failed to join thread!");
            return 2;
        }
        sum += *resault;
    }

    printf("the sum is %d\n", sum);

    return 0;
}

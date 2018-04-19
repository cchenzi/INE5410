#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>



sem_t semaforoA;
sem_t semaforoB;

void *PrintaA(void *arg) {
    while (true) {
      sem_wait(&semaforoA);
      printf("A");fflush(stdout);
      usleep(1000000);
      sem_post(&semaforoB);
    }
}

void *PrintaB(void *arg) {
    while (true) {
      sem_wait(&semaforoB);
      printf("B");fflush(stdout);
      usleep(3000000);
      sem_post(&semaforoA);
    }
}

int main(int argc, char *argv[]) {
    /*if (argc < 2) {
        printf("Uriel tongo@");
        //printf("Use: %s [tamanho do buffer][número de produtores][número de consumidores]\n", argv[0]);
        return 0;
    }*/
    sem_init(&semaforoA, 0, 1);
    sem_init(&semaforoB, 0, 1);

    pthread_t thread_A;
    pthread_t thread_B;

    pthread_create(&thread_A, NULL, PrintaA, NULL);
    pthread_create(&thread_B, NULL, PrintaB, NULL);

    pthread_join(thread_A, NULL);
    pthread_join(thread_B, NULL);

    sem_destroy(&semaforoA);
    sem_destroy(&semaforoB);


    return 0;
}

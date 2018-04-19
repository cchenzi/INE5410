#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>

int contador_global = 0;

void *ThreadFunc(void *arg) {
    int numOfLoops = *(int *)arg;
    int i;
    for (i = 0; i < numOfLoops; i++) {
        contador_global += 1;
    }
    pthread_exit(NULL);
}

void main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Informe o número de threads\n");
        return;
    }

    int numOfThreads = atoi(argv[1]);
    int numOfLoops = 1000 * numOfThreads;
    pthread_t threads[numOfThreads];
    int i;
    for (i = 0; i < numOfThreads; i++)
        pthread_create(&threads[i], NULL, ThreadFunc, (void*)&numOfLoops);

    for (i = 0; i < numOfThreads; i++)
        pthread_join(threads[i], NULL);
    
    printf("Processo principal finalizado.\n");
    printf("Contador: %d\n", contador_global);
    printf("Esperado: %d\n", numOfThreads * numOfLoops);
}
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

int produzir(int *seed);
void consumir(int produto);
void *ProdutorFunc(void *arg);
void *ConsumidorFunc(void *arg);

int indice_produtor, indice_consumidor, tamanho_buffer, tamanho_produtores, tamanho_consumidores;
int* buffer;

sem_t sem_produtor;
sem_t sem_consumidor;
pthread_mutex_t mutex_produtor;
pthread_mutex_t mutex_consumidor;

//Você deve fazer as alterações necessárias nesta função e na função
//ConsumidorFunc para que usem semáforos para coordenar a produção
//e consumo de elementos do buffer.
void *ProdutorFunc(void *arg) {
    int seed = time(NULL); //Seed para o gerador de números aleatórios

    while (true) {
        sem_wait(&sem_produtor);
        int produto = produzir(&seed); //produz um elemento usando o seed do parâmetro
        pthread_mutex_lock(&mutex_produtor);
        indice_produtor = (indice_produtor + 1) % tamanho_buffer; //calcula o próximo elemento
        buffer[indice_produtor] = produto; //adiciona o elemento produzido à lista
        printf("Produziu no indice %d\n", indice_produtor);
        pthread_mutex_unlock(&mutex_produtor);
        sem_post(&sem_consumidor);
    }
}

void *ConsumidorFunc(void *arg) {
    while (true) {
        sem_wait(&sem_consumidor);
        int produto;
        pthread_mutex_lock(&mutex_consumidor);
        indice_consumidor = (indice_consumidor + 1) % tamanho_buffer; //Calcula o próximo item a consumir
        produto = buffer[indice_consumidor]; //obtém o item da lista
        pthread_mutex_unlock(&mutex_consumidor);
        sem_post(&sem_produtor);
        consumir(produto); //Consome o item obtido.
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Use: %s [tamanho do buffer][número de produtores][número de consumidores]\n", argv[0]);
        return 0;
    }

    tamanho_buffer = atoi(argv[1]);
    tamanho_produtores = atoi(argv[2]);
    tamanho_consumidores = atoi(argv[3]);
    printf("Prod: %d\nConsum: %d\n", tamanho_produtores, tamanho_consumidores);
    indice_produtor = 0;
    indice_consumidor = 0;

    //Iniciando buffer
    buffer = malloc(sizeof(int) * tamanho_buffer);
    //Aqui você deve criar as threads e semáforos necessários
    //para que o código funcione adequadamente.
    sem_init(&sem_produtor, 0, tamanho_buffer);
    sem_init(&sem_consumidor, 0, 0);

    pthread_t threads_consumidor[tamanho_consumidores];
    pthread_t threads_produtor[tamanho_produtores];

    pthread_mutex_init(&mutex_produtor, NULL);
    pthread_mutex_init(&mutex_consumidor, NULL);

    int i;
    for (i = 0; i < tamanho_produtores; i++){
        pthread_create(&threads_produtor[i], NULL, ProdutorFunc, NULL);
    }
    for (i = 0; i < tamanho_consumidores; i++){
        pthread_create(&threads_consumidor[i], NULL, ConsumidorFunc, NULL);
    }
    for (i = 0; i < tamanho_produtores; i++){
        pthread_join(threads_produtor[i], NULL);
    }
    for (i = 0; i < tamanho_consumidores; i++){
        pthread_join(threads_consumidor[i], NULL);
    }

    sem_destroy(&sem_consumidor);
    sem_destroy(&sem_produtor);
    pthread_mutex_destroy(&mutex_produtor);
    pthread_mutex_destroy(&mutex_consumidor);

    // Libera memória do buffer
    free(buffer);

    return 0;
}

//Esta função produz um elemento. O argumento seed é usado para gerar
//um número aleatório, e é necessário por que a função rand() não
//é thread-safe e tem comportamento indefinido se usada por várias
//threads ao mesmo tempo.
int produzir(int *seed) {
    int produto = 500 + (rand_r(seed) % 500);
    printf("Produzindo %d\n", produto);
    usleep(produto * 1000);
    return produto;
}

//Esta função consome um elemento.
void consumir(int produto) {
    printf("Consumindo %d\n", produto);
    usleep(produto * 1000);
}

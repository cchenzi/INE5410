#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>

struct params {
  int id;
  int inicio;
  int fim;
};

double *a, *b, *resultado_thread;

void *thread(void *arg) {
  struct params param = *(struct params *)arg;

  /* produto escalar dos vetores */
  for (int i=param.inicio; i<param.fim; i++)
    resultado_thread[param.id] += a[i] * b[i];

  pthread_exit(NULL);
}

int main (int argc, char *argv[])
{
  int tam_vet;
  int i;
  double prod = 0.0;

  if(argc<3){
    printf("uso %s <tamanho vetores> <quantidade threads>\n", argv[0]);
    exit(1);
  }

  /* tamanho dos vetores */
  tam_vet = atoi(argv[1]);

  /* numero de threads */
  int num_threads = atoi(argv[2]);

  /* garante a criacao de no maximo "tam_vet" threads */
  if(num_threads > tam_vet)
    num_threads = tam_vet;

  pthread_t threads[num_threads];
  struct params parametros[num_threads];
  int inicio_thread, tamanho_thread;

  /* alocacao do vetor A */
  a = (double *) malloc(sizeof(double) * tam_vet);

  /* alocacao do vetor B */
  b = (double *) malloc(sizeof(double) * tam_vet);

  /* alocacao do vetor que ira armazenar o resultado temporario de cada thread */
  resultado_thread = (double *) malloc(sizeof(double) * num_threads);

  printf("Inicializando vetores A e B...\n");

  /* inicializacao dos vetores */
  for (i=0; i<tam_vet; i++)
    a[i] = i;

  for (i=0; i<tam_vet; i++)
    b[i] = i;

  printf("Calculando...\n");

  inicio_thread = 0;
  for (i=0; i<num_threads; i++) {
    tamanho_thread = i < (tam_vet % num_threads) ? tam_vet/num_threads + 1 : tam_vet/num_threads;
    parametros[i].id = i;
    parametros[i].inicio = inicio_thread;
    parametros[i].fim = inicio_thread + tamanho_thread;
    printf("Thread %d: intervalo [%d; %d)\n", i, parametros[i].inicio, parametros[i].fim);
    pthread_create(&threads[i], NULL, thread, (void*)&parametros[i]);
    inicio_thread += tamanho_thread;
  }

  for (i=0; i<num_threads; i++) {
    pthread_join(threads[i], NULL);
    prod += resultado_thread[i];
  }

  printf("Terminou!\n");

  /*** imprime o resultado ***/
  printf("******************************************************\n");
  printf("Produto escalar: %.2f\n", prod);
  printf("******************************************************\n");

  free(a);
  free(b);
  free(resultado_thread);
}

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>

double *a, *b, *c;

struct params {
    int inicio;
    int fim;
};

void *thread(void *arg) {
  struct params param = *(struct params *)arg;

  /* soma dos vetores */
  for (int i=param.inicio; i<param.fim; i++)
    c[i] = a[i] + b[i];

  pthread_exit(NULL);
}

void main(int argc, char* argv[]) {
  int tam_vet, i;
  int imprimir = 0;

  if(argc<3){
    printf("uso %s <tamanho vetores> <quantidade threads> [imprimir? 1=sim]\n", argv[0]);
    exit(1);
  }

  if(argc==4 && atoi(argv[3])==1)
    imprimir = 1;

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

  /* alocacao do vetor C */
  c = (double *) malloc(sizeof(double) * tam_vet);

  printf("Inicializando vetores A, B e C...\n");

  /* inicializacao dos vetores */
  for (i=0; i<tam_vet; i++)
    a[i] = i;

  for (i=0; i<tam_vet; i++)
    b[i] = i;

  for (i=0; i<tam_vet; i++)
    c[i] = 0;

  printf("Calculando...\n");

  inicio_thread = 0;
  for (i=0; i<num_threads; i++) {
    tamanho_thread = i < (tam_vet % num_threads) ? tam_vet/num_threads + 1 : tam_vet/num_threads;
    parametros[i].inicio = inicio_thread;
    parametros[i].fim = inicio_thread + tamanho_thread;
    printf("Thread %d: intervalo [%d; %d)\n", i, parametros[i].inicio, parametros[i].fim);
    pthread_create(&threads[i], NULL, thread, (void*)&parametros[i]);
    inicio_thread += tamanho_thread;
  }

  for (i=0; i<num_threads; i++)
      pthread_join(threads[i], NULL);

  printf("Terminou!\n");

  if(imprimir)
  {
    /*** imprime os resultados ***/
    printf("******************************************************\n");
    printf("Vetor C:\n");
    for (i=0; i<tam_vet; i++)
    printf("%10.2f  ", c[i]);
    printf("\n");
    printf("******************************************************\n");
  }

  free(a);
  free(b);
  free(c);
}

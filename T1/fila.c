#include "fila.h"
#include <stdio.h>
/**
* fila.c
* Implementação das funções da fila.h
* Descrições em fila.h
**/
/*
typedef struct elemento_t {
struct elemento_t * anterior;
struct elemento_t * proximo;
aviao_t * dado;
} elemento_t;
*/

elemento_t * aloca_elemento (aviao_t * dado) {
  elemento_t* elemento = (elemento_t*) malloc(sizeof(elemento_t));
  elemento->dado = dado;
  return elemento;
}

void desaloca_elemento (elemento_t * elemento) {
  free(elemento);
}

/*
typedef struct {
elemento_t * primeiro;
elemento_t * ultimo;
size_t n_elementos;
} fila_ordenada_t;
*/

fila_ordenada_t * criar_fila () {
  fila_ordenada_t *fila = (fila_ordenada_t*) malloc(sizeof(fila_ordenada_t));
  fila->primeiro = NULL;
  fila->ultimo = NULL;
  fila->n_elementos = 0;
  printf("\ncriandofila\n");
  pthread_mutex_init(&fila->mutex, NULL);
  return fila;
}

// Não se esqueca de desalocar todos os elementos antes de finalizar.
void desaloca_fila (fila_ordenada_t * fila) {
  elemento_t *aux_primeiro = fila->primeiro; // Primeiro elemento
  for (unsigned int i = 0; i < fila->n_elementos; i++){
    elemento_t *aux_proximo = aux_primeiro->proximo;  // auxiliar recebe primeiro+i
    free(aux_primeiro);  // libera o primeiro atual
    aux_primeiro = aux_proximo; // passa o próximo para o primeiro
  }
  pthread_mutex_destroy(&fila->mutex);
  free(fila);
}

  /// rever
void inserir (fila_ordenada_t * fila, aviao_t * dado) {
  pthread_mutex_lock(&fila->mutex);
  elemento_t* elemento = aloca_elemento(dado);
  if (fila->n_elementos == 0) {
    fila->primeiro = elemento;
    fila->ultimo = elemento;
  } else {
    if (dado->combustivel < 10) {
    fila->primeiro->anterior = elemento;
    elemento->proximo = fila->primeiro;
    fila->primeiro = elemento;
  } else {
    if (dado->id < fila->ultimo->dado->id) {
      elemento_t* a = fila->primeiro;
      while (a->dado->combustivel < 10) {
        a = a->proximo;
        printf("1111\n");
      }
      elemento->proximo = a->proximo;
      elemento->anterior = a;
      a->proximo->anterior = elemento;
      a->proximo = elemento;
    } else {
      fila->ultimo->proximo = elemento;
      elemento->anterior = fila->ultimo;
      fila->ultimo = elemento;
      }
    }
  }
  fila->n_elementos++;
  pthread_mutex_unlock(&fila->mutex);
  //printf("oi\n");
}

aviao_t * remover (fila_ordenada_t * fila) {
  pthread_mutex_lock(&fila->mutex);
  if (fila->n_elementos == 0){
    //printf("Impossível!"); // era para ser uma exceção
    pthread_mutex_unlock(&fila->mutex);
    return NULL;
  }
  if (fila->n_elementos == 1){
    aviao_t *dado = fila->primeiro->dado;
    free(fila->primeiro);
    fila->primeiro = NULL;
    fila->n_elementos--;
    pthread_mutex_unlock(&fila->mutex);
    return dado;
  }
  aviao_t *dado = fila->primeiro->dado;
  elemento_t* novo_primeiro = fila->primeiro->proximo;  // receberá o segundo elemento(primeiro+1)
  novo_primeiro->anterior = NULL;  // NULL no segundo, porque ele será o primeiro
  free(fila->primeiro);
  fila->primeiro = novo_primeiro;
  fila->n_elementos--;
  pthread_mutex_unlock(&fila->mutex);
  return dado;

}

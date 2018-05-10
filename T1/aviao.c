#include "aviao.h"
#include "stdlib.h"

/**
 * aviao.c
 * Implementação das funções do aviao.h
 * Descrições em aviao.h
 **/

/*
typedef struct {
    pthread_t thread; // Uso dos recursos pelo avião é monitorado pela thread
  size_t combustivel; // Indica prioridade do avião para pousar
  size_t id; // Identificador do avião: 0, 1, 2, ...
} aviao_t;
*/


aviao_t * aloca_aviao (size_t combustivel, size_t id) {
  aviao_t* aviao = (aviao_t*) malloc(sizeof(aviao_t));
  aviao->combustivel = combustivel;
  aviao->id = id;
  aviao->status = 0; // quer pousar
  return aviao;
}

void desaloca_aviao(aviao_t* aviao) {
  pthread_exit(&(aviao->thread)); // fim da thread do avião
  free(aviao);
}

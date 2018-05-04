#ifndef AEROPORTO_H
#define AEROPORTO_H

#include "aviao.h"

typedef size_t tempo_t;

typedef struct {
	size_t n_pistas;
	size_t n_portoes;
	size_t n_esteiras;
	size_t n_max_avioes_esteira;
	tempo_t t_pouso_decolagem;
	tempo_t t_remover_bagagens;
	tempo_t t_inserir_bagagens;
	tempo_t t_bagagens_esteira;
	sem_t* sem_esteiras;
	semt_t sem_portoes;
	sem_t sem_pistas;
	fila_ordenada_t* fila_bagagem;
	fila_ordenada_t* fila_decolagem;
	fila_ordenada_t* fila_pouso;

	// Adicionar aqui outros atributos que você achar necessários.
	// Exemplo: esteiras, portões, etc...
} aeroporto_t;


/**
 * Esta função deve fazer a alocação dinâmica de um aeroporto (malloc)
 * e atribuir os parâmetros (recebidos por um array de unsigned ints)
 * aos atributos da struct aeroporto
 **/
aeroporto_t* iniciar_aeroporto (size_t* args, size_t n_args);

/**
 * Esta função deve ser chamada quando um novo avião se aproxima
 * do aeroporto. Nesta situação um avião deve pousar em seguida,
 * mas somente se houver uma pista livre para ele.
 **/
void aproximacao_aeroporto (aeroporto_t* aeroporto, aviao_t* aviao);

/**
 * Esta função deve fazer com que o aviao pouse, utilizando uma pista livre.
 * Ela não pode ser chamada se não houver pistas livres. Em seguida o avião
 * deve se acoplar a um portão, caso haja um livre, se não houver, ele deve
 * esperar.
 **/
void pousar_aviao (aeroporto_t* aeroporto, aviao_t* aviao);

/**
 * Esta função deve acoplar um avião a um portão de embarque livre.
 * Ela não deve ser chamada caso não hajam portões livres. Em seguida, o
 * o avião deve transportar suas bagagens para fora e receber novas bagagens.
 * Um avião não pode decolar sem receber novas bagagens.
 **/
void acoplar_portao (aeroporto_t* aeroporto, aviao_t* aviao);

/**
 * Esta função deve levar as bagagens de um avião para uma esteira e
 * colocar novas bagagens num avião para que ele seja liberado para
 * decolagem. Um avião não pode decolar sem receber novas bagagens.
 * Um avião não pode receber novas bagagens sem colocar as antigas
 * em uma esteira.
 **/
void transportar_bagagens (aeroporto_t* aeroporto, aviao_t* aviao);

/**
 * Esta função deve colocar novas bagagens numa esteira, caso ela
 * esteja disponível. A esteira ficará ocupada enquanto houverem
 * bagagens nela.
 **/
void adicionar_bagagens_esteira (aeroporto_t* aeroporto, aviao_t* aviao);

/**
 * Esta função deve fazer com que um avião decole, caso haja
 * uma pista disponível para tal. Um avião que decolar deve
 * ter sua execução terminada.
 **/
void decolar_aviao (aeroporto_t* aeroporto, aviao_t* aviao);

/**
 * Esta função deve desalocar todos os recursos previamente
 * alocados pelo aeroporto. Retorna 1 caso bem sucedido.
 **/
int finalizar_aeroporto (aeroporto_t* aeroporto);

#endif

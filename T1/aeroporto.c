#include "aeroporto.h"

/**
 * aeroporto.c
 * Implementação das funções do aeroporto.h
 * Descrições em aeroporto.h
 **/

aeroporto_t* iniciar_aeroporto (size_t* args, size_t n_args) {
	aeroporto_t* aeroporto = (aeroporto_t*) malloc(sizeof(aeroporto_t));
	// Variáveis discretas (inicio n_)
	aeroporto->n_pistas = n_args[0];
	aeroporto->n_portoes = n_args[1];
	aeroporto->n_esteiras = n_args[2];
	aeroporto->n_max_avioes_esteira = n_args[3];
	// Variáveis temporais (inicio t_)
	aeroporto->t_pouso_decolagem = n_args[4];
	aeroporto->t_remover_bagagens = n_args[5];
	aeroporto->t_inserir_bagagens = n_args[6];
	aeroporto->t_bagagens_esteira = n_args[7];
	// Semáforos
	aeroporto->sem_esteiras = (aeroporto->esteiras*) malloc(sizeof(aeroporto->esteiras));
	for (size_t i = 0; i < aeroporto->n_esteiras; i++) {
		sem_init(&aeroporto->sem_esteiras[i], 0, aeroporto->n_max_avioes_esteira);
	}
	sem_init(&aeroporto->sem_portoes, 0, aeroporto->n_portoes);
	sem_init(&aeroporto->sem_pistas, 0, aeroporto->n_pistas);
	// Filas
	aeroporto->fila_bagagem = criar_fila();
	aeroporto->fila_decolagem = criar_fila();
	aeroporto->fila_pouso = criar_fila();
	return aeroporto;
}

void aproximacao_aeroporto (aeroporto_t* aeroporto, aviao_t* aviao) {
	inserir(aeroporto->fila_pouso, aviao);
}

void pousar_aviao (aeroporto_t* aeroporto, aviao_t* aviao) {
	sem_wait(&aeroporto->sem_pistas);

	remover(aeroporto->fila_pouso);  // remove aviao da fila de pouso
	sleep(aeroporto->t_pouso_decolagem);

	sem_post(&aeroporto->sem_pistas);
	sem_wait(&aeroporto->sem_portoes);

	inserir(aeroporto->fila_bagagem, aviao);  // insere aviao na fila de portao/bagagem
	acoplar_portao(aeroporto, aviao);
}

void acoplar_portao (aeroporto_t* aeroporto, aviao_t* aviao) {

	transportar_bagagens(aeroporto, aviao);

	sem_post(&aeroporto->sem_portoes);

}

void transportar_bagagens (aeroporto_t* aeroporto, aviao_t* aviao) {
	int i = -1;
	int* valor_sem = 0;
	do {
		i++;
		if (i == aeroporto->n_esteiras) {
				i = 0;
		}

		sem_t aux_sem = aeroporto->sem_esteiras[i];
		valor_sem = 0;
		sem_getvalue(&aux_sem, valor_sem);
	} while(!*valor_sem);

	sem_wait(&sem_esteiras[i]);
	sleep(aeroporto->t_remover_bagagens);

	adicionar_bagagens_esteira(aeroporto, aviao);

	sleep(aeroporto->t_inserir_bagagens);

	inserir(aeroporto->fila_decolagem, aviao);
	sem_post(&sem_esteiras[i]);
}

void adicionar_bagagens_esteira (aeroporto_t* aeroporto, aviao_t* aviao) {
	sleep(aeroporto->t_bagagens_esteira);
	remover(aeroporto->fila_bagagem);

}

void decolar_aviao (aeroporto_t* aeroporto, aviao_t* aviao) {
	sem_wait(&aeroporto->sem_pistas);

	remover(aeroporto->fila_decolagem);  // remove aviao da fila de decolagem
	sleep(aeroporto->t_pouso_decolagem);

	sem_post(&aeroporto->sem_pistas);
}

int finalizar_aeroporto (aeroporto_t* aeroporto) {
	for (size_t i = 0; i < aeroporto->n_esteiras; i++) {
		free(aeroporto->esteiras[i]);
	}
	free(aeroporto);
	return 1; // deveria ter um teste
}

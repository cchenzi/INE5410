#include "aeroporto.h"
#include "unistd.h"
#include "stdlib.h"
#include "stdio.h"
/**
 * aeroporto.c
 * Implementação das funções do aeroporto.h
 * Descrições em aeroporto.h
 **/

aeroporto_t* iniciar_aeroporto (size_t* args, size_t n_args) {
	aeroporto_t* aeroporto = (aeroporto_t*) malloc(sizeof(aeroporto_t));
	// Variáveis discretas (inicio n_)
	aeroporto->n_pistas = args[0];
	aeroporto->n_portoes = args[1];
	aeroporto->n_esteiras = args[2];
	aeroporto->n_max_avioes_esteira = args[3];
	// Variáveis temporais (inicio t_)
	aeroporto->t_pouso_decolagem = args[4];
	aeroporto->t_remover_bagagens = args[5];
	aeroporto->t_inserir_bagagens = args[6];
	aeroporto->t_bagagens_esteira = args[7];
	// Semáforos
	int aux = aeroporto->n_esteiras * aeroporto->n_max_avioes_esteira;
	sem_init(&aeroporto->sem_esteiras, 0, aux);
	sem_init(&aeroporto->sem_portoes, 0, aeroporto->n_portoes);
	sem_init(&aeroporto->sem_pistas, 0, aeroporto->n_pistas);
	// Filas
	aeroporto->fila_decolagem = criar_fila();
	aeroporto->fila_pouso = criar_fila();
	return aeroporto;
}

void aproximacao_aeroporto (aeroporto_t* aeroporto, aviao_t* aviao) {

	printf("\n99999999999999\n");
	inserir(aeroporto->fila_pouso, aviao);
	printf("Avião %zu está se aproximando, com %zuL de combústivel!\n", aviao->id, aviao->combustivel);
}

void pousar_aviao (aeroporto_t* aeroporto, aviao_t* aviao) {
	sem_wait(&aeroporto->sem_pistas);

	remover(aeroporto->fila_pouso);  // remove aviao da fila de pouso
	sleep(aeroporto->t_pouso_decolagem);
	printf("Avião %zu pousou!\n", aviao->id);

	sem_post(&aeroporto->sem_pistas);
	sem_wait(&aeroporto->sem_portoes);

	acoplar_portao(aeroporto, aviao);
}

void acoplar_portao (aeroporto_t* aeroporto, aviao_t* aviao) {
	printf("Avião %zu acoplado no portão!\n", aviao->id);
	transportar_bagagens(aeroporto, aviao);

	sem_post(&aeroporto->sem_portoes);

}

void transportar_bagagens (aeroporto_t* aeroporto, aviao_t* aviao) {
	sleep(aeroporto->t_remover_bagagens);
	printf("Avião %zu removeu bagagens do avião!\n", aviao->id);

	adicionar_bagagens_esteira(aeroporto, aviao);

	sleep(aeroporto->t_inserir_bagagens); //
	printf("Avião %zu inseriu bagagens no avião\n", aviao->id);
	inserir(aeroporto->fila_decolagem, aviao);
	decolar_aviao(aeroporto, aviao);
}

void adicionar_bagagens_esteira (aeroporto_t* aeroporto, aviao_t* aviao) {
	sem_wait(&aeroporto->sem_esteiras);

	sleep(aeroporto->t_bagagens_esteira);
	printf("Avião %zu inseriu bagagens na esteira!\n", aviao->id);

	sem_post(&aeroporto->sem_esteiras);

}

void decolar_aviao (aeroporto_t* aeroporto, aviao_t* aviao) {
	sem_wait(&aeroporto->sem_pistas);

	remover(aeroporto->fila_decolagem);
	sleep(aeroporto->t_pouso_decolagem);
	printf("Avião %zu decolou!", aviao->id);

	sem_post(&aeroporto->sem_pistas);
}

int finalizar_aeroporto (aeroporto_t* aeroporto) {
	desaloca_fila(aeroporto->fila_pouso);
	desaloca_fila(aeroporto->fila_decolagem);
	sem_destroy(&(aeroporto->sem_pistas));
	sem_destroy(&(aeroporto->sem_portoes));
	sem_destroy(&(aeroporto->sem_esteiras));
	free(aeroporto);
	return 1; // deveria ter um teste
}

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
	aeroporto->fila_pistas = criar_fila();
	return aeroporto;
}


void tratar_fila(aeroporto_t* aeroporto) {
	aviao_t* aviaoAux = remover(aeroporto->fila_pistas);  // remove aviao da fila de pouso
	if (aviaoAux == NULL){
		printf("\ngrrrrr\n");
		return;
	}
	if (aviaoAux->status == 0) {
		pousar_aviao(aeroporto, aviaoAux);
	} else {
		decolar_aviao(aeroporto, aviaoAux);
	}
}

void aproximacao_aeroporto (aeroporto_t* aeroporto, aviao_t* aviao) {
	inserir(aeroporto->fila_pistas, aviao);
	tratar_fila(aeroporto);
	printf("Avião %zu está se aproximando, com %zuL de combústivel!\n", aviao->id, aviao->combustivel);
}

void pousar_aviao (aeroporto_t* aeroporto, aviao_t* aviao) {

	usleep(1000*aeroporto->t_pouso_decolagem);
	printf("Avião %zu pousou!\n", aviao->id);
	sem_wait(&aeroporto->sem_portoes);

	acoplar_portao(aeroporto, aviao);
}

void acoplar_portao (aeroporto_t* aeroporto, aviao_t* aviao) {
	printf("Avião %zu acoplado no portão!\n", aviao->id);
	transportar_bagagens(aeroporto, aviao);

	sem_post(&aeroporto->sem_portoes);

}

void transportar_bagagens (aeroporto_t* aeroporto, aviao_t* aviao) {
	usleep(1000*aeroporto->t_remover_bagagens);
	printf("Avião %zu removeu bagagens do avião!\n", aviao->id);

	//adicionar_bagagens_esteira(aeroporto, aviao);

	usleep(1000*aeroporto->t_inserir_bagagens); //
	printf("Avião %zu inseriu bagagens no avião\n", aviao->id);
	aviao->status = 1; // quer decolar
	inserir(aeroporto->fila_pistas, aviao);
	printf("chamou aviai=o\n");
	tratar_fila(aeroporto);
}

void adicionar_bagagens_esteira (aeroporto_t* aeroporto, aviao_t* aviao) {
	sem_wait(&aeroporto->sem_esteiras);
	/// CADA ESTEIRA É UMA THREAD
	usleep(1000*aeroporto->t_bagagens_esteira);
	printf("Avião %zu inseriu bagagens na esteira!\n", aviao->id);

	sem_post(&aeroporto->sem_esteiras);

}

void decolar_aviao (aeroporto_t* aeroporto, aviao_t* aviao) {
	usleep(1000*aeroporto->t_pouso_decolagem);
	printf("Avião %zu decolou!", aviao->id);
	tratar_fila(aeroporto);
	desaloca_aviao(aviao);

}

int finalizar_aeroporto (aeroporto_t* aeroporto) {
	desaloca_fila(aeroporto->fila_pistas);
	sem_destroy(&(aeroporto->sem_pistas));
	sem_destroy(&(aeroporto->sem_portoes));
	sem_destroy(&(aeroporto->sem_esteiras));
	free(aeroporto);
	return 1; // deveria ter um teste
}

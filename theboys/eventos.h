#ifndef EVENTOS
#define EVENTOS

#include "conjunto.h"
#include "fprio.h"
#include "mundo.h"

/* estrutura de um evento qualquer */
struct evento{
	
	/* item1 e item2: IDs dos herois/bases/missoes que atuam no evento */
	int tipo, 
		tempo,
		item1, 
		item2;
};

/* Cria um evento qualquer e insere ele na LEF */
void cria_evento(struct fprio_t *lef, int tipo, int tempo, int item1, int item2);

/* Cria os eventos inicias para os herois e as missoes e agendo o fim do mundo*/
void cria_eventos_iniciais(struct fprio_t *lef, struct mundo *w);

/* O heroi h chega na base b e decide se espera ou desiste */
void chega(struct fprio_t *lef, struct mundo *w, int tempo, int h, int b);
  
/* O heroi h entra na fila de espera da base b e avisa o porteiro */
void espera(struct fprio_t *lef, struct mundo *w, int tempo, int h, int b);
  
/* O heroi h viaja a uma nova aleatoria */
void desiste(struct fprio_t *lef, struct mundo *w, int tempo, int h, int b);

/* Enquanto houver vaga na base b e herois na fila de espera o porteiro admite eles */
void avisa(struct fprio_t *lef, struct mundo *w, int tempo, int b);

/* O heroi h entra na base b e eh calculada e agendada sua saida */
void entra(struct fprio_t *lef, struct mundo *w, int tempo, int h, int b);

/* O heroi h sai da base b e viaja para uma base aleatoria */
/* O porteiro de b eh avisado */
void sai(struct fprio_t *lef, struct mundo *w, int tempo, int h, int b);

/* O heroi h viaja ate a base b */
void viaja(struct fprio_t *lef, struct mundo *w, int tempo, int h, int b);

/* O heroi h morre na missao m */
void morre(struct fprio_t *lef, struct mundo *w, int tempo, int h, int m);

/* Tenta cumprir a missao m */
/* Se houver uma base mais proxima apta para a missao ela eh cumprida e */
/* os herois da base que cumpre a missao podem morrer */
/* Se nao houver uma base apta a missao eh adiada 24 horas */
void missao(struct fprio_t *lef, struct mundo *w, int tempo, int m);

/* O mundo acaba e os as estatisticas sao impressas */
void fim(struct mundo *w, int tempo);

/* Executa o evento 'e' e incrementa o numero de eventos tratados de mundo w */
void executa_evento(struct fprio_t *lef, struct mundo *w, struct evento *e);

/* Inicia a simulacao de eventos do mundo w e encerra com fim do mundo */
/* Retira todos eventos da LEF */
void simulacao(struct fprio_t *lef, struct mundo *w);

#endif

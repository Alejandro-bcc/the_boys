#ifndef EVENTOS
#define EVENTOS

#include <stdio.h>
#include <stdlib.h>
#include "mundo.h"
#include "conjunto.h"
#include "fprio.h"

/* estrutura de um evento qualquer */
struct evento{
	
	int tipo, tempo;
	void *item1;
	void *item2;
};

/* cria um evento qualquer e insere ele na LEF  */
void cria_evento(struct fprio_t *lef, int tipo, int tempo, void *item1, void *item2);

/* cria os eventos inicias para os herois e as missoes */
/* e agenda o fim do mundo */
void cria_eventos_iniciais(struct fprio_t *e, struct mundo *m);

void chega(int tempo, struct heroi h, struct base b);
/*  
void espera(struct heroi h, struct base b);

void desiste(struct mundo m, struct heroi h);

void avisa();

void entra();

void sai();

void viaja();

void morre();

void missao();

void fim();
  */
void executa_evento(struct evento *e);

void simulacao(struct fprio_t *lef);

#endif

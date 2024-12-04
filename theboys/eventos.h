#ifndef EVENTOS
#define EVENTOS

#include <stdio.h>
#include <stdlib.h>
#include "mundo.h"
#include "conjunto.h"
#include "fprio.h"

/* estrutura de um evento qualquer */
struct evento{
	
	int tipo, 
		tempo,
		item1,
		item2;
};

/* cria um evento qualquer e insere ele na LEF  */
void cria_evento(struct fprio_t *lef, int tipo, int tempo, int item1, int item2);

/* cria os eventos inicias para os herois e as missoes */
/* e agenda o fim do mundo */
void cria_eventos_iniciais(struct fprio_t *lef, struct mundo *m);

void chega(struct fprio_t *lef, struct mundo *m, int tempo, int h, int b);
  
void espera(struct fprio_t *lef, struct mundo *m, int tempo, int heroi, int base);
  
void desiste(struct fprio_t *lef, struct mundo *m, int tempo, int heroi, int base);

void avisa(struct fprio_t *lef, struct mundo *m, int tempo, int base);

void entra();

void sai();

void viaja();

void morre();

void missao();

void fim(int tempo);

void executa_evento(struct fprio_t *lef, struct mundo *m, struct evento *e);

void simulacao(struct fprio_t *lef, struct mundo *m);

#endif

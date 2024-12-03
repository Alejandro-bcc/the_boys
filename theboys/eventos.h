#include <stdio.h>
#include <stdlib.h>
#include "mundo.h"
#include "conjunto.h"
#include "fprio.h"

/* apenas cria a lista de eventos futuros 
void cria_fila_eventos();

// destroi a fila de eventos de libera a memoria 
void destroi_fila_eventos();  */

/* cria os eventos inicias para os herois e as missoes */
/* e agenda o fim do mundo */
void cria_eventos_iniciais(struct fprio *e, struct mundo m);

void chega(struct heroi h, struct base b);

void espera(struct heroi h, struct base b);

void desiste(struct mundo m, struct heroi h);

void avisa();

void entra();

void sai();

void viaja();

void morre();

void missao();

void fim();

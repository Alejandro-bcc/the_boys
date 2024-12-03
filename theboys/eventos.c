#include "eventos.h"

#define FIM 1
#define CHEGA 2
#define ESPERA 3
#define DESISTE 4
#define AVISA 5
#define ENTRA 6
#define SAI 7
#define VIAJA 8
#define MORRE 9
#define MISSAO 10

void cria_evento(struct fprio_t *f, int tipo, int tempo, void *item1, void *item2){
	
	struct evento *novo;

	if(!(novo = malloc(sizeof(struct evento))))
		return;

	novo->tipo = tipo;
	novo->tempo = tempo;
	novo->item1 = item1;
	novo->item2 = item2;

	fprio_insere(f, novo, tipo, tempo);
}

void executa_evento(struct evento *e){

	switch(e->tipo){
		case CHEGA:
			chega(e->tempo, (struct heroi*)e->item1, e->item2);
	}
}

void cria_eventos_inicias(struct fprio_t *f, struct mundo *m){
	
	int i, tempo, base_aleat;

	cria_evento(f, FIM, m->fim, m, NULL);
	
	for(i=0; i < m->n_herois; i++){	

		tempo = rand() % 4321;
		base_aleat = rand() % m->n_bases;
		cria_evento(f, CHEGA, tempo, m->herois[i], m->bases[base_aleat]);
	}
/*  
	for(i=0; i < m->n_missoes; i++){

		tempo = rand() % m->fim;
		cria_evento(f, MISSAO, tempo, NULL, NULL);
	}
*/
}

void simulacao(struct fprio_t *lef){

	struct evento *e;
	int tipo, tempo;

	e = fprio_retira(lef, &tipo, &tempo);
	executa_evento(e);
}

void chega(int tempo, struct heroi h, struct base b){

	int vagas, espera;

	h.id_base = b.id;

	vagas = b.lotacao - cjto_card(b.presentes);

	if(vagas > 0)
		espera = 1;
	else
		espera = h.paciencia > (10 * lista_tamanho(b.espera));

	if(espera)
		printf("%d: CHEGA HEROI %d BASE %d (%d/ %d) ESPERA", tempo, h.id. b.id, b.lotacao - vagas, b.lotacao);
	else
		printf("%d: CHEGA HEROI %d BASE %d (%d/ %d) DESISTE", tempo, h.id. b.id, b.lotacao - vagas, b.lotacao);
}
/*  
void espera(struct heroi h, struct base b){

	lista_insere(b.espera, h.id, -1);
	//cria evento DESISTE
}

void desiste(struct mundo m, struct heroi h){

	int nova_base;

	nova_base = rand() % m.NBases;
	// cria evento VIAJA
}

void fim()

  */

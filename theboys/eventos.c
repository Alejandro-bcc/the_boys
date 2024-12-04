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

void cria_evento(struct fprio_t *lef, int tipo, int tempo, int item1, int item2){
	
	struct evento *novo;

	if(!(novo = malloc(sizeof(struct evento))))
		return;

	novo->tipo = tipo;
	novo->tempo = tempo;
	novo->item1 = item1;
	novo->item2 = item2;

	fprio_insere(lef, novo, tipo, tempo);
}

void executa_evento(struct fprio_t *lef, struct mundo *m, struct evento *e){

	switch(e->tipo){
		case FIM:
			fim(e->tempo);
			break;
		case CHEGA:
			chega(lef, m, e->tempo, e->item1, e->item2);
			break;
		case ESPERA:
			espera(lef, m, e->tempo, e->item1, e->item2);
			break;
		case DESISTE:
			desiste(lef, m, e->tempo, e->item1, e->item2);
			break;  
		case AVISA:
			avisa(lef, m, e->tempo, e->item1, e->item2);
			break;/*  
		case ENTRA:
			entra();
			break;
		case SAI:
			sai();
			break;
		case VIAJA:
			viaja();
			break;
		case MORRE:
			morre();
			break;
		case MISSAO:
			missao();
			break;  */
	}
}

void cria_eventos_iniciais(struct fprio_t *lef, struct mundo *m){
	
	int i, tempo, base_aleat;

	cria_evento(lef, FIM, m->fim, -1, -1);
	
	for(i=0; i < m->n_herois; i++){	
		
		tempo = rand() % 4321;
		base_aleat = rand() % m->n_bases;
		cria_evento(lef, CHEGA, tempo, i, base_aleat);
	}
/*  
	for(i=0; i < m->n_missoes; i++){

		tempo = rand() % m->fim;
		cria_evento(f, MISSAO, tempo, NULL, NULL);
	}
*/
}

void simulacao(struct fprio_t *lef, struct mundo *m){

	struct evento *e;
	int tipo, tempo;

	do{
		e = fprio_retira(lef, &tipo, &tempo);
		executa_evento(lef, m, e);
		free(e);

	}while(tipo != FIM);
	printf("\n");
}

void chega(struct fprio_t *lef, struct mundo *m, int tempo, int heroi, int base){

	struct heroi *h;
	struct base *b;
	int vagas, espera;

	h = &m->herois[heroi];
	b = &m->bases[base];

	h->id_base = b->id;

	vagas = b->lotacao - cjto_card(b->presentes);

	if(vagas > 0)
		espera = 1;
	else
		espera = h->paciencia > (10 * lista_tamanho(b->espera));

	if(espera){

		printf("%6d: CHEGA   HEROI %2d BASE %d (%2d /%2d) ESPERA\n", tempo, heroi, base, b->lotacao - vagas, b->lotacao);
		cria_evento(lef, ESPERA, tempo, heroi, base);
	}
	else{

		printf("%6d: CHEGA   HEROI %2d BASE %d (%2d /%2d) DESISTE\n", tempo, heroi, base, b->lotacao - vagas, b->lotacao);
		cria_evento(lef, DESISTE, tempo, heroi, base);
	}

}

void espera(struct fprio_t *lef, struct mundo *m, int tempo, int heroi, int base){

	struct heroi *h;
	struct base *b;

	h = &m->herois[heroi];
	b = &m->bases[base];

	printf("%6d: ESPERA  HEROI %2d BASE %d (%2d)\n", tempo, heroi, base, lista_tamanho(b->espera));
	lista_insere(b->espera, h->id, -1);
	cria_evento(lef, AVISA, tempo, base, -1);
}

void desiste(struct fprio_t *lef, struct mundo *m, int tempo, int heroi, int base){
	
	int nova_base;

	nova_base = rand() % m->n_bases;
	printf("%6d: DESISTE HEROI %2d BASE %d\n", tempo, heroi, base);
	cria_evento(lef, VIAJA, tempo, heroi, nova_base);
}

void avisa(struct fprio_t *lef, struct mundo *m, int tempo, int base){

	struct base *b;
	int vagas, heroi_fila;
	
	b = &m->bases[base];

	vagas = b->lotacao - cjto_card(b->presentes);
	while(vagas != 0){

		lista_consulta(b->espera, &heroi_fila, 0);
		lista_retira(b->espera, &heroi_fila, 0);
		cjto_insera(b->presentes, heroi_fila);
		cria_evento(lef, ENTRA, tempo, heroi_fila, base);
	}

}

void fim(int tempo){

	printf("%6d: FIM\n", tempo);
}

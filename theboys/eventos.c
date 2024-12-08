#include <stdio.h>
#include <stdlib.h>
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

	/* erro, ponteiro invalido */
	if(lef == NULL)
		return;

	/* erro, alocacao mal-sucedida */
	if(!(novo = malloc(sizeof(struct evento))))
		return;

	novo->tipo = tipo;
	novo->tempo = tempo;
	novo->item1 = item1;
	novo->item2 = item2;

	fprio_insere(lef, novo, tipo, tempo);
}

void executa_evento(struct fprio_t *lef, struct mundo *w, struct evento *e){

	/* erro, ponteiro invalido */
	if(lef == NULL || w == NULL)
		return;

	w->n_eventos++;

	switch(e->tipo){
		case FIM:
			fim(w, e->tempo);
			break;
		case CHEGA:
			chega(lef, w, e->tempo, e->item1, e->item2);
			break;
		case ESPERA:
			espera(lef, w, e->tempo, e->item1, e->item2);
			break;
		case DESISTE:
			desiste(lef, w, e->tempo, e->item1, e->item2);
			break;  
		case AVISA:
			avisa(lef, w, e->tempo, e->item1);
			break;  
		case ENTRA:
			entra(lef, w, e->tempo, e->item1, e->item2);
			break;  
		case SAI:
			sai(lef, w, e->tempo, e->item1, e->item2);
			break;  
		case VIAJA:
			viaja(lef, w, e->tempo, e->item1, e->item2);
			break;  
		case MORRE:
			morre(lef, w, e->tempo, e->item1, e->item2);
			break;  
		case MISSAO:
			missao(lef, w, e->tempo, e->item1);
			break;
	}
}

void cria_eventos_iniciais(struct fprio_t *lef, struct mundo *w){
	
	int i, tempo, base_aleat;

	/* erro, ponteiro invalido */
	if(lef == NULL || w == NULL)
		return;

	cria_evento(lef, FIM, w->fim, -1, -1);
	
	for(i=0; i < w->n_herois; i++){	
		
		tempo = rand() % 4321;
		base_aleat = rand() % w->n_bases;
		cria_evento(lef, CHEGA, tempo, i, base_aleat);
	}
      
	for(i=0; i < w->n_missoes; i++){

		tempo = rand() % w->fim;
		cria_evento(lef, MISSAO, tempo, i, -1);
	}   
}

void simulacao(struct fprio_t *lef, struct mundo *w){

	struct evento *e;
	int tipo, tempo;

	/* erro, ponteiro invalido */
	if(lef == NULL || w == NULL)
		return;

	do{
		e = fprio_retira(lef, &tipo, &tempo);
		executa_evento(lef, w, e);
		free(e);

	}while(tipo != FIM);
}

void chega(struct fprio_t *lef, struct mundo *w, int tempo, int heroi, int base){

	struct heroi *h;
	struct base *b;
	int vagas, espera;

	/* erro, ponteiro invalido */
	if(lef == NULL || w == NULL)
		return;
	
	h = &w->herois[heroi];
	b = &w->bases[base];

	if(h->morto){

		h = NULL;
		b = NULL;
		return;
	}
	
	h->id_base = b->id;

	vagas = b->lot - cjto_card(b->presentes);

	if(vagas > 0)
		espera = 1;
	else
		espera = h->pac > (10 * lista_tamanho(b->espera));

	if(espera){

		printf("%6d: CHEGA   HEROI %2d BASE %d (%2d /%2d) ESPERA\n", tempo, heroi, base, b->lot - vagas, b->lot);

		cria_evento(lef, ESPERA, tempo, heroi, base);
	}
	else{

		printf("%6d: CHEGA   HEROI %2d BASE %d (%2d /%2d) DESISTE\n", tempo, heroi, base, b->lot - vagas, b->lot);

		cria_evento(lef, DESISTE, tempo, heroi, base);
	}

	h = NULL;
	b = NULL;
}

void espera(struct fprio_t *lef, struct mundo *w, int tempo, int heroi, int base){

	struct heroi *h;
	struct base *b;
	int fila_tam;

	/* erro, ponteiro invalido */
	if(lef == NULL || w == NULL)
		return;

	h = &w->herois[heroi];
	b = &w->bases[base];	

	if(h->morto){

		h = NULL;
		b = NULL;
		return;
	}

	printf("%6d: ESPERA  HEROI %2d BASE %d (%2d )\n", tempo, heroi, base, lista_tamanho(b->espera));

	lista_insere(b->espera, h->id, -1);

	/* atualiza o fila max da base, se necessrio  */
	fila_tam = lista_tamanho(b->espera);

	if(fila_tam > b->fila_max)
		b->fila_max = fila_tam;

	cria_evento(lef, AVISA, tempo, base, -1);

	h = NULL;
	b = NULL;
}

void desiste(struct fprio_t *lef, struct mundo *w, int tempo, int heroi, int base){
	
	int nova_base;

	/* erro, ponteiro invalido */
	if(lef == NULL || w == NULL)
		return;

	if(w->herois[heroi].morto)
		return;	

	nova_base = rand() % w->n_bases;
	printf("%6d: DESISTE HEROI %2d BASE %d\n", tempo, heroi, base);
	cria_evento(lef, VIAJA, tempo, heroi, nova_base);
}

void avisa(struct fprio_t *lef, struct mundo *w, int tempo, int base){

	struct base *b;
	int vagas, n_presentes, heroi_fila;
	
	/* erro, ponteiro invalido */
	if(lef == NULL || w == NULL)
		return;

	b = &w->bases[base];

	n_presentes = cjto_card(b->presentes);
	vagas = b->lot - n_presentes;

	printf("%6d: AVISA   PORTEIRO BASE %d (%2d /%2d) FILA [ ", tempo, base, n_presentes, b->lot);
	lista_imprime(b->espera);
	printf(" ]\n");

	while(vagas != 0 && lista_tamanho(b->espera) != 0){

		lista_retira(b->espera, &heroi_fila, 0);
		cjto_insere(b->presentes, heroi_fila);
		printf("%6d: AVISA   PORTEIRO BASE %d ADMITE %2d\n", tempo, base, heroi_fila);	
		cria_evento(lef, ENTRA, tempo, heroi_fila, base);
		vagas--;
	}

	b = NULL;
}

void entra(struct fprio_t *lef, struct mundo *w, int tempo, int heroi, int base){

	struct heroi *h;
	struct base *b;
	int t_permanencia, t_saida;

	/* erro, ponteiro invalido */
	if(lef == NULL || w == NULL)
		return;

	h = &w->herois[heroi];
	b = &w->bases[base];

	if(h->morto){

		h = NULL;
		b = NULL;
		return;
	}

	t_permanencia = 15 + h->pac * ((rand() % 21) + 1);
	t_saida = tempo + t_permanencia;
	printf("%6d: ENTRA   HEROI %2d BASE %d (%2d /%2d) SAI %d\n", tempo, heroi, base, cjto_card(b->presentes), b->lot, t_saida);
	cria_evento(lef, SAI, t_saida, heroi, base);

	h = NULL;
	b = NULL;
}

void sai(struct fprio_t *lef, struct mundo *w, int tempo, int heroi, int base){

	struct base *b;
	int base_aleat;

	/* erro, ponteiro invalido */
	if(lef == NULL || w == NULL)
		return;

	if(w->herois[heroi].morto)
		return;	

	b = &w->bases[base];

	base_aleat = rand() % w->n_bases;

	cjto_retira(b->presentes, heroi);
	
	printf("%6d: SAI     HEROI %2d BASE %d (%2d /%2d)\n", tempo, heroi, base, cjto_card(b->presentes), b->lot);

	cria_evento(lef, VIAJA, tempo, heroi, base_aleat);
	cria_evento(lef, AVISA, tempo, base, -1);

	b = NULL;
}

void viaja(struct fprio_t *lef, struct mundo *w, int tempo, int heroi, int base){

	struct heroi *h;
	struct base *b_or;
	struct base *b_des;
	int distancia, duracao;

	/* erro, ponteiro invalido */
	if(lef == NULL || w == NULL)
		return;

	h = &w->herois[heroi];
	b_or = &w->bases[h->id_base];
	b_des = &w->bases[base];

	if(h->morto){
		
		h = NULL;
		b_or = NULL;
		b_des = NULL;
		return;
	}

	distancia = calcula_distancia(b_or->local, b_des->local);
	duracao = distancia / h->vel;
	
	printf("%6d: VIAJA   HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n", tempo, heroi, b_or->id, base, distancia, h->vel, tempo + duracao);
	cria_evento(lef, CHEGA, tempo + duracao, heroi, base);	

	h = NULL;
	b_or = NULL;
	b_des = NULL;
}

void morre(struct fprio_t *lef, struct mundo *w, int tempo, int heroi, int missao){

	struct heroi *h;
	struct base *b;

	/* erro, ponteiro invalido */
	if(lef == NULL || w == NULL)
		return;

	h = &w->herois[heroi];
	b = &w->bases[h->id_base];

	cjto_retira(b->presentes, heroi);
	h->morto = 1;
	printf("%6d: MORRE   HEROI %2d MISSAO %d\n", tempo, heroi, missao);
	cria_evento(lef, AVISA, tempo, h->id_base, -1);

	h = NULL;
	b = NULL;
}
  
void missao(struct fprio_t *lef, struct mundo *w, int tempo, int missao){

	struct missao *m;
	struct base *b_aux;    
	struct cjto_t *habs_aux;
	struct heroi *h_aux;
	int i, risco, b_id;

	/* erro, ponteiro invalido */
	if(lef == NULL || w == NULL)
		return;

	m = &w->missoes[missao];
	m->tent++;

	printf("%6d: MISSAO  %2d TENT %d HAB REQ: [ ", tempo, missao, m->tent);
	cjto_imprime(m->habs);
	printf(" ]\n");

	habs_aux = acha_base_apta(w, *m, &b_id);

	if(habs_aux != NULL){

		m->cump = 1;
		b_aux = &w->bases[b_id];
		b_aux->miss_part++;

		printf("%6d: MISSAO  %2d CUMPRIDA BASE %d HABS: [ ", tempo, missao, b_aux->id);
		cjto_imprime(habs_aux);
		printf(" ]\n");
		
		for(i=0; i < w->n_herois; i++){
			if(cjto_pertence(b_aux->presentes, i)){

				h_aux = &w->herois[i];
				risco = m->perigo / (h_aux->pac + h_aux->exp + 1.0);
			
				if(risco > rand() % 31){
					cria_evento(lef, MORRE, tempo, i, missao);
				}else{
					h_aux->exp++;
				}
			}
		}

	}else{
		
		printf("%6d: MISSAO  %2d IMPOSSIVEL\n", tempo, missao);
		cria_evento(lef, MISSAO, tempo + 24*60, missao, -1);
	}
   
	habs_aux = cjto_destroi(habs_aux);
	m = NULL;
	b_aux = NULL;
	h_aux = NULL;
}

void fim(struct mundo *w, int tempo){

	/* erro, ponteiro invalido */
	if(w == NULL)
		return;

	printf("%6d: FIM\n\n", tempo);

	imprime_estatisticas(w);
}

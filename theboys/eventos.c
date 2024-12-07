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
			avisa(lef, m, e->tempo, e->item1);
			break;  
		case ENTRA:
			entra(lef, m, e->tempo, e->item1, e->item2);
			break;  
		case SAI:
			sai(lef, m, e->tempo, e->item1, e->item2);
			break;  
		case VIAJA:
			viaja(lef, m, e->tempo, e->item1, e->item2);
			break;  
		case MORRE:
			morre(lef, m, e->tempo, e->item1, e->item2);
			break;  
		case MISSAO:
			missao(lef, m, e->tempo, e->item1);
			break;
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

	for(i=0; i < m->n_missoes; i++){

		tempo = rand() % m->fim;
		cria_evento(lef, MISSAO, tempo, i, -1);
	}
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

	if(h->morto)
		return;

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

	if(m->herois[heroi].morto)
		return;

	h = &m->herois[heroi];
	b = &m->bases[base];

	printf("%6d: ESPERA  HEROI %2d BASE %d (%2d )\n", tempo, heroi, base, lista_tamanho(b->espera));
	lista_insere(b->espera, h->id, -1);
	cria_evento(lef, AVISA, tempo, base, -1);
}

void desiste(struct fprio_t *lef, struct mundo *m, int tempo, int heroi, int base){
	
	int nova_base;

	if(m->herois[heroi].morto)
		return;	

	nova_base = rand() % m->n_bases;
	printf("%6d: DESISTE HEROI %2d BASE %d\n", tempo, heroi, base);
	cria_evento(lef, VIAJA, tempo, heroi, nova_base);
}

void avisa(struct fprio_t *lef, struct mundo *m, int tempo, int base){

	struct base *b;
	int vagas, heroi_fila;
	
	b = &m->bases[base];

	vagas = b->lotacao - cjto_card(b->presentes);

	printf("%6d: AVISA   PORTEIRO BASE %d (%2d /%2d) FILA [ ", tempo, base, b->lotacao - vagas, b->lotacao);
	lista_imprime(b->espera);
	printf(" ]\n");

	while(vagas != 0 && lista_tamanho(b->espera) != 0){

		lista_consulta(b->espera, &heroi_fila, 0);
		lista_retira(b->espera, &heroi_fila, 0);
		cjto_insere(b->presentes, heroi_fila);
		cria_evento(lef, ENTRA, tempo, heroi_fila, base);
		vagas--;
		printf("%6d: AVISA   PORTEIRO BASE %d ADMITE %2d\n", tempo, base, heroi_fila);	
	}
}

void entra(struct fprio_t *lef, struct mundo *m, int tempo, int heroi, int base){

	struct heroi *h;
	struct base *b;
	int t_permanencia, t_saida;

	h = &m->herois[heroi];
	b = &m->bases[base];
	
	if(h->morto)
		return;	

	t_permanencia = 15 + h->paciencia * ((rand() % 21) + 1);
	t_saida = tempo + t_permanencia;
	printf("%6d: ENTRA   HEROI %2d BASE %d (%2d /%2d) SAI %d\n", tempo, heroi, base, cjto_card(b->presentes), b->lotacao, t_saida);
	cria_evento(lef, SAI, t_saida, heroi, base);
}

void sai(struct fprio_t *lef, struct mundo *m, int tempo, int heroi, int base){

	struct base *b;
	int base_aleat;

	if(m->herois[heroi].morto)
		return;	

	b = &m->bases[base];

	base_aleat = rand() % m->n_bases;

	cjto_retira(b->presentes, heroi);
	
	printf("%6d: SAI     HEROI %2d BASE %d (%2d /%2d)\n", tempo, heroi, base, cjto_card(b->presentes), b->lotacao);

	cria_evento(lef, VIAJA, tempo, heroi, base_aleat);
	cria_evento(lef, AVISA, tempo, base, -1);
}

void viaja(struct fprio_t *lef, struct mundo *m, int tempo, int heroi, int base){

	struct heroi *h;
	struct base *b_or;
	struct base *b_des;
	int distancia, duracao;

	h = &m->herois[heroi];
	b_or = &m->bases[h->id_base];
	b_des = &m->bases[base];

	distancia = calcula_distancia(b_or->local, b_des->local);

	duracao = distancia / h->velocidade;
	
	printf("%6d: VIAJA   HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n", tempo, heroi, b_or->id, base, distancia, h->velocidade, tempo + duracao);
	cria_evento(lef, CHEGA, tempo + duracao, heroi, base);
}

void morre(struct fprio_t *lef, struct mundo *m, int tempo, int heroi, int missao){

	struct heroi *h;
	struct base *b;

	h = &m->herois[heroi];
	b = &m->bases[h->id_base];

	cjto_retira(b->presentes, heroi);
	h->morto = 1;
	printf("%6d: MORRE   HEROI %2d MISSAO %d\n", tempo, heroi, missao);
	cria_evento(lef, AVISA, tempo, h->id_base, -1);
}
  
void missao(struct fprio_t *lef, struct mundo *m, int tempo, int missao){

	struct missao *mis;
	struct base b_aux;
	struct cjto_t *habs_aux;

	mis = &m->missoes[missao];
	mis->tent++;

	printf("%6d: MISSAO %2d TENT %d HAB REQ: [ ", tempo, missao, mis->tent);
	cjto_imprime(mis->habilidades);
	printf(" ]\n");


	if(acha_base_apta(m, mis, &b_aux, &habs_aux)){

		printf("%6d: MISSAO %2d CUMPRIDA BASE ", tempo, missao);
		printf("%d HABS: [ \n", b_aux.id);
		printf(" \n ");
		printf(" ] ");

	}else{
		
		printf("%6d: MISSAO %2d IMPOSSIVEL\n", tempo, missao);
	}

 /*  
	printf("%6d: MISSAO  %2d BASE %d DIST %5d HEROIS [ ", tempo, missao, b_aux->id, dist);
	cjto_imprime(b_aux->presentes);
	printf(" ]\n");
    */

	fprio_tamanho(lef);
	  
//	cria_evento(lef, AVISA, );
}

void fim(/* struct mundo *m,  */ int tempo){

	printf("%6d: FIM\n", tempo);

	//imprime_estatisticas(m);
}

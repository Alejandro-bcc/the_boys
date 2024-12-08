#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mundo.h"

#define INICIO 0
#define FIM 525600
#define HABILIDADES 10
#define TAMANHO 20000

struct mundo * cria_mundo(){

	struct mundo *novo;
	int i;

	/* Alocacao mal-sucedida */
	if(!(novo = malloc(sizeof(struct mundo))))
		return NULL;

	/* Inicializa o mundo */
	novo->inicio = INICIO;
	novo->fim = FIM;
	novo->n_habs = HABILIDADES;
	novo->n_herois = novo->n_habs * 5;
	novo->n_bases = novo->n_herois / 5;
	novo->n_missoes = novo->fim / 100;
	novo->n_eventos = 0;
	novo->max.x = TAMANHO;
	novo->max.y = TAMANHO;

	/* Alocacao mal-sucedida */
	if(!(novo->herois = malloc(sizeof(struct heroi) * novo->n_herois))){

		free(novo);
		return NULL;
	}

	/* Alocacao mal-sucedida */
	if(!(novo->bases = malloc(sizeof(struct base) * novo->n_bases))){

		free(novo);
		return NULL;
	}

	/* Alocacao mal-sucedida */
	if(!(novo->missoes = malloc(sizeof(struct missao) * novo->n_missoes))){
		
		free(novo);
		return NULL;
	}

	/* Inicializa os herois */
	for(i=0; i < novo->n_herois; i++){

		novo->herois[i].id = i;
		novo->herois[i].exp = 0;
		novo->herois[i].pac = rand() % 101;
		novo->herois[i].vel = (rand() % 4951) + 50;
		novo->herois[i].morto = 0;
		novo->herois[i].habs = cjto_aleat((rand() % 3 ) + 1, novo->n_habs);
	}

	/* Inicializa as bases */
	for(i=0; i < novo->n_bases; i++){

		novo->bases[i].id = i;
		novo->bases[i].local.x = rand() % TAMANHO;
		novo->bases[i].local.y = rand() % TAMANHO;
		novo->bases[i].lot = (rand() % 8) + 3;
		novo->bases[i].fila_max = 0;
		novo->bases[i].miss_part = 0;
		novo->bases[i].presentes = cjto_cria(novo->n_herois);
		novo->bases[i].espera = lista_cria();
	}

	/* Inicializa as missoes */
	for(i=0; i < novo->n_missoes; i++){

		novo->missoes[i].id = i;
		novo->missoes[i].local.x = rand() % TAMANHO;
		novo->missoes[i].local.y = rand() % TAMANHO;
		novo->missoes[i].habs = cjto_aleat((rand() % 5) + 6, novo->n_habs);
		novo->missoes[i].perigo = rand() % 101;
		novo->missoes[i].tent = 0;
		novo->missoes[i].cump = 0;
	}

	return novo;
}

struct mundo * destroi_mundo(struct mundo *w){
	
	int i;

	/* erro, ponterio invalido */
	if(w == NULL)
		return NULL;

	/* Libera a memoria dos herois */
	for(i=0; i < w->n_herois; i++)
		w->herois[i].habs = cjto_destroi(w->herois[i].habs);

	free(w->herois);
	w->herois = NULL;

	/* Libera a memoria das bases */
	for(i=0; i < w->n_bases; i++){
		
		w->bases[i].presentes = cjto_destroi(w->bases[i].presentes);
		w->bases[i].espera = lista_destroi(w->bases[i].espera);
	}

	free(w->bases);
	w->bases = NULL;

	/* Libera a memoria das missoes */
	for(i=0; i < w->n_missoes; i++)
		w->missoes[i].habs = cjto_destroi(w->missoes[i].habs);

	free(w->missoes);
	w->missoes = NULL;

	/* Libera a memoria do mundo */
	free(w);
	return NULL;
}

int calcula_distancia(struct coordenadas origem, struct coordenadas destino){

	int aux;

	aux = (destino.x - origem.x) * (destino.x - origem.x);
	aux += (destino.y - origem.y) * (destino.y - origem.y);
 	return sqrt(aux);
}

void ordena_vetor_pares(struct par p[], int n){

    struct par aux;
    int i, j;

    for(i = 1; i < n; i++){

        aux = p[i];
        j = i - 1;

        while(j >= 0 && p[j].cont > aux.cont){
            p[j + 1] = p[j];
            j--;
        }
        p[j + 1] = aux;
    }
}

struct cjto_t * base_apta(struct mundo *w, struct missao m, struct base b){

	struct cjto_t *c_aux1;
	struct cjto_t *c_aux2;
	struct heroi h_aux;
	int i;

	/* ponteiro invalido ou nao há herois na base */
	if(w == NULL || (cjto_card(b.presentes) == 0))
		return NULL;

	c_aux1 = cjto_cria(w->n_habs);

	for(i=0; i < w->n_herois; i++){
		
		if(cjto_pertence(b.presentes, i)){

			h_aux = w->herois[i];
		
			c_aux2 = cjto_uniao(c_aux1, h_aux.habs);
			c_aux1 = cjto_destroi(c_aux1);
			c_aux1 = c_aux2;
		}
	}

	if(!(cjto_contem(c_aux1, m.habs)))
		c_aux1 = cjto_destroi(c_aux1);
	
	c_aux2 = NULL;
	return c_aux1;
}

struct cjto_t * acha_base_apta(struct mundo *w, struct missao m, int *b_id){

	/* vetor com as distancias e os indices das respectivas bases ate a missao */
	struct par *dist;
	struct cjto_t *habs;
	int i;

	/* erro, ponteiro invalidos */
	if(w == NULL || b_id == NULL)
		return NULL;

	/* erro, alocacao mal-sucedida */
	if(!(dist = malloc(sizeof(struct par) * w->n_bases)))
		return NULL;

	/* calcula a distancia ate todas as bases */
	for(i = 0; i < w->n_bases; i++){

		dist[i].id = i;
		dist[i].cont = calcula_distancia(m.local, w->bases[i].local);
	}

	ordena_vetor_pares(dist, w->n_bases);

	i = 0;

	while(i < w->n_bases && ((habs = base_apta(w, m, w->bases[i])) == NULL))
		i++;
   
	free(dist);
	dist = NULL;
	*b_id = i;
	return habs;
}

void imprime_estatisticas(struct mundo *w){

	struct heroi h_aux;
	struct base b_aux;
	struct missao m_aux;
	int i, n_h_mortos, m_cump, min_tent, max_tent, total_tent;
	float decimal_aux;

	/* erro, ponteiro invalido */
	if(w == NULL)
		return;

	n_h_mortos = 0;
	m_cump = 0;
	total_tent = 0;
	min_tent = w->missoes[0].tent;
	max_tent = w->missoes[0].tent;

	/* Imprime informacoes dos herois  */
	for(i=0; i < w->n_herois; i++){

		h_aux = w->herois[i];

		printf("HEROI %2d ",i);

		if(!w->herois[i].morto){

			printf("VIVO  ");

		}else{

			printf("MORTO ");
			n_h_mortos++;
		}

		printf("PAC %3d VEL %4d EXP %4d HABS [ ", h_aux.pac, h_aux.vel, h_aux.exp);
		cjto_imprime(h_aux.habs);
		printf(" ]\n");
	}

	printf("\n");

	/* Imprime informacoes das bases */
	for(i=0; i < w->n_bases; i++){

		b_aux = w->bases[i];
		printf("BASE %d LOT %d FILA MAX %d MISSOES %d\n", i, b_aux.lot, b_aux.fila_max, b_aux.miss_part);
	}

	/* Conta o numero de missoes cumpridas e determina o minimo e maximo de tentativas */
	for(i=0; i < w->n_missoes; i++){
	
		m_aux = w->missoes[i];
		total_tent += m_aux.tent;

		if(m_aux.cump)
			m_cump++;

		if(m_aux.tent > max_tent)
			max_tent = m_aux.tent;


		if(m_aux.tent < min_tent)
			min_tent = m_aux.tent;
	}

	decimal_aux = (float)(m_cump * 100)/w->n_missoes;

	printf("\n");

	printf("EVENTOS TRATADOS: %d\n", w->n_eventos);
	printf("MISSOES CUMPRIDAS: %d/%d (%.1f%%)\n", m_cump, w->n_missoes, decimal_aux);
	
	decimal_aux = (float)(total_tent / w->n_missoes);

	printf("TENTATIVAS/MISSAO: MIN %d, MAX %d, MEDIA %.1f\n", min_tent, max_tent, decimal_aux);

	decimal_aux = (float)(n_h_mortos * 100)/w->n_herois;

	printf("TAXA DE MORTALIDADE: %.1f%%", decimal_aux);
}

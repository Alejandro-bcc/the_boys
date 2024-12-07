#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "conjunto.h"
#include "lista.h"
#include "mundo.h"

#define INICIO 0
#define FIM 4600
#define HABILIDADES 10
#define TAMANHO 20000

struct mundo * cria_mundo(){

	struct mundo *novo;
	int i;

	if(!(novo = malloc(sizeof(struct mundo))))
		return NULL;

	novo->inicio = INICIO;
	novo->fim = FIM;
	novo->n_habilidades = HABILIDADES;
	novo->n_herois = novo->n_habilidades * 5;
	novo->n_bases = novo->n_herois / 5;
	novo->n_missoes = novo->fim / 100;
	novo->maximos.x = TAMANHO;
	novo->maximos.y = TAMANHO;

	if(!(novo->herois = malloc(sizeof(struct heroi) * novo->n_herois))){

		free(novo);
		return NULL;
	}

	if(!(novo->bases = malloc(sizeof(struct base) * novo->n_bases))){

		free(novo);
		return NULL;
	}

	if(!(novo->missoes = malloc(sizeof(struct missao) * novo->n_missoes))){
		
		free(novo);
		return NULL;
	}

	for(i=0; i < novo->n_herois; i++){

		novo->herois[i].id = i;
		novo->herois[i].experiencia = 0;
		novo->herois[i].paciencia = rand() % 101;
		novo->herois[i].velocidade = (rand() % 4051) + 50;
		novo->herois[i].morto = 0;
		novo->herois[i].habilidades = cjto_aleat((rand() % 3 ) + 1, novo->n_habilidades);
	}

	for(i=0; i < novo->n_bases; i++){

		novo->bases[i].id = i;
		novo->bases[i].local.x = rand() % TAMANHO;
		novo->bases[i].local.y = rand() % TAMANHO;
		novo->bases[i].lotacao = (rand() % 8) + 3;
		novo->bases[i].fila_max = 0;
		novo->bases[i].mis_part = 0;
		novo->bases[i].presentes = cjto_cria(novo->n_herois);
		novo->bases[i].espera = lista_cria(); //fila 
	}

	for(i=0; i < novo->n_missoes; i++){

		novo->missoes[i].id = i;
		novo->missoes[i].local.x = rand() % TAMANHO;
		novo->missoes[i].local.y = rand() % TAMANHO;
		novo->missoes[i].habilidades = cjto_aleat((rand() % 5) + 6, novo->n_habilidades);
		novo->missoes[i].perigo = rand() % 101;
		novo->missoes[i].tent = 0;
	}

	return novo;
}

struct mundo * destroi_mundo(struct mundo *m){
	
	int i;

	for(i=0; i < m->n_herois; i++)
		m->herois[i].habilidades = cjto_destroi(m->herois[i].habilidades);

	free(m->herois);
	m->herois = NULL;

	for(i=0; i < m->n_bases; i++){
		
		m->bases[i].presentes = cjto_destroi(m->bases[i].presentes);
		m->bases[i].espera = lista_destroi(m->bases[i].espera);
	}

	free(m->bases);
	m->bases = NULL;

	for(i=0; i < m->n_missoes; i++)
		m->missoes[i].habilidades = cjto_destroi(m->missoes[i].habilidades);

	free(m->missoes);
	m->missoes = NULL;

	free(m);
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

int base_apta(struct mundo *m, struct missao *mis, struct base *b, struct cjto_t *habs){

	struct cjto_t *c_aux1; //armezena temporariamente as habs. dos herois de b
//	struct cjto_t *c_aux2; //auxiliar que evita o vazamento de memoria
	struct heroi *h_aux;
	int i;

	/* erro, ponteiro invalido */
	if(m == NULL || mis == NULL || b == NULL)
		return -1;
	
	c_aux1 = cjto_cria(m->n_habilidades);

	for(i=0; i < m->n_herois; i++){

		h_aux = &m->herois[i];

		if(cjto_pertence(b->presentes, i)){

			c_aux1 = cjto_uniao(c_aux1, h_aux->habilidades);
//			cjto_destroi(c_aux1);
//			c_aux1 = c_aux2;
		}
	}

	if(cjto_contem(c_aux1, mis->habilidades)){

		*habs = *c_aux1;
		c_aux1 = cjto_destroi(c_aux1);
		//c_aux2 = cjto_destroi(c_aux2);
		h_aux = NULL;
		return 1;
	}

	h_aux = NULL;
	c_aux1 = cjto_destroi(c_aux1);
	//c_aux2 = cjto_destroi(c_aux2);
	return 0;
}

int acha_base_apta(struct mundo *m, struct missao *mis, struct base *b, struct cjto_t *habs){

	/* vetor com as distancias e os indices das respectivas bases ate a missao */
	struct par *dist;
	//struct cjto_t c_aux;
	int i;

	/* erro, ponteiro invalidos */
	if(m == NULL || mis == NULL || b == NULL)
		return -1;

	/* erro, alocacao mal-sucedida */
	if(!(dist = malloc(sizeof(struct par) * m->n_bases)))
		return -1;

	/* calcula a distancia ate todas as bases  */
	for(i = 0; i < m->n_bases; i++){

		dist[i].id = i;
		dist[i].cont = calcula_distancia(mis->local, m->bases[i].local);
	}

	ordena_vetor_pares(dist, m->n_bases);

	i = 0;

	while(i < m->n_bases && !base_apta(m, mis, &m->bases[dist[i].id], habs))
		i++;

	/* verifica se há uma base apta */
	if(i < m->n_bases){
		
		*b = m->bases[dist[i].id];
		free(dist);
		dist = NULL;
		return 1;
	}

	b = NULL;
	free(dist);
	dist = NULL;
	return 0;
}

void imprime_estatisticas(struct mundo *m){

	struct heroi h_aux;
	struct base b_aux;
	int i;

	for(i=0; i < m->n_herois; i++){

		h_aux = m->herois[i];

		printf("HEROI %2d ",i);

		if(!m->herois[i].morto)
			printf("VIVO  ");
		else
			printf("MORTO ");

		printf("PAC %3d VEL %4d EXP %4d HABS [ ", h_aux.paciencia, h_aux.velocidade, h_aux.experiencia);
		cjto_imprime(h_aux.habilidades);
		printf(" ]\n");
	}

	printf("\n");

	for(i=0; i < m->n_bases; i++){

		b_aux = m->bases[i];
		printf("BASE %d LOT %d FILA MAX %d MISSOES %d\n", i, b_aux.lotacao, b_aux.fila_max, b_aux.mis_part);
	}
}
  
void imprime_heroi(struct heroi h){
	
	printf("Heroi: \n");
	printf("ID: %d\n", h.id);
	printf("Paciencia: %d\n", h.paciencia);
	printf("Velocidade: %d\n", h.velocidade);
	printf("Experiencia: %d\n", h.experiencia);
	printf("ID da base: %d\n", h.id_base);
	printf("Habilidades: ");
	cjto_imprime(h.habilidades);
	printf("\n");
}

void imprime_base(struct base b){

	printf("Base: \n");
	printf("ID: %d\n", b.id);
	printf("Lotacao: %d\n", b.lotacao);
	printf("Local: (%d,%d)\n", b.local.x, b.local.y);
}

void imprime_missao(struct missao ms){	

	printf("Missao: \n");
	printf("ID: %d\n", ms.id);
	printf("Perigo: %d\n", ms.perigo);
	printf("Habilidades: ");
	cjto_imprime(ms.habilidades);
	printf("\n");
	printf("Local: (%d,%d)\n", ms.local.x, ms.local.y);
}

void imprime_mundo(struct mundo *m){
	
	int i;

	if(m == NULL)
		return;

	printf("Mundo:\n");
	printf("Tempo inicial: %d\n", m->inicio);
	printf("Tempo final: %d\n", m->fim);
	printf("Tamanho do mundo: %d\n", m->maximos.x);
	printf("n_herois: %d\n", m->n_herois);
	printf("n_bases: %d\n", m->n_bases);
	printf("n_missoes: %d\n", m->n_missoes);
	printf("\n");
	
	for(i=0; i < m->n_herois; i++){

		printf("%d ", i);
		imprime_heroi(m->herois[i]);
	}

	for(i=0; i < m->n_bases; i++){

		printf("%d ", i);
		imprime_base(m->bases[i]);
	}
/*  
	for(i=0; i < m->n_missoes; i++){

		printf("%d ", i);
		imprime_missao(m->missoes[i]);
	}
*/
}

#include <stdio.h>
#include <stdlib.h>
#include "conjunto.h"
#include "lista.h"
#include "mundo.h"

#define INICIO 0
#define FIM 15000
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
		novo->herois[i].habilidades = cjto_aleat((rand() % 3 ) + 1, novo->n_habilidades);
	}

	for(i=0; i < novo->n_bases; i++){

		novo->bases[i].id = i;
		novo->bases[i].local.x = rand() % TAMANHO;
		novo->bases[i].local.y = rand() % TAMANHO;
		novo->bases[i].lotacao = (rand() % 8) + 3;
		novo->bases[i].presentes = cjto_cria(novo->bases[i].lotacao);
		novo->bases[i].espera = lista_cria(); //fila 
	}

	for(i=0; i < novo->n_missoes; i++){

		novo->missoes[i].id = i;
		novo->missoes[i].local.x = rand() % TAMANHO;
		novo->missoes[i].local.y = rand() % TAMANHO;
		novo->missoes[i].habilidades = cjto_aleat((rand() % 5) + 6, novo->n_habilidades);
		novo->missoes[i].perigo = rand() % 101;
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

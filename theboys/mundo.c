#include <stdio.h>
#include <stdlib.h>
#include "conjunto.h"
#include "lista.h"
#include "mundo.h"

#define INICIO 0
#define FIM 525600
#define HABILIDADES 10
#define TAMANHO 20000

struct mundo * cria_mundo(){

	struct mundo *novo;
	int i;

	if(!(novo = malloc(sizeof(struct mundo))))
		return NULL;

	novo->inicio = INICIO;
	novo->fim = FIM;
	novo->NHabilidades = HABILIDADES;
	novo->NHerois = novo->NHabilidades * 5;
	novo->NBases = novo->NHerois / 5;
	novo->NMissoes = novo->fim / 100;
	novo->maximos.x = TAMANHO;
	novo->maximos.y = TAMANHO;

	if(!(novo->Herois = malloc(sizeof(struct heroi) * novo->NHerois))){

		free(novo);
		return NULL;
	}

	if(!(novo->Bases = malloc(sizeof(struct base) * novo->NBases))){

		free(novo);
		return NULL;
	}

	if(!(novo->Missoes = malloc(sizeof(struct missao) * novo->NMissoes))){
		
		free(novo);
		return NULL;
	}


	for(i=0; i < novo->NHerois; i++){

		novo->Herois[i].id = i;
		novo->Herois[i].experiencia = 0;
		novo->Herois[i].paciencia = rand() % 101;
		novo->Herois[i].velocidade = (rand() % 4051) + 50;
		//novo->Herois[i].habilidades = cjto_aleat();//tem que ver isso ai
	}

	for(i=0; i < novo->NBases; i++){

		novo->Bases[i].id = i;
		novo->Bases[i].local.x = rand() % TAMANHO;
		novo->Bases[i].local.y = rand() % TAMANHO;
		novo->Bases[i].lotacao = (rand() % 8) + 3;
		novo->Bases[i].presentes = cjto_cria(novo->Bases[i].lotacao);
		novo->Bases[i].espera = lista_cria(); //fila 
	}

	for(i=0; i < novo->NMissoes; i++){

		novo->Missoes[i].id = i;
		novo->Missoes[i].local.x = rand() % TAMANHO;
		novo->Missoes[i].local.y = rand() % TAMANHO;
		//novo->Missoes[i].habilidades = cjto_aleat();
		novo->Missoes[i].perigo = rand() % 101;
	}

	return novo;
}

void imprime_heroi(struct heroi *h){
	
	printf("Heroi: \n");
	printf("ID: %d\n", h->id);
	printf("Paciencia: %d\n", h->paciencia);
	printf("Velocidade: %d\n", h->velocidade);
	printf("Experiencia: %d\n", h->experiencia);
	printf("ID da base: %d\n", h->id_base);
}

void imprime_base(struct base *b){
}

void imprime_missao(struct missao *ms){
}

void imprime_mundo(struct mundo *m){
	
	int i;

	if(m == NULL)
		return;

	printf("Mundo:\n");
	printf("Tempo inicial: %d\n", m->inicio);
	printf("Tempo final: %d\n", m->fim);
	printf("Tamanho do mundo: %d\n", m->maximos.x);
	printf("\n");
	printf("Herois: \n");
	
	for(i=0; i < m->NHerois; i++)
		imprime_heroi(m->Herois[i]);
	
}

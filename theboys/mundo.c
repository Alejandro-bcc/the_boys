#include <stdio.h>
#include <stdlib.h>
#include "conjunto.h"
#include "lista.h"
#include "mundo.h"

#define INICIO 0
#define FIM 1500
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
		novo->Herois[i].habilidades = cjto_aleat((rand() % 3 ) + 1, novo->NHabilidades);
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
		novo->Missoes[i].habilidades = cjto_aleat((rand() % 5) + 6, novo->NHabilidades);
		novo->Missoes[i].perigo = rand() % 101;
	}

	return novo;
}

struct mundo * destroi_mundo(struct mundo *m){
	
	int i;

	for(i=0; i < m->NHerois; i++)
		m->Herois[i].habilidades = cjto_destroi(m->Herois[i].habilidades);

	free(m->Herois);
	m->Herois = NULL;

	for(i=0; i < m->NBases; i++){
		
		m->Bases[i].presentes = cjto_destroi(m->Bases[i].presentes);
		m->Bases[i].espera = lista_destroi(m->Bases[i].espera);
	}

	free(m->Bases);
	m->Bases = NULL;

	for(i=0; i < m->NMissoes; i++)
		m->Missoes[i].habilidades = cjto_destroi(m->Missoes[i].habilidades);

	free(m->Missoes);
	m->Missoes = NULL;

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
	printf("NHerois: %d\n", m->NHerois);
	printf("NBases: %d\n", m->NBases);
	printf("NMissoes: %d\n", m->NMissoes);
	printf("\n");
	
	for(i=0; i < m->NHerois; i++){

		printf("%d ", i);
		imprime_heroi(m->Herois[i]);
	}

	for(i=0; i < m->NBases; i++){

		printf("%d ", i);
		imprime_base(m->Bases[i]);
	}

	for(i=0; i < m->NMissoes; i++){

		printf("%d ", i);
		imprime_missao(m->Missoes[i]);
	}
}

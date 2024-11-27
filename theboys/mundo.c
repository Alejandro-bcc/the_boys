#include <stdio.h>
#include <stdlib.h>
#include "conjunto.h"
#include "lista.h"

#define INICIO 0
#define FIM 525600
#define HABILIDADES 10
#define TAMANHO 2000

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

	if(!(novo->base = malloc(sizeof(struct base) * novo->NBases))){

		free(novo);
		return NULL;
	}


	for(i=0; i < novo->NHerois; i++){

		novo->Herois[i].id = i;
		novo->Herois[i].experiencia = 0;
		novo->Herois[i].paciencia = rand() % 101;
		novo->Herois[i].velocidade = (rand() % 4051) + 50;
		novo->Herois[i].habilidades = cjto_aleat(rand() % novo->NHabilidades +1, 2);//tem que ver isso ai
	}

	for(i=0; i < novo-<NBases; i++){

		novo->Bases[i].id = i;
		novo->Bases[i].local.x = rand() % novo->maximos.x;
		novo->Bases[i].local.y = rand() % novo->maximos.y;
		novo->Bases[i].lotacao = (rand() % 8) + 3;
		novo->Bases[i].presentes = cjto_cria(novo->Bases[i].lotacao);
		novo->Bases[i].espera = lista_cria(); //fila 
	}
}

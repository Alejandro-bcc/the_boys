#include <stdio.h>
#include <stdlib.h>
#include "conjunto.h"
#include "lista.h"

struct mundo * cria_mundo(){

	struct mundo *novo;
	int i;

	if(!(novo = malloc(sizeof(struct mundo))))
		return NULL;

	novo->inicio = 0;
	novo->fim = 525600;
	novo->NHabilidades = 10;
	novo->NHerois = novo->NHabilidades * 5;
	novo->NBases = novo->NHerois / 5;
	novo->NMissoes = novo->fim / 100;
	(novo->maximos).x = 50;
	(novo->maximos).y = 40;

	if(!(novo->Herois = malloc(sizeof(struct heroi) * novo->NHerois))){
		free(novo);
		return NULL;
	}


	for(i=0; i < novo->NHerois; i++){

		novo->Herois[i].id = i;
		novo->Herois[i].experiencia = 0;
		novo->Herois[i].paciencia = rand() % 101;
		novo->Herois[i].velocidade = (rand() % 4051) + 50;
		novo->Herois[i].habilidades = cjto_aleat(rand() % novo->NHabilidades +1, 2);
	}
}

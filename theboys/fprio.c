// TAD Fila de prioridades (FPRIO) genérica
// Implementação com lista encadeada simples
// Feita pelo aluno Alejandro Nava. GRR: 20242778

#include <stdlib.h>
#include <stdio.h>
#include "fprio.h"

struct fprio_t * fprio_cria(){

	struct fprio_t *f;

	if(!(f = malloc(sizeof(struct fprio_t))))
		return NULL;

	f->prim = NULL;
	f->num = 0;

	return f;
}

struct fprio_t * fprio_destroi(struct fprio_t *f){

	struct fpnodo_t *aux;
	
	/* se o ponteiro for inválido não faz nada */
	if(f == NULL)
		return NULL;

	while(f->prim != NULL){
		
		aux = f->prim;
		f->prim = aux->prox;
		free(aux->item);
		free(aux);
	}

	aux = NULL;
	free(f);

	return NULL;
}

int fprio_insere(struct fprio_t *f, void *item, int tipo, int prio){

    struct fpnodo_t *novo;
    struct fpnodo_t *aux;

    /* erro, ponteiros inválidos ou alocação mal-sucedida */
    if(f == NULL || item == NULL || !(novo = malloc(sizeof(struct fpnodo_t))))
        return -1;

    novo->item = item;
    novo->tipo = tipo;
    novo->prio = prio;

    /* a lista está vazia */
    if(f->prim == NULL){

        f->prim = novo;
        novo->prox = NULL;
        return ++f->num;
    }
	
	/* insere no começo */
    if(novo->prio < f->prim->prio){

        novo->prox = f->prim;
        f->prim = novo;
        return ++f->num;
    }
    
    aux = f->prim;
    while(aux->prox != NULL && aux->item != novo->item && novo->prio >= aux->prox->prio)
      aux = aux->prox;

	/* erro, ponteiro repetido */
	if(aux->item == novo->item){

		free(novo);
		return -1;
	}


	novo->prox = aux->prox;
	aux->prox = novo;

	return ++f->num;
}

void * fprio_retira(struct fprio_t *f, int *t, int *pr){

	struct fpnodo_t *aux;
	void *item;

	/* erro, ponteiros inválidos ou fila vazia */
	if(f == NULL || t == NULL || pr == NULL || f->prim == NULL)
		return NULL;
	
	aux = f->prim;
	item = aux->item;
	*t = aux->tipo;
	*pr = aux->prio;
	f->prim = f->prim->prox;
	free(aux);
	aux = NULL;
	f->num--;

	return item;
}

int fprio_tamanho(struct fprio_t *f){

	/* erro, ponteiro inválido */
	if(f == NULL)
		return -1;

	return f->num;
}

void fprio_imprime(struct fprio_t *f){

	struct fpnodo_t *aux;

	/* erro, ponteiro inválido */
	if(f == NULL)
		return;

	aux = f->prim;
	while(aux != NULL){
		
		printf("(%d %d)", aux->tipo, aux->prio);

		if(aux->prox != NULL)
			printf(" ");

		aux = aux->prox;
	}
}

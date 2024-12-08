// programa principal do projeto "The Boys - 2024/2"
// Autor: Alejandro David Nava Nava, GRR 20242778

#include <stdio.h>
#include <stdlib.h>
#include "mundo.h"
#include "eventos.h"

int main(){

	srand(0);

	struct mundo *w;
	struct fprio_t *lef;

	lef = fprio_cria();
	w = cria_mundo();
	cria_eventos_iniciais(lef, w);
	simulacao(lef, w);

	w = destroi_mundo(w);
	lef = fprio_destroi(lef);

	return 0;
}


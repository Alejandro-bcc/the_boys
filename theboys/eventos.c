#include "eventos.h"

void cria_eventos_inicias(struct fprio *e, struct mundo m){

	
}

void chega(struct heroi h, struct base b){

	int vagas, espera;

	h.id_base = b.id;

	vagas = b.lotacao - cjto_card(b.presentes);

	if(vagas > 0)
		espera = 1;
	else
		espera = h.paciencia > (10 * lista_tamanho(b.espera));

	if(espera)
		//insere evento ESPERA 
	else
		//evento DESISTE
}

void espera(struct heroi h, struct base b){

	lista_insere(b.espera, h.id, -1);
	//cria evento DESISTE
}

void desiste(struct mundo m, struct heroi h){

	int nova_base;

	nova_base = rand() % m.NBases;
	// cria evento VIAJA
}

#ifndef MUNDO
#define MUNDO

#include "conjunto.h"
#include "lista.h"

struct coordenadas{

	int x, y;
};

/* estrutura dos herois */
struct heroi{

	int id,
		paciencia,
		velocidade,
		experiencia,
		morto, // 1 se o heroi esta morto 0 se esta vivo
		id_base;

	struct cjto_t *habilidades;
};

/* estrutura das bases */
struct base{

	int id, lotacao;
	struct cjto_t *presentes;
	struct lista_t *espera;
	struct coordenadas local;
};

/* estrutura das missoes */
struct missao{

	int id, perigo;
	struct cjto_t *habilidades;
	struct coordenadas local;
};

/* estrutura do mundo */
struct mundo{

	int n_herois, n_bases, n_missoes, n_habilidades, inicio, fim;
	struct heroi *herois;
	struct base *bases;
	struct missao *missoes;
	struct coordenadas maximos;
};

/* funcao que cria o mundo e inicializa as entidades */
/* retorno: ponteiro para o mundo ou NULL em erro  */
struct mundo * cria_mundo();

/* destroi o mundo e todas suas entidades e libera a memoria */
/* retorno: NULL */
struct mundo * destroi_mundo(struct mundo *m);

/* calcula a distancia cartesiana entre o ponto de origem e destino  */
int calcula_distancia(struct coordenadas origem, struct coordenadas destino);

/* retorna o indice do menor elemento em um vetor de inteiros de tamanho n */
int min(int *v, int n);

void imprime_estatisticas(struct mundo *m);

/* imprime um heroi e todos seus elementos  */
void imprime_heroi(struct heroi h);

/* imprime uma base e todos seus elementos  */
void imprime_base(struct base b);

/* imprime uma missao e todos seus elementos  */
void imprime_missao(struct missao ms);

/* imprime o mundo e todos seus elementos  */
void imprime_mundo(struct mundo *m);

#endif

#ifndef MUNDO
#define MUNDO

#include "conjunto.h"
#include "lista.h"

/* estrutura com um identificador e um conteudo */
struct par{

	int id, cont;
};

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

	int id, perigo, tent;
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

/* Ordena um vetor de pares com n elementos */
/* com base nos conteudos de maneira crescente */
/* Usa o algoritmo de Insertion Sort */
void ordena_vetor_pares(struct par p[], int n);

/* Detecta se uma base eh apta para uma missao */
/* Ou seja, a uniao das habilidades dos herois da base */
/* cumpre com as habilidades requeridas pela missao */
/* Devolve o conjunto das habilidades dos herois da base */
/* no parametro "habs" */
/* Retorno: 1 se a base for apta, 0 se não for ou -1 em erro */
int base_apta(struct mundo *m ,struct missao *mis, struct base *b, struct cjto_t **habs);

/* Acha a base apta mais proxima à missao mis */
/* O ponteiro para a base achada e o conjunto das habilidades */
/* dos herois da base sao devolvidos nos parametros "b" e "habs" */
/* Retorno: 0 se não houver uma base apta, 1 se houver e -1 em erro */
int acha_base_apta(struct mundo *m, struct missao *mis, struct base *b, struct cjto_t **habs);
  
//void imprime_estatisticas(struct mundo *m);
  
/* imprime um heroi e todos seus elementos  */
void imprime_heroi(struct heroi h);

/* imprime uma base e todos seus elementos  */
void imprime_base(struct base b);

/* imprime uma missao e todos seus elementos  */
void imprime_missao(struct missao ms);

/* imprime o mundo e todos seus elementos  */
void imprime_mundo(struct mundo *m);

#endif

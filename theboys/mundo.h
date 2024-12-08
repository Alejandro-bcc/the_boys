#ifndef MUNDO
#define MUNDO

#include "conjunto.h"
#include "lista.h"

struct par{

	int id, cont; //identificador e conteudo
};

struct coordenadas{

	int x, y;
};

/* estrutura dos herois */
struct heroi{

	int id,
		pac,
		vel,
		exp,
		morto, // 1 se o heroi esta morto 0 se esta vivo
		id_base;

	struct cjto_t *habs;
};

/* estrutura das bases */
struct base{

	int id, 
		lot,
		fila_max,
		miss_part; //missoes participadas
	struct cjto_t *presentes;
	struct lista_t *espera; //fila de espera
	struct coordenadas local;
};

/* estrutura das missoes */
struct missao{

	int id, 
		perigo, 
		tent, 
		cump; //0 se a missao nao foi cumprida 1 se foi
	struct cjto_t *habs;
	struct coordenadas local;
};

/* estrutura do mundo */
struct mundo{

	int n_herois, n_bases, n_missoes, n_habs, n_eventos, inicio, fim;
	struct heroi *herois;
	struct base *bases;
	struct missao *missoes;
	struct coordenadas max;
};

/* Funcao que cria o mundo e inicializa todas as entidades */
/* Retorno: ponteiro para o mundo ou NULL em erro */
struct mundo * cria_mundo();

/* Destroi o mundo e todas suas entidades e libera a memoria */
/* retorno: NULL */
struct mundo * destroi_mundo(struct mundo *w);

/* calcula a distancia cartesiana entre o ponto de origem e destino  */
int calcula_distancia(struct coordenadas origem, struct coordenadas destino);

/* Ordena um vetor de pares com n elementos */
/* com base nos conteudos de maneira crescente */
/* Usa o algoritmo de Insertion Sort */
void ordena_vetor_pares(struct par p[], int n);

/* Determina se a base b eh apta para a missao m */
/* Ou seja, o conjunto das habilidades requeridas pela missao */
/* esta contido na uniao das habilidades de todos os herois da base */
/* Retorno: ponteiro para o conjunto da uniao das habilidades dos herois */
/* ou NULL se a base nao for apta ou erro */
struct cjto_t * base_apta(struct mundo *w, struct missao m, struct base b);

/* Acha a base apta mais proxima à missao m */
/* Devolve o ID da base achada no parametro "b_id" */
/* Retorno: ponteiro para o conjunto da uniao das habilidades dos herois */
/* da base achada ou NULL se nao houver ou erro */
/* ou NULL se nao houver ou erro */
struct cjto_t * acha_base_apta(struct mundo *w, struct missao m, int *b_id);

/* Imprime as estatisticas do mundo w*/
/* junto com informacoes do herois e das bases */
void imprime_estatisticas(struct mundo *w);

#endif

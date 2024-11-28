#include "conjunto.h"
#include "lista.h"
#include "fprio.h"

struct coordenadas{

	int x, y;
};

/* estrutura dos herois */
struct heroi{

	int id,
		paciencia,
		velocidade,
		experiencia,
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

	int NHerois, NBases, NMissoes, NHabilidades, inicio, fim;
	struct heroi *Herois;
	struct base *Bases;
	struct missao *Missoes;
	struct coordenadas maximos;
};

/* funcao que cria o mundo e inicializa as entidades */
/* retorno: ponteiro para o mundo ou NULL em erro  */
struct mundo * cria_mundo();

/* imprime um heroi e todos seus elementos  */
void imprime_heroi(struct heroi *h);

/* imprime uma base e todos seus elementos  */
void imprime_base(struct base *b);

/* imprime uma missao e todos seus elementos  */
void imprime_missao(struct missao *ms);

/* imprime o mundo e todos seus elementos  */
void imprime_mundo(struct mundo *m);

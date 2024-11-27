
#include <conjunto.h>
#include <lista.h>
#include <fprio.h>

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

	struct cjto_t habilidades;
};

/* estrutura das bases */
struct base{

	int id, lotacao;
	struct cjto presentes;
	struct lista_t espera;
	struct coordenadas local;
};

/* estrutura das missoes */
struct missao{

	int id, perigo;
	struct cjto_t habilidades;
	struct coordenadas local;
};

/* estrutura do mundo */
struct mundo{

	int NHerois, NBases, NMissoes, NHabilidades, relogio, fim;
	struct heroi *Herois;
	struct base *Bases;
	struct missao *Missoes;
	struct coordenadas maximos;
};

/* funcao que cria o mundo e inicializa as entidades */
int cria_mundo(struct mundo *novo);



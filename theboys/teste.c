#include "mundo.h"
#include "eventos.h"

int main(){
	
	struct mundo *m;
	struct fprio_t *lef;
	lef = malloc(sizeof(struct fprio_t));
	m = cria_mundo();
	imprime_mundo(m);
	cria_eventos_iniciais(lef, m);
	simulacao(lef);
	m = destroi_mundo(m);

	return 0;
}

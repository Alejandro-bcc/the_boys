#include <stdlib.h>
#include "mundo.h"
#include "eventos.h"

int main(){
	
	srand(0);
	struct mundo *m;
	struct fprio_t *lef;
	lef = fprio_cria();
	m = cria_mundo();
	cria_eventos_iniciais(lef, m);
	simulacao(lef, m);
	m = destroi_mundo(m);
	lef = fprio_destroi(lef);
	return 0;
}

#include "mundo.h"

int main(){
	
	struct mundo *m;
	m = cria_mundo();
	imprime_mundo(m);
	m = destroi_mundo(m);

	return 0;
}

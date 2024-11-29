#include <stdio.h>
#include <stdlib.h>
#include "conjunto.h"
#include "lista.h"
#include "fprio.h"
#include "mundo.h"



int main(){
	
	struct mundo *m;
	m = cria_mundo();
	imprime_mundo(m);
	m = destroi_mundo(m);

	return 0;
}

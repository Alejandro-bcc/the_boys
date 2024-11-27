int cria_mundo(struct mundo *novo){

	if(!(malloc(sizeof(struct mundo))))
		return NULL;

	novo->relogio = 0;
	novo->fim = 525600;
}

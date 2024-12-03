#include <stdio.h>
#include <stdlib.h>
#include "liblef.h"

void imprime_nos(nodo_lef_t *no){
    if(!no)
        return;

    printf("%d ", no->evento->tempo);
    imprime_nos(no->prox);
}

void imprime_lef(lef_t *l){
    if(!l)
        return;

    imprime_nos(l->Primeiro);
    printf("\n");
}

int main(){
    int n;
    lef_t *l = cria_lef();
    evento_t evento;
    evento_t *ev;

    evento.dado1 = 0;
    evento.dado2 = 0;
    evento.tipo = 0;
    printf("Opcoes:\n 1-Evento Ordem.\n 2-Evento inicio.\n 3-Tira primeiro no.\n 4-imprime\n 0-Sair\n");
    scanf("%d", &n);

    while (1){
        printf("Tempo do evento:\n");
        switch (n){
            case 1:
                scanf("%d", &evento.tempo);
                adiciona_ordem_lef(l, &evento);
                imprime_lef(l);
                break;
            case 2:
                scanf("%d", &evento.tempo);
                adiciona_inicio_lef(l, &evento);
                imprime_lef(l);
                break;
            case 3:
                ev = obtem_primeiro_lef(l);
                if (ev != NULL){
                    printf ("%d\n", ev->tempo);
                    free(ev);
                }
                break;
            case 4:
                printf ("\n");
                imprime_lef(l);
                printf ("\n");
                break;
            case 0:
                destroi_lef(l);
                return 0;
                break;
            default:
                break;
        }
        printf("Opcoes:\n 1-Evento Ordem.\n 2-Evento inicio.\n 3-Tira primeiro no.\n 4-imprime\n 0-Sair\n");
        scanf("%d", &n);
    }

    return 0;
}

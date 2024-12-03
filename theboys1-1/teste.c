#include <stdio.h>
#include <stdlib.h>
#include "liblef.h"
#include "libconjunto.h"
#include "libfila.h"
#include "math.h"

#define FIMTempo 34944
#define Chegar 0
#define Sair 1
#define Missao 2
#define FIM 3

int max(int x, int y){
    if(x > y){
        return x;
    }else{
        return y;
    }
}

struct Heroi{
    int id;
    int paciencia;
    int idade;
    int exp;
    conjunto_t *CH;
};typedef struct Heroi H;

struct Local{
    int id;
    int maxlocal;
    conjunto_t *idherois;
    fila_t *fila;
    int xy[2];
};typedef struct Local L;

struct Mundo{
    int TempoAt;
    H *Herois;/*dois astericas*/
    L *Locais;
    int NH;
    int NL;
    int NTM;    
    int *Hab;/* mudar pra cjt*/
};typedef struct Mundo m;

int sortear(int a, int b){
    return rand() % (a - b + 1) + a;
}

int sortearHab(H* h, m* m){
    int QuantHab = sortear(2, 5);
    h->CH = cria_cjt (QuantHab);
    int i = sortear(0,9);/*9 oou 10?*/
    while(insere_cjt(h->CH, *(m->Hab+i)))
        i = sortear(0,9);
}

int CriarHeroi(m* m, int id){
    (m->Herois + id)->id = id;
    (m->Herois + id)->paciencia = sortear(0, 100);
    (m->Herois + id)->idade = sortear(18, 100);
    (m->Herois + id)->exp = 0;    
    sortearHab((m->Herois + id), m);
}

int CriarLocal(m* m, int id){
    (m->Locais + id)->id = id;
    (m->Locais + id)->maxlocal = sortear(5, 30);
    (m->Locais + id)->idherois = cria_cjt ((m->Locais + id)->maxlocal);
    (m->Locais + id)->fila = cria_fila ();
    (m->Locais + id)->xy[0] = sortear(0, m->NTM - 1);
    (m->Locais + id)->xy[1] = sortear(0, m->NTM - 1);
}

int CriarMundo(m* m){
    int i;
    int nhab = 10;
    m->TempoAt = 0;
    m->NH = nhab*5;
    m->NL = m->NH / 6;

    m->Hab = malloc(sizeof(int)*nhab);
    for(i=0;i<nhab;i++)
       *(m->Hab+i) = sortear(0, 20);
    
    m->NTM = 2000;

    m->Herois = malloc(sizeof(H)*m->NH);/*testar erro no malloc*/
    for(i=0;i<m->NH;i++)
        CriarHeroi(m,i);

    m->Locais = malloc(sizeof(L)*m->NL);
    for(i=0;i<m->NL;i++)
        CriarLocal(m,i);
}

int Chegada(m* m, int idH, int idL, lef_t* LE){
    evento_t evento;
    if(cardinalidade_cjt((m->Locais + idL)->idherois) >= (m->Locais + idL)->maxlocal)/*Se o local esta lotado*/
        if((m->Herois + idH)->paciencia/4 - tamanho_fila((m->Locais + idL)->fila) > 0)/*Se o herói entra na fila*/
            insere_fila((m->Locais + idL)->fila, idH);
        else{/*Se o herói não entra na fila, cria um evento de saida*/
            evento.tempo = m->TempoAt;
            evento.tipo = Sair;
            evento.dado1 = idH;
            evento.dado2 = idL;
            adiciona_ordem_lef(LE, &evento);
            }
    else{/*Se o local não esta lotado, calcula o tempo permanencia no local e cria um evento de saida*/
            insere_cjt((m->Locais + idL)->idherois, idH);/*n pediu ?*/
            int TPL = max(1, (m->Herois + idH)->paciencia/10 + sortear(-2,6));
            evento.tempo = m->TempoAt + TPL;
            evento.tipo = Sair;
            evento.dado1 = idH;
            evento.dado2 = idL;
            adiciona_ordem_lef(LE, &evento);
    }
}

int Saida(m* m, int idH, int idL, lef_t* LE){
    evento_t evento;
    int Heroi_ini_fila;
    int novo_local = sortear(0, m->NL -1);
    int dist_cartesiana = sqrt( power((m->Locais + idL)->xy[0] - (m->Locais + novo_local)->xy[0], 2) + power((m->Locais + idL)->xy[0] - (m->Locais + novo_local)->xy[0], 2) );
    int TDL = 100-max(0,(m->Herois+idH)->idade - 40)/dist_cartesiana;
    if(!vazia_fila((m->Locais + idL)->fila)){/*Se a fila do local na está fazia, o heroi no incio da fila entra*/
        retira_fila((m->Locais + idL)->fila, &Heroi_ini_fila);
        evento.tempo = m->TempoAt;
        evento.tipo = Chegar;
        evento.dado1 = Heroi_ini_fila;
        evento.dado2 = idL;
        adiciona_ordem_lef(LE, &evento);
        
        evento.tempo = m->TempoAt;
        evento.tipo = Chegar;
        evento.dado1 = Heroi_ini_fila;
        evento.dado2 = idL;
        adiciona_inicio(LE, &evento);
    }
        evento.tempo = m->TempoAt + TDL/15;
        evento.tipo = Chegar;
        evento.dado1 = idH;
        evento.dado2 = novo_local ;
        adiciona_ordem_lef(LE, &evento);
}

int Missao(m* m){
    int tam = sortear(3,6);
    conjunto_t* missao = cria_cjt(tam);
    for(int i = 0; i < cardinalidade_cjt(missao); i++)
        insere_cjt
}

int main(){
    m* Mundo = malloc(sizeof(m));
    lef_t* ListaEventos = cria_lef();
    CriarMundo(Mundo);
    int i;
    /*for(i=0;i<Mundo->NH;i++){
        printf("%d ",(Mundo->Herois+i)->id);
        printf("%d ",(Mundo->Herois+i)->paciencia);
        printf("%d ",(Mundo->Herois+i)->idade);
        printf("%d \n",(Mundo->Herois+i)->exp);
    }*/
    Chegada(Mundo, 0, 0, ListaEventos);
	return 0;
}

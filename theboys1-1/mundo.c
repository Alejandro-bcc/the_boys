#include <stdio.h>
#include <stdlib.h>
#include "liblef.h"
#include "libconjunto.h"
#include "libfila.h"
#include <math.h>
#include <time.h>

#define FIMTEMPO 34944
#define CHEGADA 0
#define SAIDA 1
#define MISSAO 2
#define FIM 3

struct Heroi{
    int id;
    int paciencia;
    int idade;
    int EXP;
    conjunto_t *Habilidades;
};typedef struct Heroi H;

struct Local{
    int id;
    int Max_Local;
    conjunto_t *Publico;
    fila_t *fila;
    int xy[2];
};typedef struct Local L;

struct Mundo{
    int TempoAt;
    int Num_Herois;
    int Num_Locais;
    int Tam_Mundo;
    int Num_Hab : 10; 
    conjunto_t *Hablilidades_all;
    H Herois[10*5];
    L Locais[10*5/6];
};typedef struct Mundo M;

int max(int x, int y){
    if(x > y){
        return x;
    }else{
        return y;
    }
}

int sortear(int a, int b){
    return rand() % (b+1-a) + a;
}

void sortearHab(H Herois[], M* m){
    int QuantHab = sortear(2, 5);
    Herois->Habilidades = cria_subcjt_cjt(m->Hablilidades_all, QuantHab);
}
       
void CriarHeroi(M* m, int id, lef_t* LE){
    (m->Herois[id]).id = id;
    (m->Herois[id]).paciencia = sortear(0, 100);
    (m->Herois[id]).idade = sortear(18, 100);
    (m->Herois[id]).EXP = 0;    
    sortearHab(&m->Herois[id], m);
    
    evento_t evento;
    evento.tempo = sortear(0,96*7);/*96*7?*/
    evento.tipo = CHEGADA;
    evento.dado1 = id;
    evento.dado2 = sortear(0, m->Num_Locais-1);
    adiciona_ordem_lef(LE, &evento);
}

void CriarLocal(M* m, int id){
    (m->Locais[id]).id = id;
    (m->Locais[id]).Max_Local = sortear(5, 30);
    (m->Locais[id]).Publico = cria_cjt ((m->Locais[id]).Max_Local);
    (m->Locais[id]).fila = cria_fila();
    (m->Locais[id]).xy[0] = sortear(0, m->Tam_Mundo - 1);
    (m->Locais[id]).xy[1] = sortear(0, m->Tam_Mundo - 1);
}

M* CriarMundo(lef_t* lef){
    int i;
    M* m = malloc(sizeof(M));
    m->TempoAt = 0;
    m->Num_Herois = m->Num_Hab*5;
    m->Num_Locais = m->Num_Herois/6;
    m->Tam_Mundo = 2000;
    m->Hablilidades_all = cria_cjt(m->Num_Hab);

    for(i=0;i<m->Num_Hab;i++)
       insere_cjt(m->Hablilidades_all, sortear(0, m->Num_Hab - 1));    

    for(i = 0;i < m->Num_Herois; i++)
        CriarHeroi(m,i,lef);

    for(i = 0; i < m->Num_Locais; i++)
        CriarLocal(m,i);

    return m;
}


void Chegada(M* m, int idH, int idL, lef_t* LE){
    evento_t evento;
    
    printf("%d: CHEGA HEROI %d Local %d (%d/%d), ", m->TempoAt, idH, idL, cardinalidade_cjt((m->Locais[idL]).Publico), (m->Locais[idL]).Max_Local);

    if(cardinalidade_cjt((m->Locais[idL]).Publico) >= (m->Locais[idL]).Max_Local)/*Se o local esta lotado*/
        if((m->Herois[idH]).paciencia/4 - tamanho_fila((m->Locais[idL]).fila) > 0){/*Se o herói entra na fila*/
            insere_fila((m->Locais[idL]).fila, idH);
            printf("FILA %d\n", tamanho_fila((m->Locais[idL]).fila));
        }
        else{/*Se o herói não entra na fila, cria um evento de saida*/
            printf("DESISTE\n");
            evento.tempo = m->TempoAt;
            evento.tipo = SAIDA;
            evento.dado1 = idH;
            evento.dado2 = idL;
            adiciona_ordem_lef(LE, &evento);
            }
    else{/*Se o local não esta lotado, calcula o tempo permanencia no local e cria um evento de saida*/
            printf("ENTRA\n");
            insere_cjt((m->Locais[idL]).Publico, idH);
            int TPL = max(1, m->Herois[idH].paciencia/10 + sortear(-2,6));
            evento.tempo = m->TempoAt + TPL;
            evento.tipo = SAIDA;
            evento.dado1 = idH;
            evento.dado2 = idL;
            adiciona_ordem_lef(LE, &evento);
    }
}

void Saida(M* m, int idH, int idL, lef_t* LE){
    evento_t evento;
    int Heroi_ini_fila;
    int novo_local = sortear(0, m->Num_Locais - 1);
    int dist_cartesiana = sqrt( pow((m->Locais[idL]).xy[0] - (m->Locais[novo_local]).xy[0], 2) + pow((m->Locais[idL]).xy[1] - (m->Locais[novo_local]).xy[1], 2) );
    int TDL = dist_cartesiana/(100-max(0, m->Herois[idH].idade - 40));

    printf("%d: SAIDA HEROI %d Local %d (%d/%d)", m->TempoAt, idH, idL, cardinalidade_cjt((m->Locais[idL]).Publico), (m->Locais[idL]).Max_Local);

    if(!vazia_fila((m->Locais[idL]).fila) && pertence_cjt((m->Locais[idL]).Publico, idH)){/*Se a fila do local não está fazia, e o herói idH estava no local, o heroi no incio da fila  cria um evento para entrar no local*/
        
        retira_cjt((m->Locais[idL]).Publico, idH);/*Retira o Heroi idH do local, apenas se ele estava la dentro*/
        retira_fila((m->Locais[idL]).fila, &Heroi_ini_fila);
        printf(", REMOVE FILA HEROI %d", Heroi_ini_fila);
        evento.tempo = m->TempoAt;
        evento.tipo = CHEGADA;
        evento.dado1 = Heroi_ini_fila;
        evento.dado2 = idL;
        adiciona_inicio_lef(LE, &evento);
    }/*Cria um novo evento de chegada para o herói que saiu*/
        evento.tempo = m->TempoAt + TDL/15;
        evento.tipo = CHEGADA;
        evento.dado1 = idH;
        evento.dado2 = novo_local;
        adiciona_ordem_lef(LE, &evento);

        printf("\n");
}

int testar_equipes(M* m, conjunto_t *missao, int idmissao){
	int i;
	int num_equipe = -1;
	int menorcard = m->Num_Herois;
	int id, poder;
	for(i = 0; i < m->Num_Locais - 1; i++){/*Analisa cada local do mundo*/
		conjunto_t* uniao = cria_cjt(10);
		inicia_iterador_cjt((m->Locais[i]).Publico);
        while(incrementa_iterador_cjt((m->Locais[i]).Publico, &id)){/*Olha cada heroi de um local i*/
            inicia_iterador_cjt ((m->Herois[id]).Habilidades);
            while(incrementa_iterador_cjt((m->Herois[id]).Habilidades, &poder))/*Olha cada poder de cada heroi no local i de insere na uniao*/
                insere_cjt(uniao, poder);
		}

        printf("%d: MISSAO %d HAB_EQL %d:", m->TempoAt, idmissao, i);
        imprime_cjt(uniao);

		if(contido_cjt(missao, uniao) && menorcard > cardinalidade_cjt((m->Locais[i]).Publico)){/*Se a uniao possui as habilidades requeridas e tem card menor que a da uniao interior, o id desse local é o novo num_equipe*/
			num_equipe = i;
			menorcard = cardinalidade_cjt((m->Locais[i]).Publico);
		}

		uniao = destroi_cjt(uniao);
	}

    printf("%d: MISSAO %d ", m->TempoAt, idmissao);
    if(num_equipe!= -1){
        printf("HAB_EQS %d:", num_equipe);
        imprime_cjt((m->Locais[num_equipe]).Publico);
    }else{
        printf("IMPOSSIVEL\n");
    }

	return num_equipe;
}

void Missao(M* m, int idmissao, lef_t* LE){
    int id;
    int id_local = -1;

    conjunto_t* missao = cria_subcjt_cjt(m->Hablilidades_all, sortear(3,6));
    printf("%d: MISSAO %d HAB_REQ ", m->TempoAt, idmissao);
    imprime_cjt(missao);
    id_local = testar_equipes(m, missao, idmissao);
    destroi_cjt(missao);

    if(id_local != -1){
    inicia_iterador_cjt((m->Locais[id_local]).Publico);
	while(incrementa_iterador_cjt((m->Locais[id_local]).Publico, &id))/*Atualiza EXP dos herois da equipe*/
        (m->Herois[id]).EXP++;
    }else{/*Se a a missão é impossível recria a missao idmissao em outro tempo*/
        evento_t evento;
        evento.tempo = sortear(m->TempoAt, FIMTEMPO);
        evento.tipo = MISSAO;
        evento.dado1 = idmissao;
        adiciona_ordem_lef(LE, &evento);
    }
}

void CriarMissoes(M* m, lef_t* LE){
    int i;
    evento_t evento;
    for(i = 0; i < FIMTEMPO/100; i++){
        evento.tempo = sortear(0, FIMTEMPO);
        evento.tipo = MISSAO;
        evento.dado1 = i;
        adiciona_ordem_lef(LE, &evento);
    }
}
void DefinirFim(lef_t* LE){
    evento_t evento;
    evento.tempo = FIMTEMPO;
    evento.tipo = FIM;
    adiciona_ordem_lef(LE, &evento);
}
void DestruirMundo(M* m, lef_t* lef){
    int i;
    destroi_lef(lef);
    for(i = 0; i < m->Num_Locais; i++)
        destroi_fila((m->Locais[i]).fila);
    for(i = 0; i < m->Num_Locais; i++)
        destroi_cjt((m->Locais[i]).Publico);
    for(i = 0; i < m->Num_Herois; i++)
        destroi_cjt((m->Herois[i]).Habilidades);
    destroi_cjt(m->Hablilidades_all);
    free(m);
}

int main(){
    srand(3);
    int i; 
    evento_t *Prime_Evento;

    lef_t* ListaEventos = cria_lef();
    M* Mundo = CriarMundo(ListaEventos);   
    CriarMissoes(Mundo, ListaEventos);
    DefinirFim(ListaEventos);

    while(Mundo->TempoAt != FIMTEMPO){
        Prime_Evento = obtem_primeiro_lef(ListaEventos);
        Mundo->TempoAt = Prime_Evento->tempo;
        switch(Prime_Evento->tipo){
            case CHEGADA:
                Chegada(Mundo, Prime_Evento->dado1, Prime_Evento->dado2, ListaEventos);
            break;
            case SAIDA:
                Saida(Mundo, Prime_Evento->dado1, Prime_Evento->dado2, ListaEventos);
            break;
            case MISSAO:
                Missao(Mundo, Prime_Evento->dado1, ListaEventos);
            break;
            case FIM:
                printf("%d: FIM\n", FIMTEMPO);
                for(i = 0; i < Mundo->Num_Herois; i++)
                    printf("HEROI %d EXPERIENCIA %d\n", i, (Mundo->Herois[i]).EXP );
            break;
        }    
        free(Prime_Evento);
    }

    DestruirMundo(Mundo, ListaEventos);
	return 0;
}

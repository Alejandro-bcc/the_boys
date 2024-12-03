#include <stdio.h>
#include <math.h>

void IgualaVetores(int v[], int u[], int a, int b){
    int i;
    for(i=0; i<=b-a; i++){
        v[i+a] = u[i];
    }
}

int BuscaSequencial(int x, int v[], int a, int b){
	if (a > b)
		return a-1;
	if (x >= v[b])
		return b;
	return BuscaSequencial(x, v, a, b-1);
}

int BuscaBinaria(int x, int v[], int a, int b){
	if (a > b)
		return a-1;
	int m = (a + b)/ 2 ;
	if (x == v[m])
		return m;
	if (x < v[m])
		return BuscaBinaria(x, v, a, m-1);
	return BuscaBinaria(x, v, m+1, b);
}

int Intercala(int v[], int a, int m, int b){
	int k;
    int u[b+1];
	if (a >= b)
		return *v;
	int i = a;
	int j = m + 1;
	for(k = 0; k <= b-a; k++){
		if((j > b) || ((i<=m) && (v[i] <= v[j]))){
			u[k] = v[i];
            i++;
		}
        else
        {
			u[k] = v[j];
			j++;
		}
	}
	IgualaVetores(v, u, a, b);
	return *v;
}

int OrdenaM(int v[], int a, int b){
	if(a>=b)
        return *v;
	int m = (a+b)/2;
	OrdenaM(v, a, m);
	OrdenaM(v, m+1, b);
	return Intercala(v, a, m, b);
    return *v;
}

void Troca(int v[], int a, int b){
	int x = v[a];
	v[a] = v[b];
	v[b] = x;
}

int Minimo(int v[], int a, int b){
	if (a == b)
		return a;
	int m = Minimo(v, a, b-1);
	if (v[b] < v[m])
		m = b;
	return m;
}

int OrdenaS(int v[], int a, int b){
	if (a >= b)
		return *v;
	Troca(v, a, Minimo(v, a, b));
	return OrdenaS(v, a+1, b);
}


int Insere(int v[], int a, int b){
	int p = BuscaBinaria(v[b], v, a, b-1);
	int i = b;
	while(i > p + 1){
		Troca(v, i, i-1);
		i--;
	}
	return *v;
}

int Ordenai(int v[], int a, int b){
	if(a >= b)
		return *v;
	Ordenai(v, a, b-1);
	Insere(v, a, b);
	return *v;
}

int Part(int v[], int a, int b, int x){
    int m = a-1;
    int i;
    for(i=a; i<=b; i++)
        if(v[i] <=x){
            m++;
            Troca(v, m, i);
        }
    return m;
}

int Quick(int v[], int a, int b){
    if (a >= b)
        return *v;
    int m = Part(v, a, b, v[b]);
    Quick(v, a, m-1);
    Quick(v, m+1, b);
}

int main(){
    int i;
    int tam = 7;
    int v[7] = {2, 1, 3, 7, 4, 5, 8};
    Quick(v, 0, tam-1);
    for(i =0; i<tam; i++){
        printf("%d ", v[i]);
    }
    printf("%d ", BuscaSequencial(v[7], v, 0, 6));
    return 0;
}

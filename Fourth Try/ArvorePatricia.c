#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

#define MAX 5;

typedef struct no{
    unsigned chave;
    struct no *esq, *dir;
    int bit;
}no;

no* criaNo(){
    no *novo = malloc(sizeof(no));
    novo->esq = novo->dir = novo;

    return novo;
}

void inicializar(no **arvore){
    (*arvore) = criaNo();

    (*arvore)->bit = -1;
    (*arvore)->chave = UINT_MAX;
}

int bit(unsigned chave, int nivel){
    return (chave >> (MAX - 1 - nivel) & 1);
}

no* buscaRec(no *arvore, unsigned chave, int nivel){
}

no* busca(no *arvore, unsigned chave){
    no *t = buscaRec(arvore->esq, chave, -1);

    return t->chave == chave ? t : NULL;
}

int main(){

    return 0;
}


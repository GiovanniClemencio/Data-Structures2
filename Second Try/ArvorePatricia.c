#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

#define MAX_BIT 5

typedef struct no{
    unsigned chave;
    int bit;
    struct no *esq, *dir;
}no;

no* criarNo(){
    no *novo = malloc(sizeof(no));
    novo->esq = novo->dir = novo;

    return novo;
}

int bit(unsigned chave, int nivel){
    return chave >> (MAX_BIT - 1 - nivel) & 1;
}

void inicializar(no **arvore){
    *arvore = criarNo();
    (*arvore)->bit = -1;
}

no* buscaRec(no *arvore, unsigned elemento, int nivel){
    if(arvore->nivel <= nivel) return arvore;

    if(bit(elemento, nivel) == 0){
        return buscaRec(arvore->esq, elemento, arvore->bit);
    }else{
        return buscaRec(arvore->dir, elemento, arvore->bit);
    }
}

no *busca(no *raiz, unsigned chave){
    no *encontrado = buscaRec(raiz->esq, chave, -1); // Chamamos raiz->esq para não buscarmos no nó dummy

    return encontrado->chave == chave ? encontrado : NULL; // Sintaxe do ternario: condição ? se_condição_vdd : se_condição_falso;
}

no* insereRec(no *arvore, unsigned elemento, int nivel, no* pai){
    no *novo;
    if(arvore->bit >= nivel || arvore->bit <= pai->bit){
        novo = criarNo();
        novo->bit = nivel;
        novo->chave = elemento;

        if(bit(novo->chave, novo->bit) == 0){
            novo->esq = novo;
            novo->dir = arvore;
        }else{
            novo->esq = arvore;
            novo->dir = novo;
        }
        return novo;
    }

    if(bit(elemento, arvore->nivel) == 0){
        arvore->esq = insereRec(arvore->esq, elemento, nivel, arvore);
    }else{
        arvore->dir = insereRec(arvore->dir, elemento, nivel, arvore);
    }

    return arvore;
}

void inserir(no **raiz, unsigned chave){
    int i;
    no *t = buscaRec((*raiz)->esq, chave, -1);

    if(t->chave == chave) return;
    for(i = 0; bit(chave, i) == bit(t->chave, i); i++);

    (*raiz)->esq = insereRec((*raiz)->esq, chave, int i, *raiz);
}

// -=-=-==-=-=-=-=-=-=- Utilities
void limpar(no *arvore){
    if(arvore->esq->bit > arvore->bit) limpar(arvore->esq);
    if(arvore->dir->bit > arvore->bit) limpar(arvore->dir);

    free(arvore);
}

int main(){
    no *raiz;

    inicializar(&raiz);

    printf("%d \n", raiz->bit);

    limpar(raiz);
    return 0;
}

#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

#define MAX_BITS 5

typedef struct no{
    unsigned chave;
    int nivel;
    struct no *esq, *dir;
}no;

no* criaNo(){
    no *novo = malloc(sizeof(no));
    novo->esq = novo->dir = novo;

    return novo;
}

int bit(unsigned chave, int nivel){
    return (chave >> (MAX_BITS - 1 - nivel)) & 1;
}

void inicializar(no **arvore){
    *arvore = criaNo();

    (*arvore)->chave = UINT_MAX;
    (*arvore)->nivel = -1;
}

no* buscaRec(no *arvore, unsigned chave, int nivel){
    if(arvore->nivel <= nivel) return arvore;

    if(bit(chave, arvore->nivel) == 0){
        return buscaRec(arvore->esq, chave, arvore->nivel);
    }else{
        return buscaRec(arvore->dir, chave, arvore->nivel);
    }
}

no* busca(no *arvore, unsigned chave){
    no *t = buscaRec(arvore->esq, chave, -1);

    return t->chave == chave ? t : NULL;
}

no* insereRec(no *arvore, unsigned elemento, int nivel, no *pai){
    no *novo;

    if(nivel <= arvore->nivel || arvore->nivel <= pai->nivel){
         novo = criaNo();
         novo->nivel = nivel;
         novo->chave = elemento;

         if(bit(elemento, nivel) == 0){
            novo->dir = arvore;
         }else{
             novo->esq = arvore;
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

void insere(no **arvore, unsigned chave){
    no *t = buscaRec((*arvore)->esq, chave, -1);

    if(t->chave == chave) return;

    int i = 0;
    while(bit(t->chave, i) == bit(chave, i)) i++;

    (*arvore)->esq = insereRec((*arvore)->esq, chave, i, *arvore);
}

void limparRec(no *arvore){
    if(arvore->esq->nivel > arvore->nivel) limparRec(arvore->esq);
    if(arvore->dir->nivel > arvore->nivel) limparRec(arvore->dir);

    free(arvore);
}

void imprimeRec(no *arvore, int profundidade){
    if(arvore == NULL) return;

    for(int i = 0; i < profundidade; i++){
        printf("  ");
    }
    printf("Chave: %u, Nivel: %d\n", arvore->chave, arvore->nivel);

    printf("{");
    if(arvore->esq != NULL && arvore->esq->nivel > arvore->nivel){
        imprimeRec(arvore->esq, profundidade + 1);
    }
    if(arvore->dir != NULL && arvore->dir->nivel > arvore->nivel){
        imprimeRec(arvore->dir, profundidade + 1);
    }
    printf("}");
}

void imprime(no *arvore){
    printf("Estrutura da arvore:\n");
    imprimeRec(arvore, 0);
    printf("\n");
}

int main(){
    no *raiz;
    inicializar(&raiz);

    no *leitor;

    printf("\n- - - Inserindo o 9 - - -\n");
    insere(&raiz, 9);
    imprime(raiz);

    printf("\n- - - Inserindo o 3 - - -\n");
    insere(&raiz, 3);
    imprime(raiz);

    printf("\n- - - Inserindo o 15 - - -\n");
    insere(&raiz, 15);
    imprime(raiz);

    printf("\n- - - Inserindo o 25 - - -\n");
    insere(&raiz, 25);
    imprime(raiz);

    printf("\n- - - Inserindo o 8 - - -\n");
    insere(&raiz, 8);
    imprime(raiz);

    leitor = busca(raiz, 8);

    printf("Esquerda do 8: %d\n Direita do 8: %d ", leitor->esq->chave, leitor->dir->chave);

    printf("\n- - - Inserindo o 1 - - -\n");
    insere(&raiz, 1);
    imprime(raiz);

    leitor = busca(raiz, 3);

    printf("Esquerda do 3: %d\n Direita do 3: %d ", leitor->esq->chave, leitor->dir->chave);

    leitor = busca(raiz, 1);

    printf("Esquerda do 1: %d\n Direita do 1: %d ", leitor->esq->chave, leitor->dir->chave);

    printf("\n- - - Inserindo o 30 - - -\n");
    insere(&raiz, 30);
    imprime(raiz);

    leitor = busca(raiz, 25);

    printf("Esquerda do 25: %d\n Direita do 25: %d \n", leitor->esq->chave, leitor->dir->chave);

    leitor = busca(raiz, 30);

    printf("Esquerda do 30: %d\n Direita do 30: %d \n", leitor->esq->chave, leitor->dir->chave);

    printf("\n- - - Inserindo o 16 - - -\n");
    insere(&raiz, 16);
    imprime(raiz);

    leitor = busca(raiz, 9);

    printf("Esquerda do 9: %d\n Direita do 9: %d \n", leitor->esq->chave, leitor->dir->chave);

    leitor = busca(raiz, 16);

    printf("Esquerda do 16: %d\n Direita do 16: %d \n", leitor->esq->chave, leitor->dir->chave);

    printf("\n- - - Inserindo o 4 (final) - - -\n");
    insere(&raiz, 4);
    imprime(raiz);


    limparRec(raiz);
    return 0;
}

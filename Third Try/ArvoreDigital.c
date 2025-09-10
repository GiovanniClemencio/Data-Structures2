#include<stdio.h>
#include<stdlib.h>

#define MAX_BITS 5

typedef struct no{
    unsigned chave;
    struct no *esq, *dir;
}no;

no* criarNo(){
    no* novo = malloc(sizeof(no));
    novo->esq = novo->dir = NULL;

    return novo;
}

int bit(unsigned chave, int nivel){
    return chave >> (MAX_BITS - 1 - nivel) & 1;
}

no* buscaRec(no *raiz, unsigned elemento, int nivel){
    if(raiz == NULL) return NULL;
    if(raiz->chave == elemento) return raiz;

    if(bit(elemento, nivel) == 0){
        return buscaRec(raiz->esq, elemento, nivel + 1);
    }else{
        return buscaRec(raiz->dir, elemento, nivel + 1);
    }
}

no* busca(no *raiz, unsigned elemento){
    return buscaRec(raiz, elemento, 0);
}

no* insereRec(no *raiz, unsigned elemento, int nivel){
    if(raiz == NULL){
        raiz = criarNo();
        raiz->chave = elemento;
        return raiz;
    }
    if(raiz->chave == elemento) return;

    if(bit(elemento, nivel) == 0){
        raiz->esq = insereRec(raiz->esq, elemento, nivel + 1);
    }else{
        raiz->dir = insereRec(raiz->dir, elemento, nivel + 1);
    }
}

void insercao(no **raiz, unsigned elemento){
    *raiz = insereRec(*raiz, elemento, 0);
}

no* removerRec(no *raiz, unsigned elemento, int nivel){
    if(raiz == NULL) return NULL;
    if(raiz->chave == elemento){
        if(raiz->dir == NULL && raiz->esq == NULL){
            free(raiz);

            return NULL;
        }else{
            if(raiz->esq != NULL && raiz->dir != NULL){
                raiz->chave = raiz->esq->chave;
                raiz->esq = removerRec(raiz->esq, raiz->chave, nivel + 1);
                return raiz;
            }else{

                no *neto;
                if(raiz->esq == NULL){
                    neto = raiz->dir;
                }else{
                    neto = raiz->esq;
                }

                free(raiz);
                return neto;
            }
        }
    }

    if(bit(elemento, nivel) == 0){
        raiz->esq = removerRec(raiz->esq, elemento, nivel + 1);
    }else{
        raiz->dir = removerRec(raiz->dir, elemento, nivel + 1);
    }
    return raiz;
}

void remover(no **raiz, unsigned elemento){
    *raiz = removerRec(*raiz, elemento, 0);
}

// -=-=-=-==-=-=- UTILITIES

void imprimir(no* arvore, int nivel){
    for(int i = 0; i < nivel; i++){
        printf("    ");
    }
    if(arvore == NULL){
        printf("VAZIO (NULL)\n");
        return;
    }
    printf("%d [\n", arvore->chave);

    imprimir(arvore->esq, nivel + 1);
    imprimir(arvore->dir, nivel + 1);

    for(int i = 0; i < nivel; i++){
        printf("    ");
    }
    printf("]\n");
}

void limpar(no* arvore){
    if(arvore == NULL){
        return;
    }
    limpar(arvore->esq);
    limpar(arvore->dir);

    free(arvore);
}

int main(){
    no *arvore = NULL;

    printf("Inserindo 5 (00101)\n");
    insercao(&arvore, 5);
    imprimir(arvore, 0);

    printf("Inserindo 8 (01000)\n");
    insercao(&arvore, 8);
    imprimir(arvore, 0);

    printf("Inserindo 2 (00010)\n");
    insercao(&arvore, 2);
    imprimir(arvore, 0);

    printf("Inserindo 13 (01101)\n");
    insercao(&arvore, 13);
    imprimir(arvore, 0);

    printf("Inserindo 1 (00001)\n");
    insercao(&arvore, 1);
    imprimir(arvore, 0);

    printf("Inserindo 0 (00000)\n");
    insercao(&arvore, 0);
    imprimir(arvore, 0);

    printf("Removendo o 0 (00000)\n");
    remover(&arvore, 0);
    imprimir(arvore, 0);

    printf("Removendo o 2 (00010)\n");
    remover(&arvore, 2);
    imprimir(arvore, 0);

    printf("Removendo o 8 (01000)\n");
    remover(&arvore, 8);
    imprimir(arvore, 0);

    limpar(arvore);
    return 0;
}

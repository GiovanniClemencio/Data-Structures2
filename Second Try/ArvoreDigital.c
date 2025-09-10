#include<stdio.h>
#include<stdlib.h>

#define MAX_BIT 5

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
    return chave >> (MAX_BIT - 1 - nivel) & 1;
}

no* buscaRec(no* raiz, unsigned chave, int nivel){
    if(raiz == NULL){
        return NULL;
    }
    if(raiz->chave == chave){
        return raiz;
    }
    if(bit(chave, nivel) == 0){
        return buscaRec(raiz->esq, chave, nivel + 1);
    }else{
        return buscaRec(raiz->dir, chave, nivel + 1);
    }
}

no* busca(no *raiz, unsigned chave){
    return buscaRec(raiz, chave, 0);
}

no* insercaoRec(no* arvore, unsigned chave, int nivel){
    if(arvore == NULL){
        no* novo = criarNo();
        novo->chave = chave;

        return novo;
    }
    if(arvore->chave == chave){
        return arvore;
    }
    if(bit(chave, nivel) == 0){
        arvore->esq = insercaoRec(arvore->esq, chave, nivel + 1);
    }else{
        arvore->dir = insercaoRec(arvore->dir, chave, nivel + 1);
    }

    return arvore;
}

void insercao(no **arvore, unsigned chave){
    *arvore = insercaoRec(*arvore, chave, 0);
}

no* removerRec(no *arvore, unsigned chave, int nivel){
    if(arvore == NULL){
        printf("Elemento nao presente na arvore\n");
        return NULL;
    }
    if(arvore->chave != chave){
        if(bit(chave, nivel) == 0){
            arvore->esq = removerRec(arvore->esq, chave, nivel + 1);
        }else{
            arvore->dir = removerRec(arvore->dir, chave, nivel + 1);
        }
        return arvore;
    }

    if(arvore->chave == chave){
        no* removedor;
        if(arvore->esq == NULL && arvore->dir == NULL){ // Se for folha
            removedor = arvore;

            free(removedor);

            return NULL;
        }else{
            if(arvore->esq == NULL || arvore->dir == NULL){ // 1 filho
                removedor = arvore;
                no* neto;
                if(arvore->esq == NULL){
                    neto = arvore->dir;
                }else{
                    neto = arvore->esq;
                }
                free(removedor);

                return neto;
            }else{ // Dois filhos
                arvore->chave = arvore->esq->chave;
                arvore->esq = removerRec(arvore->esq, arvore->chave, nivel + 1);
                return arvore;
            }
        }
    }
}

void remover(no **arvore, unsigned elemento){
    *arvore = removerRec(*arvore, elemento, 0);
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

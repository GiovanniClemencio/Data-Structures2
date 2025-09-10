#include<stdio.h>
#include<stdlib.h>

#define MAX 3

typedef struct no{
    unsigned chave;
    struct no* esq;
    struct no* dir;
}no;

no* criarNo(unsigned elemento){
    no *novo = malloc(sizeof(no));

    novo->esq = NULL;
    novo->dir = NULL;
    novo->chave = elemento;

    return novo;
}

unsigned bit(unsigned chave, int nivel){
    return chave >> (MAX - 1 - nivel) & 1; // Desloca os bits (MAX - 1 - k) vezes e faz um and lógico com 00...01
}

no* buscaRec(no *arvore, unsigned elemento, int nivel){
    if(arvore == NULL){
        return NULL;
    }
    if(elemento == arvore->chave){
        return arvore;
    }
    if(bit(arvore, nivel) == 0){
        return buscaRec(arvore->esq, elemento, nivel + 1);
    }else{
        return buscaRec(arvore->dir, elemento, nivel + 1);
    }
}

no* busca(no **arvore, unsigned elemento){
    return buscaRec(*arvore, elemento, 0);
}

no* inserirRec(no *arvore, unsigned elemento, int nivel){
    if(arvore == NULL){
        return criarNo(elemento);
    }

    if(bit(elemento, nivel) == 0){
        arvore->esq = inserirRec(arvore->esq, elemento, nivel + 1);
    }else{
        arvore->dir = inserirRec(arvore->dir, elemento, nivel + 1);
    }
    return arvore;
}

void inserir(no **arvore, unsigned elemento){
    *arvore = inserirRec(*arvore, elemento, 0);
}

no* removerRec(no *arvore, unsigned elemento, int chave){
    if(arvore == NULL){
        return NULL;
    }
    // Se o nó não contiver o elemento (caso 0);
    if(arvore->chave != elemento){
        if(bit(elemento, chave) == 0){
            arvore->esq = removerRec(arvore->esq, elemento, chave + 1);
        }else{
            arvore->dir = removerRec(arvore->dir, elemento, chave + 1);
        }
        return arvore;
    }
    // A partir daqui só roda se o nó contiver o elemento
    if(arvore->esq == NULL && arvore->dir == NULL){
        free(arvore);
        return NULL;
    }else{
        no *aux;
        if(arvore->esq == NULL || arvore->dir == NULL){
            if(arvore->esq == NULL){
                aux = arvore->dir;
            }else{
                aux = arvore->esq;
            }
            free(arvore);
            return aux;
        }else{
            aux = arvore;
            arvore = arvore->esq;
            free(aux);
            return arvore;
        }
    }
}

void remover(no **arvore, unsigned elemento){
    *arvore = removerRec(*arvore, elemento, 0);
}

// --------- UTILITIES ---------

void limpar(no *raiz){
    if(raiz != NULL){
        limpar(raiz->esq);
        limpar(raiz->dir);
        free(raiz);
    }
}

void imprimir(no *raiz, int nivel){
    if(raiz == NULL){
        return;
    }
    for(int i = 0; i < nivel; i++){
        printf(" ");
    }
    printf("%d\n{\n", raiz->chave);
    imprimir(raiz->esq, nivel + 1);
    imprimir(raiz->dir, nivel + 1);
    printf("}\n");
}

int main(){
    no *raiz = NULL;

    inserir(&raiz, 4);
    inserir(&raiz, 2);
    inserir(&raiz, 0);
    inserir(&raiz, 1);
    inserir(&raiz, 3);

    imprimir(raiz, 0);

    remover(&raiz, 3);
    printf("\nREMOVENDO O 3 ----------(FOLHA)\n");
    imprimir(raiz, 0);

    remover(&raiz, 0);
    printf("\nREMOVENDO O 0 ----------(1 FILHO)\n");
    imprimir(raiz, 0);

    remover(&raiz, 4);
    printf("\nREMOVENDO O 4 ----------(2 FILHO)\n");
    imprimir(raiz, 0);
    // Aparentemente ta funcionando , testar mais
    limpar(raiz);
    return 0;
}

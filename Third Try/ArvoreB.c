#include<stdio.h>
#include<stdlib.h>

#define MAX 5

typedef struct no{
    int n;
    int chave[MAX];
    struct no* filho[MAX + 1];
    int folha;
}no;

no* criaNo(){
    no *novo = malloc(sizeof(no));
    novo->folha = 1;
    for(int i = 0; i <= MAX; i ++){
        novo->filho[i] = NULL;
    }
    novo->n = 0;

    return novo;
}

no* busca(no* raiz, int elemento){
    if(raiz == NULL){
        return NULL;
    }

    int i = 0;
    while(i < raiz->n && elemento > raiz->chave[i]) i++;

    if(i < raiz->n && raiz->chave[i] == elemento){
        return raiz;
    }else{
        if(!raiz->folha){
            busca(raiz->filho[i], elemento);
        }else{
            printf("No nao encontrado\n");
            return NULL;
        }
    }
}

void splitChildren(no *pai, int pos){
    no *menor = pai->filho[pos];
    no *maior = criaNo();

    maior->folha = menor->folha;

    int t = (MAX + 1)/2;

    for(int i = pai->n; i >= pos; i--){
        pai->filho[i + 1] = pai->filho[i];
        if(i != pai->n) pai->chave[i + 1] = pai->chave[i];
    }
    pai->filho[pos + 1] = maior;
    pai->chave[pos] = menor->chave[t - 1];
    pai->n++;

    for(int i = 0; i < t - 1; i++){
        if(maior->folha) maior->filho[i] = menor->filho[t + i];
        maior->chave[i] = menor->chave[t + i];
        maior->n++;
    }
    maior->filho[maior->n] = menor->filho[menor->n];
    menor->n = t - 1;
}

void insereNaoCheio(no *raiz, int elemento){
    int pos = raiz->n - 1;

    if(raiz->folha){
        while(pos >=0 && elemento < raiz->chave[pos]){
            raiz->chave[pos + 1] = raiz->chave[pos];
            pos--;
        }
        pos++; // Agora pos está na posição de inserção

        raiz->chave[pos] = elemento;
        raiz->n++;
    }else{
        while(pos >= 0 && raiz->chave[pos] > elemento) pos--;
        pos++; // Agora pos é posição do filho que pode receber o elemento

        if(raiz->filho[pos]->n == MAX){
            splitChildren(raiz, pos);

            if(raiz->chave[pos] < elemento) pos++;
        }
        insereNaoCheio(raiz->filho[pos], elemento);
    }
}

void insercaoCLRS(no **raiz, int elemento){
    if(*raiz == NULL){
        *raiz = criaNo();
        (*raiz)->chave[0] = elemento;
        (*raiz)->n++;
        return;
    }

    if((*raiz)->n == MAX){
        no *novo = criaNo();
        novo->folha = 0;
        novo->filho[0] = *raiz;

        splitChildren(novo, 0);

        *raiz = novo;
    }
    insereNaoCheio(*raiz, elemento);
}

int predecessor(no *pai, int pos){
    no *buscador = pai->filho[pos];

    while(buscador->filho[buscador->n] != NULL) buscador = buscador->filho[buscador->n];

    return buscador->chave[buscador->n - 1];
}

int sucessor(no *pai, int pos){
    no *buscador = pai->filho[pos];

    while(buscador->filho[0] != NULL) buscador = buscador->filho[0];

    return buscador->chave[0];
}

void rotEsq(no *pai, int pos){
    no *esq = pai->filho[pos];
    no *dir = pai->filho[pos + 1];

    esq->chave[esq->n] = pai->chave[pos];
    esq->n++;
    if(!esq->folha) esq->filho[esq->n] = dir->filho[0];

    pai->chave[pos] = dir->chave[0];

    for(int i = 0; i < dir->n - 1; i++){
        dir->chave[i] = dir->chave[i + 1];
        if(!dir->folha) dir->filho[i] = dir->filho[i + 1];
    }
    if(!dir->folha) dir->filho[dir->n - 1] = dir->filho[dir->n];
    dir->n--;
}

void rotDir(no *pai, int pos){
    no *esq = pai->filho[pos];
    no *dir = pai->filho[pos + 1];

    for(int i = dir->n; i > 0; i--){
        dir->chave[i] = dir->chave[i - 1];
        if(!dir->folha) dir->filho[i + 1] = dir->filho[i];
    }
    if(!dir->folha){
        dir->filho[1] = dir->filho[0];
        dir->filho[0] = esq->filho[esq->n];
    }
    dir->chave[0] = pai->chave[pos];
    dir->n++;

    pai->chave[pos] = esq->chave[esq->n - 1];

    esq->n--;
}

void merge(no *pai, int pos){
    no *menor = pai->filho[pos];
    no *maior = pai->filho[pos + 1];

    int t = (MAX + 1)/2;

    menor->chave[menor->n] = pai->chave[pos];
    menor->n++;

    for(int i = 0; i < maior->n; i++){
        menor->chave[i + t] = maior->chave[i];
        if(!menor->folha) menor->filho[i + t] = maior->filho[i];
        menor->n++;
    }
    if(!menor->folha) menor->filho[menor->n] = maior->filho[maior->n];

    for(int i = pos; i < pai->n - 1; i++){
        pai->chave[i] = pai->chave[i + 1];
        pai->filho[i + 1] = pai->filho[i + 2];
    }
    pai->n--;

    free(maior);
}

void remover(no *raiz, int elemento){
    if(raiz == NULL){
        return;
    }

    int t = (MAX + 1)/2;

    int pos = 0;
    while(pos < raiz->n && elemento > raiz->chave[pos]) pos++;

    if(raiz->folha){ // Caso 1
        if(pos < raiz->n && raiz->chave[pos] == elemento){
            while(pos < raiz->n - 1){
                raiz->chave[pos] = raiz->chave[pos + 1];
                pos++;
            }
            raiz->n--;
        }
        return;
    }

    if(pos < raiz->n && raiz->chave[pos] == elemento){ // Caso 2
        int substituto;
        printf("\n\nDEBUG: ELEMENTO A SER REMOVIDO: %d\n\n", raiz->chave[pos]);
        if(raiz->filho[pos]->n >= t){ //Caso 2 a
            substituto = predecessor(raiz, pos);
            printf("\n\nDEBUG: ELEMENTO substituto: %d\n\n", substituto);
            remover(raiz->filho[pos], substituto);
            raiz->chave[pos] = substituto;
        }else{
            if(raiz->filho[pos + 1]->n >= t){ // Caso 2 b
                substituto = sucessor(raiz, pos + 1);

                remover(raiz->filho[pos + 1], substituto);
                raiz->chave[pos] = substituto;
            }else{ // Caso 2 c
                merge(raiz, pos);

                remover(raiz->filho[pos], elemento);
            }
        }
        return;
    }

    if(raiz->filho[pos]->n == t - 1){ // Caso 3
        if(pos < raiz->n && raiz->filho[pos + 1]->n >= t){ // Caso 3 a.1
            rotEsq(raiz, pos);
        }else{
            if(pos > 0 && raiz->filho[pos - 1]->n >= t){ // Caso 3 a.2
                rotDir(raiz, pos - 1);
            }else{ // Caso 3b
                if(pos < raiz->n){
                    merge(raiz, pos);
                }else{
                    merge(raiz, pos - 1);
                }
            }
        }
    }

    remover(raiz->filho[pos], elemento);
}

// =-=-=-=-=-=- UTILITIES

void imprimirRec(no* atual, int nivel){
    if(atual == NULL){
        return;
    }
    for(int i = 0; i < nivel; i++){
        printf("  ");
    }
    printf("[");
    for(int i = 0; i < atual->n; i++){
        printf("%d", atual->chave[i]);
        if(i + 1 != atual->n){
            printf(" , ");
        }
    }
    printf("]\n");

    for(int i = 0; i <= atual->n; i++){
        imprimirRec(atual->filho[i], nivel + 1);
    }
}

void imprimir(no* raiz){
    imprimirRec(raiz, 0);
}

void limpar(no *raiz){
    if(raiz == NULL){
        return;
    }

    for(int i = 0; i < raiz->n; i++){
        limpar(raiz->filho[i]);
    }

    free(raiz);
}

int main(){
    no *raiz = NULL;

    raiz = criaNo();

    insercaoCLRS(&raiz, 11);
    printf("Inseriu o 11\n");
    imprimir(raiz);

    insercaoCLRS(&raiz, 13);
    printf("Inseriu o 13\n");
    imprimir(raiz);

    insercaoCLRS(&raiz, 15);
    printf("Inseriu o 15\n");
    imprimir(raiz);

    insercaoCLRS(&raiz, 20);
    printf("Inseriu o 20\n");
    imprimir(raiz);

    insercaoCLRS(&raiz, 21);
    printf("Inseriu o 21\n");
    imprimir(raiz);

    insercaoCLRS(&raiz, 22);
    printf("Inseriu o 22\n");
    imprimir(raiz);

    insercaoCLRS(&raiz, 30);
    printf("Inseriu o 30\n");
    imprimir(raiz);

    insercaoCLRS(&raiz, 31);
    printf("Inseriu o 31\n");
    imprimir(raiz);

    insercaoCLRS(&raiz, 34);
    printf("Inseriu o 34\n");
    imprimir(raiz);

    insercaoCLRS(&raiz, 36);
    printf("Inseriu o 36\n");
    imprimir(raiz);

    insercaoCLRS(&raiz, 35);
    printf("Inseriu o 35\n");
    imprimir(raiz);

    insercaoCLRS(&raiz, 29);
    printf("Inseriu o 29\n");
    imprimir(raiz);

    insercaoCLRS(&raiz, 27);
    printf("Inseriu o 27\n");
    imprimir(raiz);

    insercaoCLRS(&raiz, 32);
    printf("Inseriu o 32\n");
    imprimir(raiz);

    insercaoCLRS(&raiz, 24);
    printf("Inseriu o 24\n");
    imprimir(raiz);

    insercaoCLRS(&raiz, 25);
    printf("Inseriu o 25\n");
    imprimir(raiz);

    insercaoCLRS(&raiz, 26);
    printf("Inseriu o 26\n");
    imprimir(raiz);

    insercaoCLRS(&raiz, 28);
    printf("Inseriu o 28\n");

    printf("\nImpressao arvore CLRS\n");
    imprimir(raiz);

    printf("\n\nRemovendo o 13\n");
    remover(raiz, 13);
    imprimir(raiz);

    printf("\n\nRemovendo o 26\n");
    remover(raiz, 26);
    imprimir(raiz);

    printf("\n\nRemovendo o 22\n");
    remover(raiz, 22);
    imprimir(raiz);

    printf("\n\nRemovendo o 25\n");
    remover(raiz, 25);
    imprimir(raiz);

    limpar(raiz);
    return 0;
}

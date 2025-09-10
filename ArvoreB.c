#include<stdio.h>
#include<stdlib.h>

#define MAX 5

typedef struct no{
    int n;
    int chaves[MAX];
    struct no* filhos[MAX + 1];
    int folha;
}no;

no* criarNo(){
    no *novo = malloc(sizeof(no));
    novo->n = 0;
    for(int i = 0; i < MAX + 1; i++){
        novo->filhos[i] = NULL;
    }
    novo->folha = 1;

    return novo;
}

no* busca(no *raiz, int elemento, int *posicao){
    if(raiz == NULL) return NULL;
    int i = 0;
    while(i < raiz->n && elemento > raiz->chaves[i]){
        i++;
    }
    if(raiz->folha){
        if(elemento == raiz->chaves[i]){
            *posicao = i;
            return raiz;
        }else{
            return NULL;
        }
    }else{
        return busca(raiz->filhos[i], elemento, posicao);
    }
}

void splitChildren(no *pai, int pos){
    no *menor = pai->filhos[pos];
    no *maior = criarNo();

    maior->folha = menor->folha;

    int t = (MAX + 1) / 2;

    for(int i = t; i < menor->n; i++){
        maior->chaves[i - t] = menor->chaves[i];
        maior->n++;
    }

    if(!maior->folha){
        for(int i = 0; i < t; i++){
            maior->filhos[i] = menor->filhos[i + t];
        }
    }

    menor->n = t - 1;

    for(int i = pai->n; i > pos; i--){
        pai->chaves[i] = pai->chaves[i - 1];
        pai->filhos[i + 1] = pai->filhos[i];
    }
    pai->filhos[pos + 1] = maior;
    pai->chaves[pos] = menor->chaves[t-1];
    pai->n++;
}

no* insereNaoCheio(no *raiz, int elemento){
    if(raiz == NULL){
        return;
    }
    int i = raiz->n - 1;
    if(raiz->folha){
        while(i >= 0 && raiz->chaves[i] > elemento){
            raiz->chaves[i + 1] = raiz->chaves[i];
            i--;
        }
        raiz->chaves[i + 1] = elemento;
        raiz->n++;
    }else{
        while(i > 0 && raiz->chaves[i] > elemento){
            i--;
        }
        i++;
        if(raiz->filhos[i]->n == MAX){
            splitChildren(raiz, i);
            if(elemento > raiz->chaves[i]) i++;
        }
        insereNaoCheio(raiz->filhos[i], elemento);
    }
}

void inserirCLRS(no **raiz, int elemento){
    if(*raiz == NULL){ // Se n tiver raiz, cria uma
        *raiz = criarNo();
        (*raiz)->chaves[0] = elemento;
        (*raiz)->n++;
        return;
    }

    if((*raiz)->n == MAX){
        no *novo = criarNo();
        novo->folha = 0;
        novo->filhos[0] = (*raiz); // Cria nova raiz

        splitChildren(novo, 0);

        insereNaoCheio(novo, elemento);
        *raiz = novo;
    }else{
        insereNaoCheio(*raiz, elemento);
    }
}

int sucessor(no *arvore){
    no *navegador = arvore;
    int proximo;

    while(arvore->filhos[0] != NULL){
        navegador = arvore->filhos[0];
    }

    proximo = navegador->chaves[0];

    return proximo;
}

int predecessor(no *arvore){
    no *navegador = arvore;
    int antecessor;
    while(arvore->filhos[arvore->n] != NULL){
        navegador = arvore->filhos[arvore->n];
    }

    antecessor = navegador->chaves[navegador->n - 1];

    return antecessor;
}

no* merge(no *pai, int pos){
    no *menor = pai->filhos[pos];
    no *maior = pai->filhos[pos + 1];

    int t = (MAX + 1)/2;

    menor->chaves[menor->n] = pai->chaves[pos];
    menor->n++;

    for(int i = menor->n; i < MAX; i++){
        menor->chaves[i] = maior->chaves[i - t];
        if(!menor->folha){
            menor->filhos[i] = maior->filhos[i - t];
        }
        menor->n++;
    }
    if(!menor->folha) menor->filhos[menor->n] = maior->filhos[maior->n];

    for(int i = pos; i < MAX - 1; i++){
        pai->chaves[i] = pai->chaves[i + 1];
        pai->filhos[i + 1] = pai->filhos[i + 2];
    }
    pai->n--;

    free(maior);
}

no* rotacaoDir(no *pai, int pos){
    int elementoPai = pai->chaves[pos];
    no *maior = pai->filhos[pos + 1];
    no *menor = pai->filhos[pos];

    pai->chaves[pos] = menor->chaves[menor->n - 1];

    for(int i = maior->n; i > 0; i--){
        maior->chaves[i] = maior->chaves[i + 1];
        if(!maior->folha){
            maior->filhos[i + 1] = maior->filhos[i];
        }
    }
    maior->chaves[0] = elementoPai;
    maior->filhos[0] = menor->filhos[menor->n];
    maior->n++;
    menor->n--;
}

no* rotacaoEsq(no *pai, int pos){
    int elementoPai = pai->chaves[pos];
    no *menor = pai->filhos[pos];
    no *maior = pai->filhos[pos + 1];

    menor->chaves[menor->n] = elementoPai;
    menor->n++;
    menor->filhos[menor->n] = maior->filhos[0];
    pai->chaves[pos] = maior->chaves[0];

    for(int i = 0; i< MAX - 1; i++){
        maior->chaves[i] = maior->chaves[i + 1];
        if(!maior->folha){
            maior->filhos[i] = maior->filhos[i + 1];
        }
    }
    if(!maior->folha){
        maior->filhos[maior->n - 1] = maior->filhos[maior->n];
    }
    maior->n--;
}

void remover(no **arvore, int elemento){
    if((*arvore) == NULL){
        printf("Elemento nao encontrado\n");
        return;
    }

    int t = (MAX + 1)/2;
    int pos = 0;

    while(pos < (*arvore)->n - 1 && elemento > (*arvore)->chaves[pos]){
        pos++;
    }

    if((*arvore)->folha){ // Caso 1
        if((*arvore)->chaves[pos] == elemento){
            for(int i = pos; i < (*arvore)->n; i++){
                (*arvore)->chaves[i] = (*arvore)->chaves[i + 1];
            }
            (*arvore)->n--;
            return;
        }else{
            printf("Elemento não presente na folha\n");
        }
    }
    // Caso 2
    if((*arvore)->chaves[pos] == elemento && (*arvore)->filhos[pos]->n >=t){ // Caso 2a
        int antecessor = predecessor((*arvore)->filhos[pos]);

        (*arvore)->chaves[pos] = antecessor;
        remover(&(*arvore)->filhos[pos], antecessor);
        return;
    }else{
        if((*arvore)->chaves[pos] == elemento && (*arvore)->filhos[pos + 1]->n >=t){ // Caso 2b
            int proximo = sucessor((*arvore)->filhos[pos + 1]);

            (*arvore)->chaves[pos] = proximo;
            remover(&(*arvore)->filhos[pos + 1], proximo);
            return;
        }else{
            if((*arvore)->chaves[pos] == elemento){ // Caso 2 c
                merge(&(*arvore), pos);
                return remover((*arvore)->filhos[pos], elemento);
            }
        }
    }

    if((*arvore)->filhos[pos]->n < t){ // Caso 3
        if(pos > 0 &&  pos < (*arvore)->n - 1 && ((*arvore)->filhos[pos - 1]->n >= t || (*arvore)->filhos[pos + 1]->n >= t)){ // Caso 3 a
            if((*arvore)->filhos[pos - 1]->n >= t){
                rotacaoDir((*arvore), pos - 1);
            }else{
                rotacaoEsq((*arvore), pos);
            }
        }else{
            if((pos > 0 && (*arvore)->filhos[pos - 1]->n >= t) || pos == 0 && (*arvore)->filhos[pos + 1]->n >= t){ // Caso 3a na pos == 0 ou pos == *arvore->n - 1
                if(pos == 0){
                    rotacaoEsq((*arvore), pos);
                }else{
                    rotacaoDir((*arvore), pos);
                }
            }else{ // Caso 3b
                if(pos < (*arvore)->n){
                    merge((*arvore), pos);
                }else{
                    merge((*arvore), pos - 1);
                }
            }
        }
    }

    remover(&(*arvore)->filhos[pos], elemento);
}


///////////////////////// UTILITIES

/* imprime um nó com indentação por profundidade */
void imprimir_no_rec(no *r, int profundidade) {
    if (r == NULL) return;

    /* indentação */
    for (int k = 0; k < profundidade; k++) printf("  ");

    /* cabeçalho do nó */
    printf("Nodo  [");
    for (int i = 0; i < r->n; i++) {
        printf("%d", r->chaves[i]);
        if (i + 1 < r->n) printf(" ");
    }
    printf("]  (n=%d, folha=%d)\n", r->n, r->folha);

    /* se não for folha, imprime os filhos recursivamente */
    if (!r->folha) {
        for (int i = 0; i <= r->n; i++) {
            imprimir_no_rec(r->filhos[i], profundidade + 1);
        }
    }
}

/* wrapper público — chama a recursão começando da raiz */
void imprimirArvore(no *raiz) {
    imprimir_no_rec(raiz, 0);
}

void limpar(no* raiz){
    no *limpador = raiz;
    if(limpador != NULL){
        for(int i = 0; i < limpador->n+1; i++){
            limpar(limpador->filhos[i]);
        }
    }
    free(limpador);
}

int main(){
    no *raiz = NULL;

    raiz = criarNo();
    inserirCLRS(&raiz, 11);
    printf("Inseriu o 11\n");
    imprimirArvore(raiz);
    inserirCLRS(&raiz, 13);
    printf("Inseriu o 13\n");
    imprimirArvore(raiz);
    inserirCLRS(&raiz, 15);
    printf("Inseriu o 15\n");
    imprimirArvore(raiz);
    inserirCLRS(&raiz, 20);
    printf("Inseriu o 20\n");
    imprimirArvore(raiz);
    inserirCLRS(&raiz, 21);
    printf("Inseriu o 21\n");
    imprimirArvore(raiz);
    inserirCLRS(&raiz, 22);
    printf("Inseriu o 22\n");
    imprimirArvore(raiz);
    inserirCLRS(&raiz, 30);
    printf("Inseriu o 30\n");
    imprimirArvore(raiz);
    inserirCLRS(&raiz, 31);
    printf("Inseriu o 31\n");
    imprimirArvore(raiz);
    inserirCLRS(&raiz, 34);
    printf("Inseriu o 34\n");
    imprimirArvore(raiz);
    inserirCLRS(&raiz, 36);
    printf("Inseriu o 36\n");
    imprimirArvore(raiz);
    inserirCLRS(&raiz, 35);
    printf("Inseriu o 35\n");
    imprimirArvore(raiz);
    inserirCLRS(&raiz, 29);
    printf("Inseriu o 29\n");
    imprimirArvore(raiz);
    inserirCLRS(&raiz, 27);
    printf("Inseriu o 27\n");
    imprimirArvore(raiz);
    inserirCLRS(&raiz, 32);
    printf("Inseriu o 32\n");
    imprimirArvore(raiz);
    inserirCLRS(&raiz, 24);
    printf("Inseriu o 24\n");
    imprimirArvore(raiz);
    inserirCLRS(&raiz, 25);
    printf("Inseriu o 25\n");
    imprimirArvore(raiz);
    inserirCLRS(&raiz, 26);
    printf("Inseriu o 26\n");
    imprimirArvore(raiz);
    inserirCLRS(&raiz, 28);
    printf("Inseriu o 28\n");
    imprimirArvore(raiz);

    imprimirArvore(raiz);

    printf("\n\nRemovendo o 13\n");
    remover(&raiz, 13);
    imprimirArvore(raiz);

    printf("\n\nRemovendo o 26\n");
    remover(&raiz, 26);
    imprimirArvore(raiz);

    printf("\n\nRemovendo o 22\n");
    remover(&raiz, 22);
    imprimirArvore(raiz);

    limpar(raiz);
    return 0;
}

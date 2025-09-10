#include<stdio.h>
#include<stdlib.h>

#define MAX 5

typedef struct no{
    int n;
    int chave[MAX];
    struct no* filho[MAX + 1];
    int folha;
}no;

no* criarNo(){
    no *novo = malloc(sizeof(no));
    novo->folha = 1;
    novo->n = 0;
    int i = 0;
    while(i < MAX + 1){
        novo->filho[i] = NULL;
        i++;
    }

    return novo;
}

no* busca(no *raiz, int elemento){
    int i = 0;

    while(i < raiz->n && elemento > raiz->chave[i]){
        i++;
    }
    if(i < raiz->n && elemento == raiz->chave[i]){
        return raiz;
    }
    if(raiz->folha){
        return NULL;
    }
    return busca(raiz->filho[i], elemento);
}

no* buscaBin(no *raiz, int minus, int plus, int elemento){
    int pos = (minus + plus)/2;
    if(raiz->chave[pos] == elemento){
        return raiz;
    }
    if(raiz->chave[pos] > elemento && minus > 0){
        return buscaBin(raiz, minus, pos, elemento);
    }
    if(raiz->chave[pos] < elemento && plus < raiz->n - 1){
        return buscaBin(raiz, pos, plus, elemento);
    }
    if(raiz->folha){
        return NULL;
    }else{
        int i = 0;
        while(i < raiz->n && elemento > raiz->chave[i]){
            i++;
        }
        return buscaBin(raiz->filho[i], 0, raiz->n - 1, elemento);
    }
}

void splitChildren(no *pai, int pos){
    no *menor = pai->filho[pos]; // Nó a ser splitado
    no *maior = criarNo(); // Nó que armazenará os maiores elementos do nó a ser splitado

    maior->folha = menor->folha;

    int i = pai->n;
    int t = (MAX + 1)/2;

    while(i >= pos){
        if(i < MAX - 1) pai->chave[i + 1] = pai->chave[i]; // Arruma espaço para a chave que irá subir
        pai->filho[i + 1] = pai->filho[i]; // Arruma os ponteiros dos filhos para ficarem certos quando a chave subir;
        i--;
    }

    pai->filho[pos + 1] = maior;
    pai->chave[pos] = menor->chave[t - 1]; // Subindo o elemento do split
    pai->n++;

    for(int i = 0 ; i < t - 1; i++){
        if(!menor->folha) maior->filho[i] = menor->filho[i + t]; // Passando os filhos dos maiores elementos para o outro
        maior->chave[i] = menor->chave[i + t]; // Passando os maiores elementos do nó a ser splitado para o outro
        maior->n++;
    }
    maior->filho[maior->n] = menor->filho[menor->n]; // Passando o ultimo filho

    menor->n = t - 1; // Ajustando o tamanho do menor;
}

/*
int splitChildrenClassico(no *pai, int pos){
    no *menor = pai->filho[pos]; // Nó a ser splitado
    no *maior = criarNo(); // Nó que armazenará os maiores elementos do nó a ser splitado

    maior->folha = menor->folha;

    int i = pai->n;
    int t = (MAX + 1)/2;

    for(int i = 0 ; i < t - 1; i++){
        if(!menor->folha) maior->filho[i] = menor->filho[i + t]; // Passando os filhos dos maiores elementos para o outro
        maior->chave[i] = menor->chave[i + t]; // Passando os maiores elementos do nó a ser splitado para o outro
        maior->n++;
    }
    maior->filho[maior->n] = menor->filho[menor->n]; // Passando o ultimo filho

    menor->n = t - 1; // Ajustando o tamanho do menor;

    return menor->chave[menor->n]; // Retorna a chave que vai subir
}

void inserirClassicoNaoCheio(no *raiz, no *pai, int elemento){
    if(raiz->folha){
        if(raiz->n == MAX){ // Quer dizer que raiz está cheio
            int posFilho = 0;
            while(pai->filho[posFilho] != raiz){
                posFilho++;
            }
            int sobe = splitChildrenClassico(pai, posFilho);
            pai->folha = 1;

            inserirClassico(&pai, sobe);

            pai->folha = 0;
        }
        int i = raiz->n - 1;
        while(i >= 0 && raiz->chave[i] > elemento){
            raiz->chave[i + 1] = raiz->chave[i];
            i--;
        }
        i++;
        raiz->chave[i] = elemento;

    }else{
        int i = raiz->n - 1;
        while(i >= 0 && raiz->chave[i] > elemento){
            raiz->chave[i + 1] = raiz->chave[i];
            i--;
        }
        i++;
        inserirClassicoNaoCheio(raiz->filho[i], raiz, elemento);
    }
}

void inserirClassico(no **raiz, int elemento){
    if(*raiz == NULL){
        *raiz = criarNo();
        (*raiz)->chave[0] = elemento;
        (*raiz)->n++;
        return;
    }

    if((*raiz)->folha && (*raiz)->n == MAX){
        no *novo = criarNo();
        novo->filho[0] = *raiz;
        splitChildren(novo, 0);

        *raiz = novo;
    }else{
        if((*raiz)->folha){
            inserirClassicoNaoCheio(*raiz, NULL, elemento);
        }else{
            int i = 0;
            while(i < (*raiz)->n && (*raiz)->chave[i] < elemento){
                i++;
            }
            inserirClassicoNaoCheio((*raiz)->filho[i], *raiz, elemento);
        }
    }
}
*/

void insereNaoCheio(no *raiz, int elemento){
    int i = raiz->n - 1;
    if(raiz->folha){ // Se for folha é nela que a inserção acontece
        while(i >= 0 && elemento < raiz->chave[i]){ // Se elemento é menor que o atual da chave
            raiz->chave[i + 1] = raiz->chave[i]; //Move o atual da chave uma posição a frente
            i--;
        }
        i++; // Aqui se coloca o i na ultima posição em que ele é menor (local de inserção)
        raiz->chave[i] = elemento;
        raiz->n++;
    }else{ // Se não for folha
        while(i >= 0 && elemento < raiz->chave[i]){
            i--; // Vai testando todas as chaves do nó atual até que o elemento atual do nó seja menor que elemento
        }
        i++;// coloca ele na posição certa do no filho que deve ir
        if(raiz->filho[i]->n == MAX){ // Se o nó filho estiver cheio faz o split
            splitChildren(raiz, i);
            if(elemento > raiz->chave[i]) i++; // Aqui verifica se o elemento que subiu do filho cheio mudou a posição do filho a ser inserido
        }
        insereNaoCheio(raiz->filho[i], elemento); // chamada recursiva para o nó filho que deve continuar a  busca de inserção
    }
}

void inserirCLRS(no **raiz, int elemento){ // Wrapper
    if((*raiz) == NULL){ // Se a arvore estiver vaiza cria uma raiz com o elemento de inserção
        *raiz = criarNo();
        (*raiz)->chave[0] = elemento;
        (*raiz)->n++;
        return;
    }

    if((*raiz)->n == MAX){ // Se a raiz estiver cheia, cria uma nova raiz e faz o split da antiga
        no *novaRaiz = criarNo();
        novaRaiz->folha = 0;
        novaRaiz->filho[0] = *raiz;
        splitChildren(novaRaiz, 0); // splitChindren(no *pai, int posicao)

        *raiz = novaRaiz; // Substitui a raiz antiga pela nova
    }
    insereNaoCheio(*raiz, elemento); // Insere a partir da raiz que agora é com ctz não cheia
}

int sucessor(no *raiz){
    if(!raiz->folha){
        return sucessor(raiz->filho[0]);
    }
    return raiz->chave[0];
}

int predecessor(no *raiz){
    if(!raiz->folha){
        return predecessor(raiz->filho[raiz->n]);
    }else{
        return raiz->chave[raiz->n - 1];
    }
}

void merge(no *pai, int pos){
    no *esq = pai->filho[pos];
    no *dir = pai->filho[pos + 1];

    esq->chave[esq->n] = pai->chave[pos];
    esq->n++;

    for(int i = pos; i < pai->n - 1; i++){
        pai->chave[i] = pai->chave[i + 1];
        pai->filho[i + 1] = pai->filho[i + 2];
    }
    pai->n--;

    int i = 0;
    while(esq->n <= MAX){
        if(esq->n < MAX) esq->chave[esq->n] = dir->chave[i];
        if(!esq->folha) esq->filho[esq->n] = dir->filho[i];
        i++;
        esq->n++;
    }
    esq->n--;

    free(dir);
}

void rotacaoEsq(no *pai, int pos){
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

void rotacaoDir(no *pai, int pos){
    no *esq = pai->filho[pos];
    no *dir = pai->filho[pos + 1];

    int i = dir->n;
    while(i >= 0){
        if(!dir->folha) dir->filho[i + 1] = dir->filho[i];
        if(i < dir->n) dir->chave[i + 1] = dir->chave[i];
        i--;
    }
    dir->n++;
    dir->chave[0] = pai->chave[pos];
    printf("\n\nDEBUG ELEMENTO DESCIDO NA ROTACAO(%d)\n\n", dir->chave[0]);
    printf("\nTAMANHO RAIZ: %d\n", dir->n);
    if(!dir->folha) dir->filho[0] = esq->filho[esq->n];

    pai->chave[pos] = esq->chave[esq->n - 1];

    esq->n--;
}

void remover(no *raiz, int elemento){
    if(raiz == NULL){
        printf("Arvore vazia\n");
        return;
    }

    int t = (MAX + 1) / 2;
    int pos = 0;

    while(pos < raiz->n - 1 && raiz->chave[pos] < elemento) pos++; // Move até a posição onde o nó a ser removido estaria
    printf("Encontrei pos: %d\nPrimeiro elemento: %d\nSegundo elemento: %d\nTerceiro elemento: %d\n",pos,raiz->chave[0],raiz->chave[1],raiz->chave[2]);

    // Caso 1
    if(raiz->folha){
        printf("\nREM FOLHA Pos: %d\n", pos);
        if(raiz->chave[pos] == elemento){
            for(int i = pos; i < raiz->n - 1; i++){
                raiz->chave[i] = raiz->chave[i + 1];
            }
            raiz->n--;
        }else{
            printf("Elemento nao encontrado\n");
        }
        return;
    }
    // Caso 2 aqui pos é o local da chave a ser retirada
    if(raiz->chave[pos] == elemento){
        int substituto;
        if(raiz->filho[pos]->n >= t){ // Caso 2 a
            substituto = predecessor(raiz->filho[pos]);
            raiz->chave[pos] = substituto;
            remover(raiz->filho[pos], substituto);
        }else{
            if(raiz->filho[pos + 1]->n >= t){ // Caso 2 b
                substituto = sucessor(raiz->filho[pos +  1]);
                raiz->chave[pos] = substituto;
                remover(raiz->filho[pos + 1], substituto);
            }else{ // Caso 2 c
                merge(raiz, pos);
                remover(raiz->filho[pos], elemento);
            }
        }
        return;
    }
    printf("Antes do caso 3\n");
    printf("Pos: %d", pos);
    // Caso 3 aqui pos é a indice da subarvore filha que contém a chave a ser retirada
    if(raiz->filho[pos]->n < t){
        printf("Entrou no caso 3\n");
        if(pos < raiz->n && raiz->filho[pos + 1]->n >= t){ //  Inicio do Caso 3a
            rotacaoEsq(raiz, pos);
        }else{
            if(pos > 0 && raiz->filho[pos - 1]->n >= t){
                rotacaoDir(raiz, pos - 1);
            }else{ // Caso 3b
                printf("Entou no caso 3 b\n");
                if(pos < raiz->n){
                    printf("Entrou antes do merge\n");
                    merge(raiz, pos);
                    printf("Saiu do merge\n");
                }else{
                    if(pos == 0){
                        merge(raiz, pos);
                    }else{
                        merge(raiz, pos - 1);
                    }
                }
            }
        }
    }
    printf("Vai chamar a recursiva\nELEMENTO A SER RETIRADO(%d)\n",elemento);
    remover(raiz->filho[pos], elemento);
    printf("Saiu da recursiva\n");

}

// -=-=-=-=-=-=-=-=-=-=- Utilities

/* imprime um nó com indentação por profundidade */
void imprimir_no_rec(no *r, int profundidade) {
    if (r == NULL){
        for (int k = 0; k < profundidade; k++) printf("  ");
        printf("NULL\n");
        return;
    }

    /* indentação */
    for (int k = 0; k < profundidade; k++) printf("  ");

    /* cabeçalho do nó */
    printf("Nodo  [");
    for (int i = 0; i < r->n; i++) {
        printf("%d", r->chave[i]);
        if (i + 1 < r->n) printf(" ");
    }
    printf("]  (n=%d, folha=%d)\n", r->n, r->folha);

    /* se não for folha, imprime os filhos recursivamente */
    if (!r->folha) {
        for (int i = 0; i <= r->n; i++) {
            imprimir_no_rec(r->filho[i], profundidade + 1);
        }
    }
}

/* wrapper público — chama a recursão começando da raiz */
void imprimirArvore(no *raiz) {
    imprimir_no_rec(raiz, 0);
}

void limpar(no *raiz){
    printf("Entrou no limpar");
    if(!raiz->folha){
        for(int i = 0; i <= raiz->n; i++){
            limpar(raiz->filho[i]);
        }
    }
    free(raiz);
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

    printf("\nImpressao arvore CLRS\n");
    imprimirArvore(raiz);

    printf("\n\nRemovendo o 13\n");
    remover(raiz, 13);
    imprimirArvore(raiz);

    printf("\n\nRemovendo o 26\n");
    remover(raiz, 26);
    imprimirArvore(raiz);

    printf("\n\nRemovendo o 22\n");
    remover(raiz, 22);
    imprimirArvore(raiz);

    printf("\n\nRemovendo o 25\n");
    remover(raiz, 25);
    imprimirArvore(raiz);

    limpar(raiz);

    return 0;
}

#include<stdio.h>
#include<stdlib.h>

typedef struct no{
    int v; // A qual vertice esse nó de refere
    struct no * prox;
}no;

typedef no * pNo;

typedef struct{
    pNo *adjacencia;
    int n;
} grafo;

typedef grafo * pGrafo;

pGrafo criarGrafo(int n){
    pGrafo novo = malloc(sizeof(grafo));

    novo->n = n;

    novo->adjacencia = malloc(n * sizeof(pNo)); // Cria a base da lista ligada de adjacencia
    for(int i = 0; i < novo->n; i++){ // popula a base das listas de adjacência com NULL
        novo->adjacencia[i] = NULL;
    }

    return novo;
}

void liberaLista(pNo lista){
    if(lista != NULL){
        liberaLista(lista->prox);
        free(lista);
    }
}

void destroiGrafo(pGrafo lixo){
    for(int i = 0; i < lixo->n; i++){
        liberaLista(lixo->adjacencia[i]);
    }

    free(lixo->adjacencia);

    free(lixo);
}

pNo insereNaLista(pNo lista, int v){
    pNo novo = malloc(sizeof(no));

    novo->v = v;
    novo->prox = lista;

    return novo;
}

void insereAresta(pGrafo g, int u, int v){
    g->adjacencia[u] = insereNaLista(g->adjacencia[u], v);
    g->adjacencia[v] = insereNaLista(g->adjacencia[v], u);
}

pNo removeDaLista(pNo lista, int v){
    if(lista == NULL) return NULL; // se chegou no fim

    if(lista->v == v){ // se o nó atual tem o elemento a ser removido
        pNo proximo = lista->prox; // se cria um ponteiro pro proximo
        free(lista); // libera o atual
        return proximo; //retorna o proximo, continuando o encadeamento da lista
    }else{
        lista->prox = removeDaLista(lista->prox, v); // caso contário vai pro próximo
        return lista; // e retorna o que já estava ligado (n mexe no atual)
    }
}

void removeAresta(pGrafo g, int u, int v){
    g->adjacencia[u] = removeDaLista(g->adjacencia[u], v);
    g->adjacencia[v] = removeDaLista(g->adjacencia[v], u);
}

int temAresta(pGrafo g, int u, int v){
    for(pNo atual = g->adjacencia[u]; atual != NULL; atual = atual->prox){ // Segue a linha do u enquanto não encontrar nulo
        if(atual->v == v){ // Se o no da linha de u tem v como elemento, significa que estão ligados
            return 1;
        }
    }

    return 0;
}

void imprimeArestas(pGrafo g){
    for(int u = 0; u < g->n; u++){
        for(pNo aresta = g->adjacencia[u]; aresta != NULL; aresta= aresta->prox){
            printf("{%d %d}\n",u, aresta->v);
        }
    }
}

// novo
void visita_rec(pGrafo g, int *componentes, int componenteAtual, int vertice){
    pNo navegador;

    componentes[vertice] = componenteAtual;

    for(navegador = g->adjacencia[vertice]; navegador != NULL; navegador = navegador->prox){
        if(componentes[navegador->v] == -1){
            visita_rec(g, componentes, componenteAtual, navegador->v);
        }
    }
}

int * encontra_componentes(pGrafo g){
    int cont;
    int componenteAtual = 0; // Controlador de qual componente estamos atualmente
    int *componentes = malloc(g->n * sizeof(int)); // cria o vetor que indica a qual componente cada vertice pertence, a posição no vetor indica o vertice e o valor indica qual componente

    for(cont = 0; cont < g->n; cont ++){
        componentes[cont] = -1; // inicializa todas as posições com valor neutro controle
    }

    for(cont = 0; cont < g->n; cont++){ // navega vertice a vertice
        if(componentes[cont] == -1){ // se esse vértice não tem componente, chama a visita recursiva
            visita_rec(g, componentes, componenteAtual, cont);
            componenteAtual++; // se chegou aqui foi até o fim de um componente, então agora se inicia outro
        }
    }

    return componentes;
}

void busca_em_profundidade(pGrafo g, int *pais, int noPai, int noFilho){
    pNo navegador;

    pais[noFilho] = noPai; // Primeiro se coloca o pai do nó atual
    for(navegador = g->adjacencia[noFilho]; navegador != NULL; navegador = navegador->prox){ // esse for vai navefar pelos adjacentes do atual
        if(pais[navegador->v] == -1){ // se o nó navegado ainda n tiver pai
            busca_em_profundidade(g, pais, noFilho, navegador->v); //chamada recursiva pro nó sem pai
        }
    }
}

int * encontra_caminhos(pGrafo g, int noInicial){
    int cont;

    int *pai = malloc(g->n * sizeof(int)); // aloca o vetor de pais, a posição indica o vertice e o valor indica o pai desse vértice
    for(cont = 0; cont < g->n; cont++){
        pai[cont] = -1; // inicializando todos os pais com o valor nulo de -1
    }

    busca_em_profundidade(g, pai, noInicial, noInicial);
    return pai;
}

void imprime_caminho_reverso(int verticeAtual, int *pai){
    printf("%d ", verticeAtual); // imprime
    if(pai[verticeAtual] != verticeAtual){ // verifica se o pai dele não é ele mesmo
        imprime_caminho_reverso(pai[verticeAtual], pai); // chama a impressão para o pai
    }
}

void imprime_caminho(int verticeAtual, int *pai){
    if(pai[verticeAtual] != verticeAtual){ // verifica se o pai do atual não é ele mesmo
        imprime_caminho(pai[verticeAtual], pai); // chamada recursiva para o pai do atual (se constrói um stack)
    }
    printf("%d ", verticeAtual); // imprime o atual após ter a impressão de todos os antecessores
}

int main(){
    pGrafo novo = criarGrafo(8);

    insereAresta(novo, 0, 3);
    insereAresta(novo, 0, 4);
    insereAresta(novo, 0, 7);
    insereAresta(novo, 3, 4);
    // Primeira componente completa

    // O vertice 1 ficará solto, então segunda componente completa
    insereAresta(novo, 2, 5);
    insereAresta(novo, 2, 6);
    //Terceira componente completa

    printf("Grafo inicial: \n");
    imprimeArestas(novo);
    printf("\n");

    int * componentes = encontra_componentes(novo);

    printf("\nComponenetes: ");
    for(int i = 0; i < 8; i++){
        printf("%d ", componentes[i]);
    }
    printf("\n");

    int * pais = encontra_caminhos(novo, 0);

    printf("\nVetor de pais: ");
    for(int i = 0; i < novo->n; i++){
        printf("%d ", pais[i]);
    }
    printf("\n");

    printf("\nCaminho comecado pelo 3 na rodem reversa: ");
    imprime_caminho_reverso(3, pais);
    printf("\n");
    printf("\nCaminho comecado pelo 3 na ordem correta: ");
    imprime_caminho(3, pais);


    destroiGrafo(novo);
    return 0;
}

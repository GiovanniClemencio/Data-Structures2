#include<stdio.h>
#include<stdlib.h>

typedef struct{
    int **adj; // Matriz adjacência (é uma matriz n x n)
    int n; // Número de vertices (pontos)
}grafo;

typedef grafo * pGrafo; // Só facilitando a escrita

pGrafo CriarGrafo(int n){
    pGrafo novo = malloc(sizeof(grafo)); // Aloca espaço para o grafo

    novo->n = n;

    novo->adj = malloc(n * sizeof(int *)); // Aloca o vetor de ponteiros de int que será usado para gerar a matriz adjacencia

    for(int i = 0; i <n; i++){
        novo->adj[i] = malloc(n * sizeof(int)); // Vai em cada posição do vetor da base da matriz adjecência e cria uma linha com n espaços em cada
    }

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n ; j++){
            novo->adj[i][j] = 0; // Populando toda a matriz com 0
        }
    }

    return novo;
}

void destroiGrafo(pGrafo lixo){
    for(int i = 0 ; i < lixo->n ; i++){
        free(lixo->adj[i]); // Libera cada linha da matriz adjacencia
    }
    free(lixo->adj); // libera aquela base da matriz adjacência
    free(lixo); // Libera o grafo
}

void insereAresta(pGrafo g, int u, int v){
    // u e v são as vertices que serão ligados
    g->adj[u][v] = 1;
    g->adj[v][u] = 1; // A matriz é espelhada (se u está ligado a v então v está ligado a u)
}

void removeAresta(pGrafo g, int u, int v){
    // u e v são os vértices que deverão ser desligados
    if(g->adj[u][v] == 0) return; // Se já estiverem desligados, só sair
    g->adj[u][v] = 0;
    g->adj[v][u] = 0; // A matriz é espelhada (se u está ligado a v então v está ligado a u)
}

int temAresta(pGrafo g, int u, int v){
    return g->adj[u][v]; // Como só usamos 0 e 1 na matriz, só retornamos o valor que será interpretado como booleano no retorno
}

void imprimeArestas(pGrafo g){
    for(int i = 0; i < g->n; i++){ // Aqui i é o número da linha
        for(int j = i + 1; j < g->n; j++){ // J é numero de colunas e começa em i + 1 para ecvitar repetição já que a matriz é espelhada
            if(g->adj[i][j]) printf("{%d %d}\n", i , j);
        }
    }
}

// pGrafo leGrafo()

int grau(pGrafo g, int u){ // Ta um pouco diferente de slide
    int grau = 0;
    for(int i = 0; i < g->n; i++){
        grau += g->adj[u][i]; // Soma todo o vetor na linha de u
    }

    return grau;
}

int maisPopular(pGrafo g){
    int atual, grauAtual, max, grauMax;
    max = 0;
    grauMax = grau(g, 0);
    for(atual = 1; atual < g->n; atual++){
        grauAtual = grau(g, atual);
        if(grauAtual > grauMax){
            grauMax = grauAtual;
            max = atual;
        }
    }
    return max;
}

void imprimeRecomendacoes(pGrafo g, int u){
    for(int i = 0; i < g->n; i++){ // passa por todas as linhas da matriz
        if(g->adj[u][i]){ // se a linha atual é adjacente a u
            for(int j = 0; j < g->n; j++){ // Busca os adjacentes da atual
                if(g->adj[i][j] && j != u && !g->adj[u][j]){ // Se é adjacente, não é o próprio u e já não é adjacente a u, o recomenda
                    printf("%d\n", j);
                }
            }
        }
    }
}
// novo

int busca_rec(pGrafo g, int *visitado, int atual, int fim){
    int cont;

    if(atual == fim) return 1; // Sempre existe caminho entre um vertice e ele mesmo

    visitado[atual] = 1; // primeiro se marca que o nó atual já foi visitado

    for(cont = 0; cont < g->n; cont++){ // Esse for vai fazer com que seja checada a linha inteira atual (g->adj[atual][cont])
        if(g->adj[atual][cont] && !visitado[cont]){ // Se o atual e o cont forem adjacentes e o cont n foi visitado ainda, precisamos olhar a partir desse cont
            if(busca_rec(g, visitado, cont, fim)){ // aqui se chama a finção recursivamente para o cont, só vai para a proxima linha se tiver um caminho, caso contrario vai para a próxima iteração do for
                return 1;
            }
        }
    }
    return 0; // só chega aqui se em nenhuma iteração do for foi encontrado o caminho (ou seja, nada a partir da linha de atual)
}

int existe_caminho(pGrafo g, int inicio, int fim){ // é praticamente um wrapper para busca_rec()
    int encontrou, cont;
    int *visitado = malloc(g->n * sizeof(int)); // É um vetor que representa os vertices, o valor incica de foi fisitado ou n (booleano)
    for(cont = 0; cont < g->n; cont++){
        visitado[cont] = 0; // Preenche visitado com 0s
    }
    encontrou = busca_rec(g, visitado, inicio, fim); // chava a função que verifica se há caminho msm

    free(visitado);

    return encontrou;
}

int main(){
    pGrafo exemplo = CriarGrafo(6);

    insereAresta(exemplo, 1, 4);
    insereAresta(exemplo, 1, 2);
    insereAresta(exemplo, 1, 5);
    insereAresta(exemplo, 0, 3);

    printf("Grafor inicial: \n");
    imprimeArestas(exemplo);
    printf("\n");

    int caminho;

    caminho = existe_caminho(exemplo, 1, 3);
    printf("Existe caminho entre 1 e 3 : ");
    if(caminho){
        printf("Sim!\n");
    }else printf("Nao! \n");

    caminho = existe_caminho(exemplo, 2, 4);
    printf("Existe caminho entre 2 e 4 : ");
    if(caminho){
        printf("Sim!\n");
    }else printf("Nao! \n");

    caminho = existe_caminho(exemplo, 4, 2);
    printf("Existe caminho entre 4 e 2 : ");
    if(caminho){
        printf("Sim!\n");
    }else printf("Nao! \n");

    caminho = existe_caminho(exemplo, 1, 1);
    printf("Existe caminho entre 1 e 1 : ");
    if(caminho){
        printf("Sim!\n");
    }else printf("Nao! \n");

    caminho = existe_caminho(exemplo, 1, 2);
    printf("Existe caminho entre 1 e 2 : ");
    if(caminho){
        printf("Sim!\n");
    }else printf("Nao! \n");

    caminho = existe_caminho(exemplo, 3, 1);
    printf("Existe caminho entre 3 e 1 : ");
    if(caminho){
        printf("Sim!\n");
    }else printf("Nao! \n");

    destroiGrafo(exemplo);
    return 0;
}

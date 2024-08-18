#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 5

typedef struct grafo_cidades Grafo;
struct grafo_cidades {
    int cidade; // vértice
    int estrada; // aresta
    Grafo* prox; // próxima aresta
};

typedef struct aresta_estradas Aresta;
struct aresta_estradas {
    int origem;
    int destino;
    int peso;
};

typedef struct melhor_caminho Subconjunto;
struct melhor_caminho {
    int pai;
    int altura;
};

void printaCidades(Grafo* cidades) {
    for (int i = 0; i < MAX_VERTICES; i++) {
        printf("\nCidade %d:\n", i);
        Grafo* temp = cidades[i].prox;
        while (temp != NULL) {
            printf("%d -> Cidade %d (Estrada %d km)\n", i , temp->cidade, temp->estrada);
            temp = temp->prox;
        }
    }
}

void liberaMemoria(Grafo* cidades) {
    for (int i = 0; i < MAX_VERTICES; i++) {
        Grafo* temp = cidades[i].prox;
        while (temp != NULL) {
            Grafo* prox = temp->prox;
            free(temp);
            temp = prox;
        }
    }
    free(cidades);
    printf("Memoria liberada com sucesso\n");
}

int find(Subconjunto* subconjuntos, int origem_destino) {
    if (subconjuntos[origem_destino].pai != origem_destino){
        subconjuntos[origem_destino].pai = find(subconjuntos, subconjuntos[origem_destino].pai);
    }
    return subconjuntos[origem_destino].pai;
}

void Union(Subconjunto* subconjuntos, int x, int y) { //União por altura
    int raizX = find(subconjuntos, x);
    int raizY = find(subconjuntos, y);

    if (subconjuntos[raizX].altura < subconjuntos[raizY].altura) { //Verifica se a raizX é menor em altura do que a raizY
        subconjuntos[raizX].pai = raizY; //Novo Pai da raizX é a raizY
    }
    else if (subconjuntos[raizX].altura > subconjuntos[raizY].altura) { //Verifica se a raizX é maior em altura do que a raizY
        subconjuntos[raizY].pai = raizX; //Novo Pai da raizY é a raizX
    }
    else {
        subconjuntos[raizY].pai = raizX; //Novo Pai da raizY é a raizX pois alturas são iguais
        subconjuntos[raizX].altura++; //Incrementa altura da raizX
    }
}

int comparaArestas(const void* a, const void* b) {
    Aresta* a1 = (Aresta*)a;
    Aresta* a2 = (Aresta*)b;
    return a1->peso > a2->peso; //Compara o peso das arestas
}

void KruskalMST(Grafo* cidades) {
    // Criar um array de arestas
    int max_arestas;
    max_arestas = (MAX_VERTICES * (MAX_VERTICES - 1))/ 2; // 10 arestas possíveis no máximo
    Aresta* arestas = (Aresta*)malloc(max_arestas * sizeof(Aresta));
    int aux = 0; //indice auxiliar

    // Preencher o array de arestas
    for (int i = 0; i < MAX_VERTICES; i++) {
        for (Grafo* temp = cidades[i].prox; temp != NULL; temp = temp->prox) {
            if (i < temp->cidade) {
                arestas[aux].origem = i; // Ele parte dele mesmo
                arestas[aux].destino = temp->cidade; // Salva o valor do vertice
                arestas[aux].peso = temp->estrada; // Salva peso da aresta
                aux++;
            }
        }
    }

    Subconjunto* subconjuntos = (Subconjunto*)malloc(MAX_VERTICES * sizeof(Subconjunto));
    if ( subconjuntos == NULL){
        printf("Erro em alocar memoria \n");
        exit(1);
    }

    Aresta* resultado = (Aresta*)malloc((MAX_VERTICES - 1) * sizeof(Aresta));
    if ( resultado == NULL){
        printf("Erro em alocar memoria \n");
        exit(1);
    }

    qsort(arestas, aux, sizeof(Aresta), comparaArestas); // Ordena as arestas em função do peso

    // Preencher o subconjunto com alturas zeradas
    for (int i = 0; i < MAX_VERTICES; ++i) {
        subconjuntos[i].pai = i;
        subconjuntos[i].altura = 0;
    }

    int qtd_arestas = 0; // Contador de arestas que serão utilizado no Kruskal
    int i = 0; // Indice para iterar as arestas que foram ordenadas

    while (qtd_arestas < MAX_VERTICES - 1 && i < aux) {
        Aresta proximaAresta = arestas[i++];

        int verticeX = find(subconjuntos, proximaAresta.origem);
        int verticeY = find(subconjuntos, proximaAresta.destino);

        if (verticeX != verticeY) { //Se os pais/raizes/vertices são iguais então não faz sentido fazer união entre eles
            resultado[qtd_arestas++] = proximaAresta; //Montando o subconjunto gerador de Kruskal
            Union(subconjuntos, verticeX, verticeY);
        }
    }

    printf("\nEstradas que serao asfaltadas:\n");
    for (i = 0; i < qtd_arestas; ++i)
        printf("%d <--> %d == %d km\n", resultado[i].origem, resultado[i].destino, resultado[i].peso);

    free(arestas);
    free(subconjuntos);
    free(resultado);
}

int main(void) {
    Grafo* cidades = (Grafo*)malloc(MAX_VERTICES * sizeof(Grafo)); // cidades = [0, 1, 2, 3, 4]
    if (cidades == NULL) {
        printf("Erro ao alocar memoria\n");
        exit(1);
    }

    // cidade 0
    Grafo* cidade01 = (Grafo*)malloc(sizeof(Grafo));
    cidade01->cidade = 1;
    cidade01->estrada = 7;

    Grafo* cidade04 = (Grafo*)malloc(sizeof(Grafo));
    cidade04->cidade = 4;
    cidade04->estrada = 1;
    cidade01->prox = cidade04;
    cidade04->prox = NULL;
    cidades[0].prox = cidade01;

    // cidade 1
    Grafo* cidade10 = (Grafo*)malloc(sizeof(Grafo));
    cidade10->cidade = 0;
    cidade10->estrada = 3;

    Grafo* cidade12 = (Grafo*)malloc(sizeof(Grafo));
    cidade12->cidade = 2;
    cidade12->estrada = 4;
    cidade10->prox = cidade12;

    Grafo* cidade14 = (Grafo*)malloc(sizeof(Grafo));
    cidade14->cidade = 4;
    cidade14->estrada = 2;
    cidade12->prox = cidade14;
    cidade14->prox = NULL;
    cidades[1].prox = cidade10;

    // cidade 2
    Grafo* cidade21 = (Grafo*)malloc(sizeof(Grafo));
    cidade21->cidade = 1;
    cidade21->estrada = 4;

    Grafo* cidade23 = (Grafo*)malloc(sizeof(Grafo));
    cidade23->cidade = 3;
    cidade23->estrada = 5;
    cidade21->prox = cidade23;

    Grafo* cidade24 = (Grafo*)malloc(sizeof(Grafo));
    cidade24->cidade = 4;
    cidade24->estrada = 6;
    cidade23->prox = cidade24;
    cidade24->prox = NULL;
    cidades[2].prox = cidade21;

    // cidade 3
    Grafo* cidade32 = (Grafo*)malloc(sizeof(Grafo));
    cidade32->cidade = 2;
    cidade32->estrada = 5;

    Grafo* cidade34 = (Grafo*)malloc(sizeof(Grafo));
    cidade34->cidade = 4;
    cidade34->estrada = 6;
    cidade32->prox = cidade34;
    cidade34->prox = NULL;
    cidades[3].prox = cidade32;

    // cidade 4
    Grafo* cidade40 = (Grafo*)malloc(sizeof(Grafo));
    cidade40->cidade = 0;
    cidade40->estrada = 1;

    Grafo* cidade41 = (Grafo*)malloc(sizeof(Grafo));
    cidade41->cidade = 1;
    cidade41->estrada = 2;
    cidade40->prox = cidade41;

    Grafo* cidade42 = (Grafo*)malloc(sizeof(Grafo));
    cidade42->cidade = 2;
    cidade42->estrada = 6;
    cidade41->prox = cidade42;

    Grafo* cidade43 = (Grafo*)malloc(sizeof(Grafo));
    cidade43->cidade = 3;
    cidade43->estrada = 6;
    cidade42->prox = cidade43;
    cidade43->prox = NULL;
    cidades[4].prox = cidade40;

    printaCidades(cidades);

    KruskalMST(cidades); //Criando a arvore geradora de Kruskal

    liberaMemoria(cidades);

    return 0;
}



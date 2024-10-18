#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <limits.h>

#define INF INT_MAX

typedef struct tipoitem{
  char caractere_mapa; 
  int indice; 
}tipoItem;

typedef struct tipoLista{
  tipoItem *vertices;
  int qtd;
}tipoLista;

typedef struct tipoGrafoMatrizAdj{
  tipoLista Lista;
  int **matrizAdj;
  int **matrizVert;
}tipoGrafoMatrizAdj;

typedef struct Fila {
  int* itens;
  int inicio;
  int fim;
  int tam;
  int capacidade;
} Fila;

void inicializaGrafo(tipoGrafoMatrizAdj *grafo, int V){
  int i, j;
  grafo->Lista.vertices = (tipoItem*)malloc(V*sizeof(tipoItem));
  grafo->Lista.qtd = 0;
}

void inicializaMatrizes(tipoGrafoMatrizAdj *grafo, int V){
  int i;
  grafo->matrizAdj = (int**)malloc(V*sizeof(int*));
  for(i=0; i<V; i++){
    grafo->matrizAdj[i] = (int*)calloc(V,sizeof(int));
  }

  grafo->matrizVert = (int**)malloc(V*sizeof(int*));
  for(i=0; i<V; i++){
    grafo->matrizVert[i] = (int*)calloc(V,sizeof(int));
  }
}

void insereNaLista(tipoLista *lista, char caractere){
  int qtd = lista->qtd;

  lista->vertices[qtd].caractere_mapa = caractere;
  lista->vertices[qtd].indice = qtd;

  lista->qtd++;
}

int buscaVertice(tipoGrafoMatrizAdj *grafo, char caracter){
  int i;
  tipoLista *lista = &grafo->Lista;
  for(i=0; i<lista->qtd; i++){
    if(lista->vertices[i].caractere_mapa == caracter)
      return i;
  }
  return -1;
}

Fila* criarFilaCircular(int capacidade) {
    Fila* fila = (Fila*)malloc(sizeof(Fila));
    fila->capacidade = capacidade;
    fila->inicio = 0;
    fila->tam = 0;
    fila->fim = capacidade - 1;
    fila->itens = (int*)malloc(fila->capacidade * sizeof(int));
    return fila;
}

int estaVazia(Fila* fila) {
  return (fila->tam == 0);
}

void enfileirar(Fila* fila, int item) {
  fila->fim = (fila->fim + 1) % fila->capacidade;
  fila->itens[fila->fim] = item;
  fila->tam++;
}

int desenfileirar(Fila* fila) {
  int item = fila->itens[fila->inicio];
  fila->inicio = (fila->inicio + 1) % fila->capacidade;
  fila->tam--;
  return item;
}

void bfs(int** grafo, int nVert, int inicio, int* dist) {
  Fila* fila = criarFilaCircular(nVert);

  dist[inicio] = 0;
  enfileirar(fila, inicio);

  while (!estaVazia(fila)) {
      int no = desenfileirar(fila);

      for (int i = 0; i < nVert; i++) {
          if (grafo[no][i] && dist[i] == INF) {
              dist[i] = dist[no] + 1;
              enfileirar(fila, i);
          }
      }
  }

  free(fila->itens);
  free(fila);
}

int obterMenorDistancia(int** grafo, int nVert, int inicio, int tesouro) {
  int* dist = (int*)malloc(nVert * sizeof(int));

  for (int i = 0; i < nVert; i++) {
    dist[i] = INF;
  }

  bfs(grafo, nVert, inicio, dist);

  int menorDistancia = dist[tesouro];
  free(dist);

  // Se a distância do destino ainda é INF, não há caminho
  if (menorDistancia == INF) {
      return -1;
  } else {
      return menorDistancia;
  }
}



int main(){
  tipoGrafoMatrizAdj *grafo = (tipoGrafoMatrizAdj*)malloc(sizeof(tipoGrafoMatrizAdj));
  int i, j, nLinhas, nVert=0, VertA, VertB;
  char **mapa;


  scanf("%i",&nLinhas);
  getchar();
  mapa = (char**)malloc(sizeof(char*)*nLinhas);
  for(i=0; i<nLinhas; i++){
    mapa[i] = (char*)malloc(sizeof(char)*80);
  }
  inicializaGrafo(grafo, nLinhas * 80);

  for(i=0; i<nLinhas; i++){
    if (fgets(mapa[i], 80, stdin) != NULL) {
      int len = strlen(mapa[i]);
      if (len > 0 && mapa[i][len - 1] == '\n') {
        mapa[i][len - 1] = '\0';
      }
    }
    for(j=0; j<strlen(mapa[i]); j++){
      if(mapa[i][j] == '*'){
        nVert++;
      }
      else if(mapa[i][j] == '+'){
        nVert++;
      }
      else if(mapa[i][j] == '$'){
        nVert++;
      }
      else if(mapa[i][j] == ' '){
        nVert++;
      }
      else{

      }
    }
  }

  inicializaMatrizes(grafo, nVert);
  nVert = 0;

  for(i=0; i<nLinhas; i++){
    for(j=0; j<strlen(mapa[i]); j++){
      if(mapa[i][j] == '*'){
        insereNaLista(&grafo->Lista, mapa[i][j]);
        grafo->matrizVert[i][j] = nVert;
        nVert++;
      }
      else if(mapa[i][j] == '+'){
        insereNaLista(&grafo->Lista, mapa[i][j]);
        grafo->matrizVert[i][j] = nVert;
        nVert++;
      }
      else if(mapa[i][j] == '$'){
        insereNaLista(&grafo->Lista, mapa[i][j]);
        grafo->matrizVert[i][j] = nVert;
        nVert++;
      }
      else if(mapa[i][j] == ' '){
        insereNaLista(&grafo->Lista, mapa[i][j]);
        grafo->matrizVert[i][j] = nVert;
        nVert++;
      }
      else{}
    }
  }

  for(i=0; i<nLinhas; i++){
    for(j=0; j<strlen(mapa[i]); j++){
      if(mapa[i][j] == '*' || mapa[i][j] == '+' || mapa[i][j] == '$' || mapa[i][j] == ' '){
        VertA = grafo->matrizVert[i][j];
        if(mapa[i-1][j] == '*' || mapa[i-1][j] == '+' || mapa[i-1][j] == '$' || mapa[i-1][j] == ' '){
          VertB = grafo->matrizVert[i-1][j];
          grafo->matrizAdj[VertA][VertB] = 1;
        }
        if(mapa[i+1][j] == '*' || mapa[i+1][j] == '+' || mapa[i+1][j] == '$' || mapa[i+1][j] == ' '){
          VertB = grafo->matrizVert[i+1][j];
          grafo->matrizAdj[VertA][VertB] = 1;
        }
        if(mapa[i][j-1] == '*' || mapa[i][j-1] == '+' || mapa[i][j-1] == '$' || mapa[i][j-1] == ' '){
          VertB = grafo->matrizVert[i][j-1];
          grafo->matrizAdj[VertA][VertB] = 1;
        }
        if(mapa[i][j+1] == '*' || mapa[i][j+1] == '+' || mapa[i][j+1] == '$' || mapa[i][j+1] == ' '){
          VertB = grafo->matrizVert[i][j+1];
          grafo->matrizAdj[VertA][VertB] = 1;
        }
      }    
    }
  }

  int vertJoao = buscaVertice(grafo, '*');
  int vertPedrinho = buscaVertice(grafo, '+');
  int vertTesouro = buscaVertice(grafo, '$');

  int distJoao = obterMenorDistancia(grafo->matrizAdj, nVert, vertJoao, vertTesouro);
  int distPedrinho = obterMenorDistancia(grafo->matrizAdj, nVert, vertPedrinho, vertTesouro);;

  
  if(distJoao == -1){
    if(distPedrinho == -1)
      printf("-1");
    else
      printf("2");
  }  
    
  else if(distPedrinho == -1){
    if(distJoao == -1)
      printf("-1");
    else
      printf("1");
  } 
    
  else if(distJoao == distPedrinho)
    printf("0");
  else if(distJoao < distPedrinho)
    printf("1");
  else if(distPedrinho < distJoao)
    printf("2");
    
  return 0;
}
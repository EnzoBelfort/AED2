#include<stdio.h>
#include<stdlib.h>

typedef struct tipoitem{
  int tempo_cadeia; 
  int indice; 
}tipoItem;

typedef struct tipoLista{
  tipoItem *vertices;
  int qtd;
}tipoLista;

typedef struct tipoGrafoMatrizAdj{
  tipoLista Lista;
  int **matrizAdj;
}tipoGrafoMatrizAdj;


void inicializaGrafo(tipoGrafoMatrizAdj *grafo, int V, int A){
  int i, j;
  grafo->Lista.vertices = (tipoItem*)malloc(V*sizeof(tipoItem));
  grafo->Lista.qtd = 0;

  grafo->matrizAdj = (int**)malloc(V*sizeof(int*));
  for(i=0; i<V; i++){
    grafo->matrizAdj[i] = (int*)calloc(V,sizeof(int));
  }
}


void insereNaLista(tipoLista *lista, int tempo){
  int qtd = lista->qtd;

  lista->vertices[qtd].tempo_cadeia = tempo;
  lista->vertices[qtd].indice = qtd;

  lista->qtd++;
}

void trocaChefia(tipoGrafoMatrizAdj *grafo, int A, int B, int *memoria){
  int i, j;

    // Atualizando a matriz de adjacência
  for (i = 0; i < grafo->Lista.qtd; i++) {
    int aux = grafo->matrizAdj[i][A];
    grafo->matrizAdj[i][A] = grafo->matrizAdj[i][B];
    grafo->matrizAdj[i][B] = aux;
  }

  for (j = 0; j < grafo->Lista.qtd; j++) {
    int aux = grafo->matrizAdj[A][j];
    grafo->matrizAdj[A][j] = grafo->matrizAdj[B][j];
    grafo->matrizAdj[B][j] = aux; 
  }

    // Invalida a memoria
  for (int i = 0; i < grafo->Lista.qtd; i++) {  
      memoria[i] = -1;
  }
}


int maiorTempo(tipoGrafoMatrizAdj *grafo, int A, int *memoria) {
  if (memoria[A] != -1) {
      return memoria[A];
  }

  int i, encontrouChefe = 0, maior = 0;

  for (i = 0; i < grafo->Lista.qtd; i++) {
      if (grafo->matrizAdj[i][A] == 1) {
          encontrouChefe++;
          if (grafo->Lista.vertices[i].tempo_cadeia > maior) {
              maior = grafo->Lista.vertices[i].tempo_cadeia;
          }
          int tempoChefe = maiorTempo(grafo, i, memoria);
          if (tempoChefe > maior) {
              maior = tempoChefe;
          }
      }
  }

  if (encontrouChefe == 0) {
      memoria[A] = 0;
  } else {
      memoria[A] = maior;
  }

  return memoria[A];
}

void consultaTempo(tipoGrafoMatrizAdj *grafo, int A, int *memoria) {
  int resultado;
  
  resultado = maiorTempo(grafo, A-1, memoria);

  if (resultado == 0) {
      printf("*\n");
  } else {
      printf("%i\n", resultado);
  }
}



int main(){
  tipoGrafoMatrizAdj *grafo = (tipoGrafoMatrizAdj*)malloc(sizeof(tipoGrafoMatrizAdj));
  int N, M, I;
  int i, Ti, X, Y, A, B;
  char comando;

  scanf("%i %i %i",&N, &M, &I);  
  inicializaGrafo(grafo, N, M);

  int *memoria = (int*)malloc(N * sizeof(int));
  for (i = 0; i < N; i++) {
      memoria[i] = -1;  
  }

  for(i=0; i<N; i++){
    scanf("%i",&Ti);
    insereNaLista(&grafo->Lista, Ti);
  }

  for(i=0; i<M; i++){
    scanf("%i %i",&X, &Y);
    grafo->matrizAdj[X-1][Y-1] = 1;
  }

  for(i=0; i<I; i++){
    scanf(" %c",&comando);
    
    if(comando == 'T'){
      scanf("%i %i",&A, &B);
      trocaChefia(grafo, A-1, B-1, memoria);
    }
    
    if(comando == 'P'){
      scanf("%i",&A);
      consultaTempo(grafo, A, memoria);
    }
  }

  free(memoria);
  free(grafo);
  return 0;
}
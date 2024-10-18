#include<stdio.h>

void selectionSort(int *vet, int N){
  int i, j, indice_menor, aux;

  for(i=0; i<N-1; i++){
    indice_menor = i;
    for(j=i+1; j<N; j++){
      if(vet[j] < vet[indice_menor]){
        indice_menor = j;
      }
    }
    aux = vet[i];
    vet[i] = vet[indice_menor];
    vet[indice_menor] = aux;
  }
}

/*    Algoritmo:
   - Selecione o indice_menor item do subvetor;
   - Troque-o com o primeiro item do subvetor;
   - Repita essas operações ate que reste apenas um elemento;

  Selection não é estável 
  Linear em trocas O(n)
  Quadratica em comparações O(n^2) em qualquer caso
  Melhor caso: custo ainda é quadrático O(n^2)
*/  
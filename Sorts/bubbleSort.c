void bubbleSort(int *vet, int N){
  int i, j, aux;

  for(i=0; i<N-1; i++){
    for(j=1; j<N-i; j++){
      if(vet[j-1]>vet[j]){
        aux = vet[j-1];
        vet[j-1] = vet[j];
        vet[j] = aux;       
      }
    }
  }
}

/*
    Algoritmo estavel
    Melhor caso: O(n)
    Pior caso: O(n^2)
    Caso Medio: O(n^2)
*/
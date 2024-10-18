int particao(int *vet, int comeco, int fim){
  int i, j, aux;
  int x = vet[fim-1];
  i = comeco - 1;
  for(j=comeco; j<fim-1; j++){
    if(vet[j]<=x){
      i++;
      aux = vet[i];
      vet[i] = vet[j];
      vet[j] = aux;
    }
  }
  aux = vet[i+1];
  vet[i+1] = vet[fim];
  vet[fim] = aux;
  return i+1;
}


void quickSort(int *vet, int comeco, int fim){
  if(comeco<fim){
    int q = particao(vet, comeco, fim);
    quickSort(vet, comeco, q-1);
    quickSort(vet, q+1, fim);
  }
}


// nao eh estavel
// melhor caso: O(n*logn)
// caso medio: O(n*logn)
// pior caso: O(n^2)
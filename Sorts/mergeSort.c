void merge(int *vet, int comeco, int meio, int fim){
  int i, j;
  int tam1 = meio - comeco + 1;
  int tam2 = fim - meio + 2;
  int L[tam1], R[tam2];

  for(i=0; i<tam1; i++)
    L[i] = vet[comeco + i];
  for(i=0; i<tam2; i++)
    R[i] = vet[meio + i];

  L[tam1 - 1] = 32000;
  R[tam2 - 1] = 32000;

  i=0; j=0;
  for(int k=comeco; k<=fim ; k++){
    if(L[i]<=R[j]){
      vet[k]=L[i];
      i++;
    }
    else{
      vet[k]=R[j];
      j++;
    }
  }
}

void mergeSort(int *vet, int comeco, int fim){
  if(comeco < fim){
    int meio = (comeco + fim)/2;
    mergeSort(vet, comeco, meio);
    mergeSort(vet, meio + 1, fim);
    merge(vet, comeco, meio, fim);
  }
}

// estavel
// O(n*logn)
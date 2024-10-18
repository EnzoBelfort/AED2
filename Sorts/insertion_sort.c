// #include <stdio.h>


// void insertion_sort (vet[]){
//   int i, j, n, aux;

//   for(i=1;i<n;i++){
//     j = i-1;
//     aux = vet[i];
//     while(j>=0 && vet[j]>aux){
//       vet[j+1] = vet[j]; 
//       j--;
//     }
//     vet[j+1] = aux;
//   }
// }


void insertionSort(int *vet, int tam){
  int i, j, aux;

  for(i=1; i<tam; i++){
    j = i-1;
    aux = vet[i];
    while(j>=0 && aux<vet[j]){
      vet[j+1] = vet[j];
      j--;
    }  
    vet[j+1] = aux;
  }
}

/*   
   Algoritmo:
    - A partir da 2ª posição do vet (i=1)
    - selecione o i-esimo item da sequencia fonte
    - coloque-o no lugar apropriado na sequencia destino de acordo com o criterio de ordenacao

   Ordem de compelxidade:
    - Melhor caso: n-1, ou seja, O(n)
    - Pior caso: O(n^2)
    - Caso medio: O(n^2)

   Algoritmo estavel
   excelente para arquivos quase ordenados
*/
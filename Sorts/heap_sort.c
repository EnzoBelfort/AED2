#include<stdio.h>
#include<stdlib.h>

typedef struct{
  int *vet;
  int N;
  int size;
}heap_type;

void print(heap_type *A){
  int i;
  printf("***\n");
  for(i=0;i<A->N;i++)
    printf("%d\n",A->vet[i]);
}

void exchange(int *vet, int i, int j){
  int aux;
  aux = vet[i];
  vet[i] = vet[j];
  vet[j] = aux;
}

void max_heapfy(heap_type *A, int i){
  int l = 2*i+1;
  int r = 2*i+2;
  int largest;
  if(l < A->size && A->vet[l] > A->vet[i])
    largest = l;
  else 
    largest = i;
  if(r < A->size && A->vet[r] > A->vet[largest]){
    largest = r;
  }
  if(largest != i){
    exchange(A->vet, i, largest);
    max_heapfy(A, largest);
  }
}

void build_max_heap(heap_type *A){
  A->size = A->N;
  int i;
  for(i = A->N/2-1; i>=0; i--){
    max_heapfy(A,i);
  }
}

void heapsort(heap_type *A){
  int i;
  build_max_heap(A);
  for(i=A->N-1; i>=1; i++){
    exchange(A->vet,0,i);
    A->size--;
    max_heapfy(A, 0);
  }
  print(A);
}

int main(){
  heap_type A;
  int i;
  scanf("%d",&A.N);
  A.vet = malloc(sizeof(int) * A.N);
  
  for(i=0; i<A.N; i++){
    scanf("%d",&A.vet[i]);
  }

  heapsort(&A);
}


// nao eh estavel
// sempre O(n * logn)
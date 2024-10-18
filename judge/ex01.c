#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void transf_minusculo(char *palavra){
  int i;
  for(i=0; i<strlen(palavra); i++)
    if(palavra[i] >= 'A' && palavra[i] <= 'Z')
      palavra[i] = palavra[i] + 32;
}


void prints(char **vet, int comeco, int quantidades_prints){
  int i;
  for(i = comeco-1; i < comeco + quantidades_prints - 1; i++){
    printf("%s\n",vet[i]);
  }
}

void print_counting(int *vet){
  int i;
  for(i=0; i<27; i++)
    printf("%i ",vet[i]);
}

int converte(char caractere){
  if(caractere == '\0')
    return 0;
  else 
    return caractere - 'a' + 1;
}


void contingSort(char **strings, int tam, int max_caractere, int indice_atual){
  int i, j, indice_caractere;
  char **strings_ordenadas = (char **)calloc(tam, sizeof(char *));
  int C[27] = {0};

  for(i=0; i<tam; i++){
    indice_caractere = converte(strings[i][max_caractere - indice_atual - 1]);
    C[indice_caractere]++;
  }

  for(i=0; i<27; i++){
    C[i+1] += C[i]; 
  }
  print_counting(C);
  printf("\n");

  for(i = tam - 1; i >= 0; i--){
    indice_caractere = converte(strings[i][max_caractere - indice_atual - 1]);
    strings_ordenadas[C[indice_caractere] - 1] = strings[i];
    C[indice_caractere]--;
  }

  for(i=0; i < tam; i++)
    strings[i] = strings_ordenadas[i];
  free(strings_ordenadas);
}

void radixSort(char **strings, int tam, int max){
  int i;
  for(i=0; i<max; i++){
    contingSort(strings, tam, max, i);
  }
}


int main(){
  int i, N, P, M, max=0;
  char **strings;

  scanf("%i",&N);
  strings = malloc(N*sizeof(char*));

  for(i=0; i<N; i++){
    strings[i] = calloc(16,sizeof(char));
    scanf("%s",strings[i]);
    if(strlen(strings[i]) > max)
      max = strlen(strings[i]);
    transf_minusculo(strings[i]);
  }

  scanf("%i %i",&P, &M);
  radixSort(strings, N, max);

  prints(strings, P, M);

  return 0;
}
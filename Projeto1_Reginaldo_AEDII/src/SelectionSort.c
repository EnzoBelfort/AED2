#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Imprime um vetor de números inteiros, um elemento por linha. */
void print(int vet[], int N)
{
  int i;
  for (i = 0; i < N; i++)
    printf("%i\n", vet[i]);
}

/* Imprime um vetor de strings, um elemento por linha. */
void prints(char **nome, int N)
{
  int i;
  for (i = 0; i < N; i++)
    puts(nome[i]);
}

void selection_sort(int vet[], int N)
{
  int i, j, aux;
  for (i = 0; i < N - 1; i++)
  {
    for (j = i; j < N; j++)
    {
      if (vet[i] > vet[j])
      {
        aux = vet[i];
        vet[i] = vet[j];
        vet[j] = aux;
      }
    }
  }
}

/* Selection sort para vetores de strings. */
void sselection_sort(char **nome, int N)
{
  int i, j;
  char *aux;
  for (i = 0; i < N - 1; i++)
  {
    for (j = i; j < N; j++)
    {
      if (strcmp(nome[i], nome[j]) > 0)
      {
        aux = nome[i];
        nome[i] = nome[j];
        nome[j] = aux;
      }
    }
  }
}

int main()
{
  int *vet, i, N, max;
  char c[2], **nome;
  /* Ver FORMATO DOS CASOS DE TESTE.txt para detalhes */
  scanf("%i", &N);
  scanf("%1s", c);
  scanf("%i", &max);
  if (c[0] == 'i')
  {
    vet = (int *)calloc(N, sizeof(int));
    for (i = 0; i < N; i++)
      scanf("%d", &vet[i]);
    selection_sort(vet, N);
    print(vet, N);
  }
  else
  {
    nome = calloc(N, sizeof(char *));
    getchar();
    for (i = 0; i < N; i++)
    {
      nome[i] = (char *)calloc((max + 2), sizeof(char));
      fgets(nome[i], max + 2, stdin);
      nome[i][strlen(nome[i]) - 1] = '\0';
    }
    sselection_sort(nome, N);
    prints(nome, N);
  }
  return 0;
}
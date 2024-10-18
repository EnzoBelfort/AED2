#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Imprime um vetor de números inteiros, um elemento por linha. */
void print(int *vetor, int N)
{
  int i;
  for (i = 0; i < N; i++)
    printf("%i\n", vetor[i]);
}

/* Imprime um vetor de strings, um elemento por linha. */
void prints(char **nome, int N)
{
  int i;
  for (i = 0; i < N; i++)
  {
    puts(nome[i]);
  }
}

/* Mescla dois subvetores de números inteiros ordenados, delimitados
    pelos argumentos inicio, meio e fim, de forma
    a produzir um vetor ordenado, sobre-escrevendo
    o vetor original passado como argumento. */
void merge(int *vetor, int inicio, int meio, int fim)
{
  int i, j = inicio, k = meio;
  int *vet1 = calloc(fim - inicio, sizeof(int));
  for (i = 0; i < fim - inicio; i++)
  {
    if (j < meio && k < fim)
    {
      if (vetor[j] <= vetor[k])
      {
        vet1[i] = vetor[j];
        j++;
      }
      else
      {
        vet1[i] = vetor[k];
        k++;
      }
    }
    else if (j < meio)
    {
      vet1[i] = vetor[j];
      j++;
    }
    else
    {
      vet1[i] = vetor[k];
      k++;
    }
  }
  for (i = inicio, k = 0; i < fim; i++, k++)
  {
    vetor[i] = vet1[k];
  }
  free(vet1);
}

/* Equivalente da função "merge" para vetores de strings. */
void merges(char **nome, int inicio, int meio, int fim)
{
  int i, j = inicio, k = meio;
  char **aux = (char **)calloc(fim - inicio, sizeof(char *));
  for (i = 0; i < fim - inicio; i++)
  {
    if (j < meio && k < fim)
    {
      if (strcmp(nome[j], nome[k]) < 1)
      {
        aux[i] = nome[j];
        j++;
      }
      else
      {
        aux[i] = nome[k];
        k++;
      }
    }
    else if (j < meio)
    {
      aux[i] = nome[j];
      j++;
    }
    else
    {
      aux[i] = nome[k];
      k++;
    }
  }
  for (i = inicio, k = 0; i < fim; i++, k++)
  {
    nome[i] = aux[k];
  }
  free(aux);
}

void mergesort(int *vetor, int start, int end)
{
  if (end - start <= 1)
    return;
  int meio = start + (end - start) / 2;
  mergesort(vetor, start, meio);
  mergesort(vetor, meio, end);
  merge(vetor, start, meio, end);
}

/* Função mergesort para vetores de strings. */
void mergesorts(char **nome, int start, int end)
{
  if (end - start <= 1)
    return;
  int meio = start + (end - start) / 2;
  mergesorts(nome, start, meio);
  mergesorts(nome, meio, end);
  merges(nome, start, meio, end);
}

int main()
{
  char c[2], **nome;
  int i, *vet, N, max;
  /* Ver FORMATO DOS CASOS DE TESTE.txt para detalhes */
  scanf("%i", &N);
  scanf("%1s", c);
  scanf("%i", &max);
  if (c[0] == 'i')
  {
    vet = (int *)calloc(N, sizeof(int));
    for (i = 0; i < N; i++)
      scanf("%i", &vet[i]);
    mergesort(vet, 0, N);
    print(vet, N);
  }
  else if (c[0] == 's')
  {
    nome = (char **)calloc(N, sizeof(char *));
    getchar();
    for (i = 0; i < N; i++)
    {
      nome[i] = (char *)calloc(max, sizeof(char));
      fgets(nome[i], max + 2, stdin);
      nome[i][strlen(nome[i]) - 1] = '\0'; /* Remover \n do final */
    }
    mergesorts(nome, 0, N);
    prints(nome, N);
  }
  return 0;
}
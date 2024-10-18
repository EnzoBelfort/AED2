#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Imprime um vetor de números inteiros, um elemento por linha. */
void print(int *vet, int N)
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
  {
    puts(nome[i]);
  }
}

/* 
Calcula o índice não negativo em que a contagem do
caractere <letra> será armazenada no vetor de contagem
do algoritmo countingsort para vetores de strings.
Os caracteres são mapeados da seguinte maneira:
A-Z: números de 1 a 26
\0 e espaço: 0 (zero)
 */
int convert(char letra)
{
  int i;
  if (letra == ' ' || letra == '\0')
    return 0;
  if (letra >= 'A' && letra <= 'Z')
    i = (letra - 'A' + 1);
  return i;
}

/* Realiza o countingsort em um vetor de inteiros, considerando apenas
    o algarismo dado por p, onde dec = 10**p. */
void CountingSort(int *vetor, int N, int dec)
{
  int vet[10] = {0};
  int *ordenado = (int *)calloc(N, sizeof(int));
  int j, a;
  for (j = 0; j < N; j++)
  {
    a = (vetor[j] / dec) % 10;
    vet[a]++;
  }
  for (j = 0; j < 9; j++)
  {
    vet[j + 1] += vet[j];
  }
  for (j = N - 1; j >= 0; j--)
  {
    a = (vetor[j] / dec) % 10;
    ordenado[vet[a] - 1] = vetor[j];
    vet[a]--;
  }
  for (j = 0; j < N; j++)
    vetor[j] = ordenado[j];
  free(ordenado);
}

/* Realiza o countingsort em um vetor de strings, considerando apenas
    o caractere dado por p, onde p=0 representa o último caractere da
    string preenchida com espaços à sua direita até que seu tamanho seja
    igual a max, e p=max-1 é o primeiro caractere da string. */
void CountingSorts(char **nome, int N, int max, int p)
{
  int vet[27] = {0};
  char **ordenado = (char **)calloc(N, sizeof(char *));
  int j, c;
  for (j = 0; j < N; j++)
  {
    /*
    IMPORTANTE: Como as strings foram alocadas por calloc, elas já
    estão todas preenchidas com \0 até seus tamanhos serem iguais a max.
    Por conta disso, não é necessário calcular o verdadeiro índice
    dentro das strings, como nas implementações das outras linguagens de programação.
    */
    c = convert(nome[j][max - p - 1]);
    vet[c]++;
  }
  for (j = 0; j < 26; j++)
  {
    vet[j + 1] += vet[j];
  }
  for (j = N - 1; j >= 0; j--)
  {
    c = convert(nome[j][max - p - 1]);
    ordenado[vet[c] - 1] = nome[j];
    vet[c]--;
  }
  for (j = 0; j < N; j++)
    nome[j] = ordenado[j];
  free(ordenado);
}

/* Realiza o radixsort em um vetor de inteiros.
    max é o número de algarismos do maior número presente. */
void RadixSort(int *vet, int N, int max)
{
  int i, dec = 1;
  for (i = 0; i < max; i++)
  {
    CountingSort(vet, N, dec);
    dec *= 10;
  }
}

/* Realiza o radixsort em um vetor de strings.
    max é o número máximo de caracteres entre os elementos do vetor. */
void RadixSorts(char **nome, int N, int max)
{
  int i;
  for (i = 0; i < max; i++)
  {
    CountingSorts(nome, N, max, i);
  }
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
    RadixSort(vet, N, max);
    print(vet, N);
  }
  else if (c[0] == 's')
  {
    nome = (char **)calloc(N, sizeof(char *));
    getchar();
    for (i = 0; i < N; i++)
    {
      nome[i] = (char *)calloc(max + 2, sizeof(char));
      fgets(nome[i], max + 2, stdin);
      nome[i][strlen(nome[i]) - 1] = '\0';
    }
    RadixSorts(nome, N, max);
    prints(nome, N);
  }
  return 0;
}
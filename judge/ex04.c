#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct no_qtd {
  int qtd;
  struct no_qtd *prox;
} no_qtd;

typedef struct no {
  char *chave;
  no_qtd *lista_qtd;
  struct no *prox;
} no;

typedef struct tabela {
  int m;
  no **lista_encadeada;
} hash;

void inicializaHash(hash *tabela, int m) {
  tabela->m = m;
  tabela->lista_encadeada = (no **)malloc(sizeof(no *) * m);
  for (int i = 0; i < m; i++) {
    tabela->lista_encadeada[i] = NULL;
  }
}

void criaNo(no *n, char *chave, int qtd_prod) {
  n->chave = strdup(chave);
  n->lista_qtd = (no_qtd *)malloc(sizeof(no_qtd));
  n->lista_qtd->qtd = qtd_prod;
  n->lista_qtd->prox = NULL;
  n->prox = NULL;
}

void criaNoQtd(no_qtd *novoNoQtd, int qtd_prod) {
  novoNoQtd->prox = NULL;
  novoNoQtd->qtd = qtd_prod;
}

int ASCII_INT(char *nome, int m) {
  int i, inteiro_transformado = 0;
  for (i = 0; i < strlen(nome); i++) {
    inteiro_transformado = (inteiro_transformado * 128 + nome[i]) % m;
  }
  return inteiro_transformado;
}

// Função de Bubble Sort para ordenar os nós na lista encadeada
void bubbleSort(no **cabeca) {
  if (*cabeca == NULL) return;

  int trocado;
  no *aux1;
  no *limite = NULL;

  do {
    trocado = 0;
    aux1 = *cabeca;

    while (aux1->prox != limite) {
      if (strcmp(aux1->chave, aux1->prox->chave) > 0) {
        // Trocar os nós
        char *chaveTemp = aux1->chave;
        aux1->chave = aux1->prox->chave;
        aux1->prox->chave = chaveTemp;

        no_qtd *listaTemp = aux1->lista_qtd;
        aux1->lista_qtd = aux1->prox->lista_qtd;
        aux1->prox->lista_qtd = listaTemp;

        trocado = 1;
      }
      aux1 = aux1->prox;
    }
    limite = aux1;
  } while (trocado);
}

void insere(hash *tabela, char *nome, int qtd_prod) {
  int indiceChave = ASCII_INT(nome, tabela->m);

  no *aux = tabela->lista_encadeada[indiceChave];
  no *anterior = NULL;

  // Procurar se o nome já está na lista
  while (aux != NULL) {
    if (strcmp(nome, aux->chave) == 0) {
      // Nome encontrado, adicionar quantidade na lista de quantidades
      no_qtd *inicio = aux->lista_qtd;
      no_qtd *anteriorQtd = NULL;

      // Percorre a lista até encontrar o final
      while (inicio != NULL) {
        anteriorQtd = inicio;
        inicio = inicio->prox;
      }

      no_qtd *novoNoQtd = (no_qtd *)malloc(sizeof(no_qtd));
      criaNoQtd(novoNoQtd, qtd_prod);

      if (anteriorQtd == NULL) {
        aux->lista_qtd = novoNoQtd;
      } else {
        anteriorQtd->prox = novoNoQtd;
      }
      return;
    }
    anterior = aux;
    aux = aux->prox;
  }

  // Nome não encontrado, criar novo nó
  no *novoNo = (no *)malloc(sizeof(no));
  criaNo(novoNo, nome, qtd_prod);

  if (anterior == NULL) {
    tabela->lista_encadeada[indiceChave] = novoNo;
  } else {
    anterior->prox = novoNo;
  }
}

void busca(hash *tabela_hash, char *nome) {
  int indiceChave = ASCII_INT(nome, tabela_hash->m);
  no *aux = tabela_hash->lista_encadeada[indiceChave];

  // Ordenar a lista
  bubbleSort(&tabela_hash->lista_encadeada[indiceChave]);

  // Imprimir todos os nós ordenados
  aux = tabela_hash->lista_encadeada[indiceChave];
  while (aux != NULL) {
    printf("%s ", aux->chave);
    aux = aux->prox;
  }
  printf("\n");

  // Buscar o nó específico e imprimir suas quantidades
  aux = tabela_hash->lista_encadeada[indiceChave];
  while (aux != NULL) {
    if (strcmp(aux->chave, nome) == 0) {
      no_qtd *inicio = aux->lista_qtd;
      while (inicio != NULL) {
        printf("%i ", inicio->qtd);
        inicio = inicio->prox;
      }
      printf("\n");
      return;
    }
    aux = aux->prox;
  }
  printf("0\n");
}

int main() {
  int m, operacao, qtd_prod;
  char nome[15];
  hash *tabela = (hash *)malloc(sizeof(hash));

  scanf("%i", &m);
  inicializaHash(tabela, m);

  scanf("%i", &operacao);
  while (operacao != 0) {
    if (operacao == 1) {
      scanf("%s", nome);
      getchar();
      scanf(" %i", &qtd_prod);

      insere(tabela, nome, qtd_prod);
    } else {
      scanf("%s", nome);
      getchar();
      busca(tabela, nome);
    }
    scanf("%i", &operacao);
  }

  return 0;
}

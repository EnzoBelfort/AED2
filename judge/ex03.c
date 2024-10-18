#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct no {
  char *chave;
  char *valor;
  struct no *prox;
} no;

typedef struct tabela_hash {
  int numElementos, capacidade;
  no **lista_encadeada; // lista de ponteiros de nos
} hash;

void criaNo(no *no, char *chave, char *valor) {
  no->chave = strdup(chave);  // aloca e copia a string
  no->valor = strdup(valor);  
  no->prox = NULL;
}

void inicializaHash(hash *tabela_hash, int tam) {
  tabela_hash->capacidade = tam;
  tabela_hash->numElementos = 0;
  tabela_hash->lista_encadeada = (no **)malloc(sizeof(no *) * tabela_hash->capacidade);
  for (int i = 0; i < tabela_hash->capacidade; i++) {
      tabela_hash->lista_encadeada[i] = NULL; 
  }
}

int funcaoHash(hash *tabela_hash, char *chave) {
  int hash = 0;
  int A = 101; // primo
  for (int i = 0; chave[i] != '\0'; i++) {
      hash = (hash * A + chave[i]) % tabela_hash->capacidade;
  }
  return hash % tabela_hash->capacidade;
}


void inserir(hash *tabela_hash, char *chave, char *valor) {
  int indiceChave = funcaoHash(tabela_hash, chave);
  no *novoNo = (no *)malloc(sizeof(no));

  criaNo(novoNo, chave, valor);

  // sem colisão
  if (tabela_hash->lista_encadeada[indiceChave] == NULL) {
    tabela_hash->lista_encadeada[indiceChave] = novoNo;
  }

  // Colisão
  else {
    novoNo->prox = tabela_hash->lista_encadeada[indiceChave];
    tabela_hash->lista_encadeada[indiceChave] = novoNo;
  }
}

char *busca(hash *tabela_hash, char *chave) {
    int indiceChave = funcaoHash(tabela_hash, chave);
    no *aux = tabela_hash->lista_encadeada[indiceChave];
    while (aux != NULL) {
        if (strcmp(aux->chave, chave) == 0) { 
            return aux->valor;
        }
        aux = aux->prox;
    }
    return chave; // Retorna a própria chave se não encontrado
}

int qtd_espacos(char *str){
    int espacos = 0;
    while(*str){
        if (*str == ' '){
           espacos++;
        }
        str++;
    }
    return espacos;
}


int main() {
  int M, N, i, j, k;
  hash *tabela_hash;
  char original[101];
  char traduzida[101];
  char linha[101];
  char palavra_linha[101];

  tabela_hash = (hash *)malloc(sizeof(hash));

  scanf("%i %i", &M, &N);
  inicializaHash(tabela_hash, M);
  getchar(); 

  for (i = 0; i < M; i++) {
    scanf("%s", original);
    getchar(); 
    fgets(traduzida, sizeof(traduzida), stdin);
    traduzida[strcspn(traduzida, "\n")] = '\0'; 

    inserir(tabela_hash, original, traduzida);
  }

  int qtd_strings;
  char *strings;

  char **linhas = (char **)malloc(N * sizeof(char *));
  for (i = 0; i < N; i++) {
    fgets(linha, sizeof(linha), stdin);
    linha[strcspn(linha, "\n")] = '\0'; 
    linhas[i] = strdup(linha); 
  }

  for(i=0; i < N; i++){
    k=0;
    for(j=0; j<=strlen(linhas[i]); j++){
      if(linhas[i][j] != ' ' && linhas[i][j] != '\0'){
        palavra_linha[k] = linhas[i][j];
        k++;
      }
      else{
        k = 0;
        printf("%s ",busca(tabela_hash, palavra_linha));
        memset(palavra_linha, 0, sizeof(palavra_linha));
      }

    }
    printf("\n");
  } 

  // Liberar memoria alocada
  for (i = 0; i < N; i++) {
    free(linhas[i]);
  }
  free(linhas);

  for (i = 0; i < tabela_hash->capacidade; i++) {
    no *current = tabela_hash->lista_encadeada[i];
    while (current != NULL) {
      no *temp = current;
      current = current->prox;
      free(temp->chave);
      free(temp->valor);
      free(temp);
    }
  }
  free(tabela_hash->lista_encadeada);
  free(tabela_hash);

  return 0;
}
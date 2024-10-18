#include<stdio.h>
#include<stdlib.h>

typedef struct lista_vetores{
  int *vetor_ordenado;
  int tam;
  int inicio;
}lista_vetores;


void remove_menor(lista_vetores *lista, int *vetor_menores, int k){
  int i;
  int menor = vetor_menores[0];
  for(i=0; i<k; i++){   
    if(lista[i].tam != 0 && lista[i].tam != lista[i].inicio){
      if(lista[i].vetor_ordenado[0] == menor){
        lista[i].inicio++;      
      }
    }   
  }
}


void ordena(int *vetor_menores, int k, int cont){
  int i, j, aux;

  for(i=1; i<k-cont; i++){
    j = i-1;
    aux = vetor_menores[i];
    while(j>=0 && aux<vetor_menores[j]){
      vetor_menores[j+1] = vetor_menores[j];
      j--;
    }  
    vetor_menores[j+1] = aux;
  }
}

void printa(int *vetor_menores, int k, int cont){
  int i;
  for(i=0; i<k-cont; i++){
    printf("%i ",vetor_menores[i]);
  }
}



int main(){
  int k, i, tam, num, j, l, h, cont=0;
  int *vetor_menores;
  lista_vetores *lista;

  scanf("%i %i",&k, &i);
  lista = malloc(k * sizeof(lista_vetores));
  for(j=0; j<k; j++){
    scanf("%i",&tam);
    lista[j].tam = tam;
    if(tam!=0){
      lista[j].vetor_ordenado = (int*)malloc(tam * sizeof(int));
      lista[j].inicio = 0;
      for(l=0; l<tam; l++){
        scanf("%i",&num);
        lista[j].vetor_ordenado[l] = num;
      }
    }
    else{
      cont++;
    }
  }

  // for(j=0; j<k; j++){
  //   for(l=0; l<lista[j].tam; l++){                         //checar a formação da matriz de vetores
  //     printf("%i\t",lista[j].vetor_ordenado[l]);
  //   }
  //   printf("          %i",lista[j].tam);
  //   printf("\n");
  // }

  vetor_menores = malloc((k-cont)*sizeof(int));
  h=0;
  for(j=0; j<k; j++){
    if(lista[j].tam != 0){
      vetor_menores[h] = lista[j].vetor_ordenado[0];
      h++;
    }  
  }

  ordena(vetor_menores, k, cont);

  if(i==1){
    printa(vetor_menores, k, cont);
  }
  else{
    for(l=1; l<i; l++){
      remove_menor(lista, vetor_menores, k);
      h=0;
      for(j=0; j<k; j++){
        if(lista[j].tam != 0){
          if(lista[j].tam != lista[j].inicio){
            vetor_menores[h] = lista[j].vetor_ordenado[lista[j].inicio];
            h++;
          }
          else{
            cont++;
          }
        }  
      }
      ordena(vetor_menores, k, cont);
    }


    // for(j=0; j<k; j++){
    //   for(l=lista[j].inicio; l<lista[j].tam; l++){                         //checar a matriz final
    //     printf("%i\t",lista[j].vetor_ordenado[l]);
    //   }
    //   printf("\n");
    // }

    printa(vetor_menores, k, cont);
  }

  return 0;
}
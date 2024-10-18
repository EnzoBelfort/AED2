int buscaBinaria (int *vet, int comeco, int fim, int busca){
  int meio;
    
  if(fim - comeco == 0){
    if(vet[comeco] == busca)
      return comeco;
    else
      return -1;
  }
    
  else if(comeco < fim){
    meio = (comeco + fim)/2;
    if(vet[meio]==busca)
      return meio;
    else if(vet[meio] < busca)
      return buscaBinaria(vet, meio+1, fim, busca);
    else
      return buscaBinaria(vet, comeco, meio-1, busca);
  }
  else
    return -1;
   
}


// C(n) = log n
// CASA


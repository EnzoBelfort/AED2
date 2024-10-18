void countingSort(int *vet, int n) {
    int ordenado[n];
    int vet_cont[10] = {0};

    // Contagem das ocorrências dos dígitos
    for (int i = 0; i < n; i++) {
      vet_cont[vet[i]]++;
    }

    // Soma acumulada das contagens
    for (int i = 1; i < 10; i++) {
        vet_cont[i] += vet_cont[i - 1];
    }

    // Preenchimento do vetor ordenado
    for (int i = n - 1; i >= 0; i--) {
      ordenado[vet_cont[vet[i]]] = vet[i];
      vet_cont[vet[i]]--;
    }

    // Copia o vetor ordenado de volta para o vetor original
    for (int i = 0; i < n; i++) {
        vet[i] = ordenado[i];
    }
}


// linear: O(n + k) = O(n)
// estavel
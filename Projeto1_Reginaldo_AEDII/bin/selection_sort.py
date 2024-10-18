def prints(nome, N):
    '''
    Imprime a lista, um elemento por linha.
    '''
    for i in range(N):
        print(nome[i])


def selectionSort(vet, tam):

    for i in range(tam):
        indice_min = i  # inicia o primeiro indice do subvetor como mínimo

        for j in range(i + 1, tam):    # seleciona o elemento mínimo a cada iteração
            if vet[j] < vet[indice_min]:
                indice_min = j
        vet[i], vet[indice_min] = vet[indice_min], vet[i]  # inverte elementos


def main():
    # Ver FORMATO DOS CASOS DE TESTE.txt para detalhes
    entrada = input()
    variaveis = entrada.split()
    N = variaveis[0]
    c = variaveis[1]
    max_length = variaveis[2]

    N = int(N)

    if c == 'i':
        vet = [int(input()) for _ in range(N)]
        selectionSort(vet, N)
        prints(vet, N)
    else:
        nome = []
        for _ in range(N):
            nome.append(input().strip())
        # Os operadores de ordem (<, >, <=, >=) também funcionam nas strings do Python.
        selectionSort(nome, N)
        prints(nome, N)


if __name__ == "__main__":
    main()

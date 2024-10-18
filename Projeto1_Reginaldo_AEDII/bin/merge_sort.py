def prints(nome, N):
    '''
    Imprime a lista, um elemento por linha.
    '''
    for i in range(N):
        print(nome[i])


def merge(vetor, inicio, meio, fim):
    '''
    Mescla dois subvetores ordenados, delimitados
    pelos argumentos inicio, meio e fim, de forma
    a produzir um vetor ordenado, sobre-escrevendo
    o vetor original passado como argumento.
    '''
    i, j, k = 0, inicio, meio
    vet1 = [0] * (fim - inicio)

    for i in range(fim - inicio):
        if j < meio and k < fim:
            if vetor[j] <= vetor[k]:
                vet1[i] = vetor[j]
                j += 1
            else:
                vet1[i] = vetor[k]
                k += 1

        elif j < meio:
            vet1[i] = vetor[j]
            j += 1

        else:
            vet1[i] = vetor[k]
            k += 1

    k = 0
    for i in range(inicio, fim):
        vetor[i] = vet1[k]
        k += 1


def mergesort(vetor, start, end):
    if end - start <= 1:
        return
    meio = start + (end - start) // 2
    mergesort(vetor, start, meio)
    mergesort(vetor, meio, end)
    merge(vetor, start, meio, end)


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
        mergesort(vet, 0, N)
        prints(vet, N)
    else:
        nome = []
        for _ in range(N):
            nome.append(input().strip())
        # Os operadores de ordem (<, >, <=, >=) também funcionam nas strings do Python.
        mergesort(nome, 0, N)
        prints(nome, N)


if __name__ == "__main__":
    main()

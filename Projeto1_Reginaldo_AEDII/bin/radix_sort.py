def prints(nome, N):
    '''
    Imprime a lista, um elemento por linha.
    '''
    for i in range(N):
        print(nome[i])


def countingSort(vet, exp1):
    '''
    Realiza o countingsort em um vetor de inteiros, considerando apenas
    o algarismo dado por a, onde exp1 = 10**a.
    '''
    n = len(vet)

    ordenado = [0] * (n)

    vet_cont = [0] * (10)

    for i in range(0, n):
        indice = (vet[i]/exp1)
        vet_cont[int((indice) % 10)] += 1

    for i in range(1, 10):
        vet_cont[i] += vet_cont[i-1]

    i = n-1
    while i >= 0:
        indice = (vet[i]/exp1)
        ordenado[vet_cont[int((indice) % 10)] - 1] = vet[i]
        vet_cont[int((indice) % 10)] -= 1
        i -= 1

    i = 0
    for i in range(0, len(vet)):
        vet[i] = ordenado[i]


def radixSort(vet, max_tam):
    '''
    Realiza o radixsort em um vetor de inteiros.
    max_tam é o número de algarismos do maior número presente.
    '''
    exp = 1
    while max_tam > 0:
        countingSort(vet, exp)
        exp *= 10
        max_tam -= 1

     # Para string:


def indice_char(string, max, i):
    '''
    Calcula o índice não negativo em que a contagem do
    caractere string[i] será armazenada no vetor de contagem
    do algoritmo countingsort para vetores de strings.
    i=0 representa o último caractere da string preenchida com espaços 
    à sua direita até que seu tamanho seja igual a max, e i=max-1 é o
    primeiro caractere da string. Os caracteres são mapeados da seguinte maneira:
    A-Z: números de 1 a 26
    Outros caracteres: 0 (zero)
    '''
    indice = 0
    dif_tam = max - len(string)

    if i >= dif_tam:
        indice_a_partir_do_final = i - dif_tam
        c = string[len(string) - 1 - indice_a_partir_do_final]
    else:
        c = ' '

    if c == ' ':
        indice = 0

    if 'A' <= c <= 'Z':
        indice = ord(c) - ord('A') + 1

    return indice


def ScountingSort(nome, N, max, i):
    '''
    Realiza o countingsort em um vetor de strings, considerando apenas
    o caractere dado por i, onde i=0 representa o último caractere da
    string preenchida com espaços à sua direita até que seu tamanho seja
    igual a max, e i=max-1 é o primeiro caractere da string.
    '''
    vet = [0] * 27
    ordenado = [None] * N
    for j in range(N):
        c = indice_char(nome[j], max, i)
        vet[c] += 1
    for j in range(1, 27):
        vet[j] += vet[j - 1]
    for j in range(N - 1, -1, -1):
        c = indice_char(nome[j], max, i)
        ordenado[vet[c] - 1] = nome[j]
        vet[c] -= 1
    for j in range(N):
        nome[j] = ordenado[j]
    a = 1


def SradixSort(nomes, N, max):
    '''
    Realiza o radixsort em um vetor de strings.
    max é o número máximo de caracteres entre os elementos do vetor.
    '''
    for i in range(max):
        ScountingSort(nomes, N, max, i)


def main():
    # Ver FORMATO DOS CASOS DE TESTE.txt para detalhes
    entrada = input()
    variaveis = entrada.split()
    N = variaveis[0]
    c = variaveis[1]
    max_tam = variaveis[2]

    N = int(N)
    max_tam = int(max_tam)

    if c == 'i':
        vet = [int(input()) for _ in range(N)]
        radixSort(vet, max_tam)
        prints(vet, N)
    else:

        nomes = []
        for _ in range(N):
            entrada = input().rstrip('\n')
            nomes.append(entrada)
        SradixSort(nomes, N, max_tam)
        prints(nomes, N)


if __name__ == "__main__":
    main()

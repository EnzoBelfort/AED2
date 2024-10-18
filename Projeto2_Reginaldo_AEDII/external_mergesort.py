import sys
from io import TextIOBase
from tempfile import mkdtemp
from pathlib import Path
import shutil
from typing import Callable


DEBUG = False


def counting_sort(vet, exp1):
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


def radix_sort(vet, max_tam):
    '''
    Realiza o radixsort em um vetor de inteiros.
    max_tam é o número de algarismos do maior número presente.
    '''
    exp = 1
    while max_tam > 0:
        counting_sort(vet, exp)
        exp *= 10
        max_tam -= 1


def ler_registro(arq: TextIOBase) -> tuple[int, int, int] | None:
    '''
    Lê e retorna None se for encontrado o fim do arquivo ou a próxima 
    3-upla na forma (valor do registro, número de vezes que foi lido + 1, número de gravações).
    A quantidade de leituras é incrementada.
    '''
    ln = arq.readline()
    if ln:
        valor, leituras, gravacoes = (int(x) for x in ln.split(','))
        return valor, leituras + 1, gravacoes
    return None


def gravar_registro(arq: TextIOBase, reg: tuple[int, int, int]) -> None:
    '''
    Grava um registro no dado arquivo, incrementando o número de gravações.
    '''
    arq.write(f'{reg[0]},{reg[1]},{reg[2] + 1}\n')


def intercalar_arquivos(entrada_a: TextIOBase, entrada_b: TextIOBase, saida: TextIOBase) -> None:
    '''
    Intercala duas entradas ordenadas, gravando os registros
    no arquivo `saida`, com as respectivas contagens de gravações
    atualizadas.
    '''
    valor_primeiro, valor_segundo = ler_registro(entrada_a), ler_registro(entrada_b)
    contagem_a, contagem_b, contagem_saida = 0, 0, 0
    while valor_primeiro is not None and valor_segundo is not None:
        if valor_primeiro[0] < valor_segundo[0]:
            gravar_registro(saida, valor_primeiro)
            valor_primeiro = ler_registro(entrada_a)
            contagem_saida += 1
            contagem_a += 1
        else:
            gravar_registro(saida, valor_segundo)
            valor_segundo = ler_registro(entrada_b)
            contagem_saida += 1
            contagem_b += 1
    if valor_segundo is None:
        gravar_registro(saida, valor_primeiro)
        contagem_saida += 1
        v = ler_registro(entrada_a)
        while v is not None:
            contagem_a += 1
            contagem_saida += 1
            gravar_registro(saida, v)
            v = ler_registro(entrada_a)
    if valor_primeiro is None:
        gravar_registro(saida, valor_segundo)
        contagem_saida += 1
        v = ler_registro(entrada_b)
        while v is not None:
            contagem_b += 1
            contagem_saida += 1
            gravar_registro(saida, v)
            v = ler_registro(entrada_b)
    if DEBUG:
        print(f'{entrada_a.name}:{contagem_a} + {entrada_b.name}:{contagem_b} → {saida.name}:{contagem_saida}\n', file=sys.stderr)


def gerador_de_novos_arquivos(diretorio_base: Path) -> Callable[[], Path]:
    '''
    Retorna uma função que não aceita argumentos e,
    quando chamada, retorna o caminho para um arquivo
    não existente dentro de `diretorio_base`.
    '''
    incremento = 0
    def gerar():
        nonlocal incremento
        incremento += 1
        return diretorio_base / f'i{incremento}.txt'
    return gerar


def dividir_entrada_em_subarquivos_ordenados(entrada: TextIOBase, M: int, arquivos: Callable[[], Path]) -> None:
    '''
    Separa os registros de `entrada` em arquivos de até `M` registros
    cada, fornecidos pela função `arquivos` ao ser chamada sucessivamente.
    '''
    eof = False
    while not eof:
        chunk = []
        max_tam = -1
        for _ in range(M):
            linha = entrada.readline().strip()
            if not linha:
                eof = True
                break
            max_tam = max(max_tam, len(linha))
            chunk.append(int(linha))
        if chunk:
            radix_sort(chunk, max_tam)
            arquivos().write_text('\n'.join(f'{x},1,1' for x in chunk) + '\n')


def juntar_dois_arquivos(diretorio_trabalho: Path, saida: TextIOBase | Path):
    '''
    Intercala dois arquivos ordenados de menor tamanho dentro de `diretorio_trabalho`,
    colocando os registros ordenados em `saida`. Se `saida` for um arquivo já
    aberto, ele não pode estar localizado dentro de `diretorio_trabalho`.
    '''
    caminho_primeiro, caminho_segundo = sorted(diretorio_trabalho.iterdir(), key=lambda x: x.stat().st_size)[:2]
    fechar = None
    if isinstance(saida, Path):
        saida = fechar = saida.open('w')
    try:
        with caminho_primeiro.open('r') as primeiro:
            with caminho_segundo.open('r') as segundo:
                intercalar_arquivos(primeiro, segundo, saida)
    finally:
        caminho_primeiro.unlink()
        caminho_segundo.unlink()
        if fechar is not None:
            fechar.close()


def main(entrada: TextIOBase, saida: TextIOBase, M: int) -> None:
    diretorio_trabalho = Path(mkdtemp())
    novo_arquivo = gerador_de_novos_arquivos(diretorio_trabalho)
    try:
        dividir_entrada_em_subarquivos_ordenados(entrada, M, novo_arquivo)
        
        while len([x for x in diretorio_trabalho.iterdir()]) > 2:
            juntar_dois_arquivos(diretorio_trabalho, novo_arquivo())
        
        if len([x for x in diretorio_trabalho.iterdir()]) == 2:
            juntar_dois_arquivos(diretorio_trabalho, saida)
        else:
            arquivo, = diretorio_trabalho.iterdir()
            with arquivo.open('r') as f:
                v = ler_registro(f)
                while v is not None:
                    gravar_registro(saida, v)
                    v = ler_registro(f)
    finally:
        shutil.rmtree(diretorio_trabalho)
        
    

if __name__ == '__main__':
    main(sys.stdin, sys.stdout, 5000)

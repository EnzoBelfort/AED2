#!/usr/bin/env python3
from pathlib import Path
from random import Random
from pdfminer.high_level import extract_pages
from pdfminer.layout import LTTextBoxHorizontal
from functools import cache
import string
import re
from unidecode import unidecode
from io import TextIOWrapper
from shutil import rmtree


""" 
Esse arquivo gera todos os casos de teste
que serão utilizados para o projeto.

Depende dos pacotes unidecode e pdfminer.six, disponíveis no PyPI.

Testado no Ubuntu 20.04, CPython 3.11
"""


RAIZ = Path(__file__).parent
CASOS_GERADOS_PROCEDURALMENTE = RAIZ / 'casos_procedurais'
PDFS = RAIZ / 'pdfs'


def main(rng: Random) -> None:

    # Excluir todos os casos de teste gerados anteriormente
    rmtree(CASOS_GERADOS_PROCEDURALMENTE, ignore_errors=True)
    CASOS_GERADOS_PROCEDURALMENTE.mkdir()

    # Ajuste os valores nas linhas abaixo para configurar a geração de casos
    for n in range(5_000, 100_001, 5_000):
        gerar_inteiros_aleatorios(rng, 0, 10**6, n)
        gerar_inteiros_em_ordem_nao_ascendente(rng, 0, 10**6, n)
        gerar_inteiros_identicos(37, n)
        gerar_inteiros_quase_ordenados(rng, 0, 10**6, n)
        gerar_inteiros_quase_ordenados(rng, 0, 10**6, n, 0)
        print(f'Gerados os casos numéricos para n={n}')
    for n in range(1000, 5901, 118):
        gerar_nomes_lista_sisu_embaralhados(rng, n)
        gerar_nomes_lista_sisu_quase_ordenados(rng, n)
        print(f'Gerados os casos de nomes para n={n}')


def gravar_caso(nome_arquivo_sem_extensao: str, dados: list[int] | list[str]) -> None:
    '''
    Grava um vetor em dois arquivos de teste,
    um de entrada (que será lido pelos programas)
    e um de saída (para verificação do funcionamento dos programas),
    dentro do diretório casos_procedurais.
    '''

    nome_arquivo_base = f'auto_{nome_arquivo_sem_extensao}.{len(dados)}'
    nome_arquivo_entrada = nome_arquivo_base + '.in'
    nome_arquivo_saida = nome_arquivo_base + '.out'

    with (CASOS_GERADOS_PROCEDURALMENTE / nome_arquivo_entrada).open('w') as file:
        gravar_vetor(file, dados, True)

    with (CASOS_GERADOS_PROCEDURALMENTE / nome_arquivo_saida).open('w') as file:
        gravar_vetor(file, sorted(dados), False)


def gravar_vetor(arquivo: TextIOWrapper, dados: list[int] | list[str], gerar_cabecalho: bool) -> None:
    '''
    Grava um vetor em um arquivo, com um elemento por linha. Se gerar_cabecalho,
    gravar uma linha extra no início do arquivo com o número de elementos do vetor,
    o tipo de dado (i=int, s=string) e o número máximo de caracteres
    ou algarismos entre os elementos do vetor (usado pelo radixsort). 
    '''

    codigo_tipo = {
        int: 'i',
        str: 's'
    }
    if gerar_cabecalho:
        arquivo.write(f'{len(dados)} ')
        arquivo.write(f'{codigo_tipo[type(dados[0])]} ')
        arquivo.write(f'{max(len(str(x)) for x in dados)}\n')
    for d in dados:
        arquivo.write(str(d).strip())
        arquivo.write('\n')


def gerar_inteiros_aleatorios(rng: Random, valor_minimo: int, valor_maximo: int, tamanho: int) -> None:
    gravar_caso(f'inteiros_aleatorios_{valor_minimo}-{valor_maximo}',
                vetor_inteiro_aleatorio(rng, valor_minimo, valor_maximo, tamanho))


def gerar_inteiros_em_ordem_nao_ascendente(rng: Random, valor_minimo: int, valor_maximo: int, tamanho: int):
    vec = vetor_inteiro_aleatorio(rng, valor_minimo, valor_maximo, tamanho)
    vec.sort(reverse=True)
    gravar_caso(
        f'inteiros_em_ordem_nao_ascendente_{valor_minimo}-{valor_maximo}', vec)


def gerar_inteiros_quase_ordenados(rng: Random, valor_minimo: int, valor_maximo: int, tamanho: int, porcentagem_trocas: float = 0.02) -> None:
    vec = vetor_inteiro_aleatorio(rng, valor_minimo, valor_maximo, tamanho)
    trocar_aleatoriamente(rng, vec, porcentagem_trocas)
    gravar_caso(
        f'inteiros_quase_ordenados_{valor_minimo}-{valor_maximo}_{100-(100*porcentagem_trocas):.2f}%', vec)


def trocar_aleatoriamente(rng: Random, vec: list[object], porcentagem_trocas: float) -> None:
    '''
    Embaralha parcialmente o vetor, fazendo um número de
    trocas aleatórias igual a o número de elementos do
    vetor multiplicado por porcentagem_trocas.
    '''

    vec.sort()
    trocas = round(porcentagem_trocas * len(vec))
    for _ in range(trocas):
        i = rng.randrange(0, len(vec))
        j = rng.randrange(0, len(vec))
        aux = vec[i]
        vec[i] = vec[j]
        vec[j] = aux


def gerar_inteiros_identicos(valor: int, tamanho: int) -> None:
    gravar_caso(f'inteiros_identicos_{valor}', [valor] * tamanho)


def gerar_nomes_lista_sisu_embaralhados(rng: Random, tamanho: int) -> None:
    nomes = extrair_nomes_lista_sisu()
    nomes = rng.choices(nomes, k=tamanho)
    rng.shuffle(nomes)
    gravar_caso('nomes_sisu_embaralhados', nomes)


def gerar_nomes_lista_sisu_quase_ordenados(rng: Random, tamanho: int, porcentagem_trocas: float = 0.02) -> None:
    nomes = extrair_nomes_lista_sisu()
    nomes = rng.choices(nomes, k=tamanho)
    nomes.sort()
    trocar_aleatoriamente(rng, nomes, porcentagem_trocas)
    gravar_caso('nomes_sisu_quase_ordenados', nomes)


@cache
def extrair_nomes_lista_sisu() -> list[str]:
    '''
    Lê os arquivos do diretório "pdfs" e extrai os nomes
    dos candidatos aprovados de cada arquivo, combinando
    todos os nomes em uma lista. Os nomes dos arquivos
    devem ser dados por:
        NOME_LISTA#X0_MIN,X1_MAX.pdf
    onde X0_MIN e X1_MAX representam os valores da coordenada
    horizontal dentro do arquivo PDF tal que todos os nomes
    estejam totalmente dentro desse intervalo de coordenadas.
    '''

    nomes = []
    for pdf in PDFS.iterdir():
        x0_min, x1_max = (float(x) for x in pdf.stem.split('#')[1].split(','))
        for pag in extract_pages(pdf):
            for componente in pag:
                if isinstance(componente, LTTextBoxHorizontal) and componente.x0 > x0_min and componente.x1 < x1_max:
                    nomes.append(extrair_nome_da_string(componente.get_text()))

    return ordenar_e_remover_duplicados(nomes)


def extrair_nome_da_string(s_pdf: str) -> str | None:

    # Retira acentos, caracteres não-ascii, etc.
    s = unidecode(s_pdf)

    # Transforma todos os caracteres "whitespace" no espaço tradicional.
    for caractere_whitespace in string.whitespace:
        s = s.replace(caractere_whitespace, ' ')

    # Retira todas as instâncias de espaços duplos.
    espaco_duplo = ' ' * 2
    while espaco_duplo in s:
        s = s.replace(espaco_duplo, ' ')

    # Retirar tudo o que não é letra maiúscula nem espaço
    # (Os nomes do arquivo estão todos em caixa alta)
    s = re.sub(r'[^A-Z ]', '', s)

    s = s.strip()
    
    if len(s) >= 5:  # Evitar escolher outros elementos, como a palavra literal "NOME",
        return s     # que pode aparecer acima de todos os nomes, já que os arquivos são tabelas.
    return None


def ordenar_e_remover_duplicados(vetor: list[object]) -> list[object]:
    '''
    Retorna uma cópia do vetor dado, ordenada e sem elementos repetidos.
    '''

    vetor_ordenado = sorted(x for x in vetor if x)
    resultado = []
    anterior = object()  # sentinela diferente de qualquer objeto
    for valor in vetor_ordenado:
        if valor != anterior:
            resultado.append(valor)
        anterior = valor
    return resultado


def vetor_inteiro_aleatorio(rng: Random, valor_minimo: int, valor_maximo: int, tamanho: int) -> list[int]:
    '''
    Gera um vetor de números inteiros aleatórios.
    '''

    return [rng.randint(valor_minimo, valor_maximo) for _ in range(tamanho)]


if __name__ == '__main__':
    main(Random(31415))

#!/usr/bin/env python3
from pathlib import Path
from random import Random
from shutil import rmtree
import os
import numpy as np
from io import TextIOBase

""" 
Esse arquivo gera todas as as entradas
que serão utilizados para o projeto.
"""


RAIZ = Path(__file__).parent
DIR_SAIDA = RAIZ / 'casos_gerados'


def main() -> None:

    # Excluir todos os casos de teste gerados anteriormente
    #rmtree(DIR_SAIDA, ignore_errors=True)
    DIR_SAIDA.mkdir(exist_ok=True)

    # Ajuste os valores nas linhas abaixo para configurar a geração de casos
    for tamanho in [int(10**(x/100)) for x in range(300, 751, 50)]:
        for valor_min, valor_max in [(0, 10**7), (0, 100), (10**5, 10**7)]:
            caminho = DIR_SAIDA / f'{tamanho}.{valor_min}.{valor_max}.txt'
            if caminho.exists():
                continue
            with caminho.open('w') as f:
                gerar_arquivo(valor_min, valor_max, tamanho, f)
        

def gerar_arquivo(valor_min: int, valor_max: int, tamanho: int, out: TextIOBase):
    if tamanho > 100_000:
        esq = tamanho // 2
        gerar_arquivo(valor_min, valor_max, esq, out)
        gerar_arquivo(valor_min, valor_max, tamanho-esq, out)
        return
    out.write('\n'.join(str(int(x)) for x in np.random.uniform(valor_min, valor_max, tamanho)))
    out.write('\n')


if __name__ == '__main__':
    main()

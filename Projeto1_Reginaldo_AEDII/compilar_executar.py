#!/usr/bin/env python3
from pathlib import Path
import sys
from subprocess import Popen, PIPE
import os
import shutil
import shlex
from binascii import hexlify
import statistics
import itertools
from datetime import datetime


"""
Esse arquivo compila todo o código fonte
dos algoritmos de ordenação e executa
todas as combinações entre algoritmo, linguagem e caso de teste,
gravando os resultados em um arquivo CSV.

Caso a pasta casos_procedurais esteja vazia,
deve-se executar o arquivo gerar_casos.py antes deste para
o correto funcionamento.

Depende dos executáveis "time", "bash", "gcc", "java" e "javac" presentes no PATH.

Testado no Ubuntu 20.04, CPython 3.11.8.
"""


EXECUCOES_POR_TESTE = 1
EXECUCOES_NO_LOOP = 4
RAIZ = Path(__file__).parent
EXECUTAVEIS = RAIZ / 'bin'
CASOS = RAIZ / 'casos'
CASOS_GERADOS_PROCEDURALMENTE = RAIZ / 'casos_procedurais'
CODIGO_FONTE = RAIZ / 'src'
TEMPORARIOS = RAIZ / 'tmp'
AGORA = datetime.now().strftime('%Y-%m-%d-%H-%M-%S')
ARQUIVO_IPC_TIME = TEMPORARIOS / 'time.out'
ARQUIVO_SAIDA_TABELA_SUMARIO = RAIZ / f'tabela_sumario_{AGORA}.csv'
ARQUIVO_SAIDA_DATASET = RAIZ / f'dataset_{AGORA}.csv'
NUMERO_DE_COMBINACOES = len([x for x in EXECUTAVEIS.iterdir()]) * (len(
    [x for x in CASOS.iterdir()]) + len([x for x in CASOS_GERADOS_PROCEDURALMENTE.iterdir() if x.suffix == '.in']))


def main() -> None:
    # Garantir que o ambiente está limpo
    excluir_todos_os_arquivos_do_diretorio(EXECUTAVEIS)
    excluir_todos_os_arquivos_do_diretorio(TEMPORARIOS)

    compilar_todos_os_algoritmos()
    resultados = executar_testes()

    exportar_dataset(resultados)


def excluir_todos_os_arquivos_do_diretorio(diretorio: Path) -> None:
    shutil.rmtree(diretorio, ignore_errors=True)
    diretorio.mkdir()


def compilar_todos_os_algoritmos() -> None:
    '''
    Compila todos os arquivos do diretório "src", colocando os executáveis resultantes
    no diretório "bin".
    '''
    for arquivo in CODIGO_FONTE.iterdir():
        compilar_um_algoritmo(arquivo)


def compilar_um_algoritmo(arquivo: Path) -> None:
    '''
    Compila um dado arquivo e coloca o executável na pasta "bin".
    Se o arquivo tiver a extensão .py, ele é simplesmente copiado
    para a pasta "bin".
    '''
    comando_compilador = comando_para_compilar_arquivo(arquivo)
    if comando_compilador is not None:
        child = Popen(comando_compilador)
        if child.wait() != 0:
            print('(X) O comando acima falhou e seu código de saída foi',
                  child.returncode)
            exit(child.returncode)


def comando_para_compilar_arquivo(arquivo: Path) -> list[str] | None:
    '''
    Retorna o comando usado para compilar ou copiar para a pasta "bin" um
    dado arquivo, escolhendo o compilador correto. Retorna None
    se não for possível identificar o compilador a ser utilizado.
    '''

    if arquivo.suffix == '.java':
        comando = ['javac', '-d', EXECUTAVEIS.absolute(),
                   arquivo.absolute()]
    elif arquivo.suffix == '.c':
        comando = ['gcc', arquivo.absolute(), '-o',
                   (EXECUTAVEIS / arquivo.stem).absolute()]
    elif arquivo.suffix == '.py':
        shutil.copy(arquivo, EXECUTAVEIS / arquivo.name)
        print(shlex.join(str(x) for x in ['cp', arquivo.absolute(),
                                          (EXECUTAVEIS / arquivo.name).absolute()]))
        return None
    else:
        print('skip', arquivo.absolute())
        return None
    print(shlex.join(str(x) for x in comando))
    return comando


def executar_testes() -> dict[str, dict[str, list[float]]]:
    '''
    Executa os testes para todas as combinações de executáveis (=algoritmos * linguagens)
    e casos de teste, e retorna um dicionário em que a chave é o nome do arquivo executável
    e o valor é outro dicionário, em que a chave é o nome do arquivo do caso de teste e o valor
    é uma lista de floats, com o tempo de execução para cada iteração.
    '''
    resultados = {}
    falhas = {}
    execucoes = 0

    for exe in EXECUTAVEIS.iterdir():
        comando = comando_para_executar_arquivo(exe)

        resultados[exe.name] = {}
        falhas[exe.name] = set()

        for caso in itertools.chain(CASOS.glob('*.in'), CASOS_GERADOS_PROCEDURALMENTE.glob('*.in')):
            print(
                f'*** Executando {exe.name} com o caso {caso.name}... ', end='')
            sys.stdout.flush()
            tempos = []
            soma = 0
            for _ in range(EXECUCOES_POR_TESTE):  # EXECUCOES_POR_TESTE = 1, então a lista sempre tem tamanho 1.
                t, sucesso = medir_tempo_para_execucao_do_comando_e_testar_saida(
                    comando, caso)
                tempos.append(t)
                soma += t
            print(f'{soma/EXECUCOES_POR_TESTE:.4f} s')
            execucoes += 1
            print(f'\n* Progresso: {execucoes}/{NUMERO_DE_COMBINACOES} [{100*execucoes/NUMERO_DE_COMBINACOES:.2f}%]')

            resultados[exe.name][caso.name] = tempos
            if not sucesso:
                falhas[exe.name].add(caso.stem)

    for exe, casos_de_falha in falhas.items():
        status = 'OK' if len(casos_de_falha) == 0 else ''
        print(f'- {exe}: {status}')
        for falha in casos_de_falha:
            print('  FAIL: ', falha)

    return resultados


def medir_tempo_para_execucao_do_comando_e_testar_saida(comando: list[str], caminho_arquivo_stdin: Path) -> tuple[float, bool]:
    '''
    Executa o comando dado, medindo o tempo de execução usando o utilitário "time" do Linux.
    Retorna um float indicando o tempo de execução médio e um bool indicando se a saída do
    programa é igual ao gabarito (True) ou não (False).
    '''

    outfile, cmd = arquivo_de_saida_e_comando_para_medir_tempo(
        comando, caminho_arquivo_stdin)
    sucesso = True
    child = Popen(cmd, stdin=PIPE, cwd=EXECUTAVEIS)
    child.wait()
    if child.returncode != 0:
        print(
            f'(!) Possível erro no programa: código de saída = {child.returncode}')
        sucesso = False

    sucesso = sucesso and comparar_saida_se_existir_caso_de_teste(
        caminho_arquivo_stdin, outfile.read_bytes())

    with ARQUIVO_IPC_TIME.open() as saida_time:
        saida = saida_time.read().strip().replace(',', '.')
        for linha in saida.split('\n'):
            try:
                tempo = float(linha.strip())
                break
            except ValueError:
                pass

    ARQUIVO_IPC_TIME.unlink()
    return tempo / EXECUCOES_NO_LOOP, sucesso


def arquivo_de_saida_e_comando_para_medir_tempo(comando: list[str], arquivo_stdin: Path) -> tuple[Path, list[str]]:
    '''
    Retorna o comando a ser executado para medir o tempo de execução do dado comando,
    junto ao arquivo em que será gravada a saída do programa após sua execução.
    '''

    arq_saida = TEMPORARIOS / 'out$i'
    loop = 'for i in {1..%i}; do %s > %s < %s; done' % (
        EXECUCOES_NO_LOOP, shlex.join(comando), str(arq_saida.absolute()), shlex.quote(str(arquivo_stdin.absolute())))
    return TEMPORARIOS / 'out1', ['time', '-f', '%e', '-o', ARQUIVO_IPC_TIME.absolute(), 'bash', '-c', loop]


def comparar_saida_se_existir_caso_de_teste(caminho_arquivo_entrada: Path, bytes_stdout_programa: bytes) -> bool:
    '''
    Compara o conteúdo do arquivo de saída correspondente ao dado arquivo de entrada
    aos bytes_stdout_programa. Retorna True se forem iguais ou se o arquivo 
    correspondente não existir, False caso contrário.
    '''

    caminho_arquivo_saida_esperada = caminho_arquivo_entrada.with_suffix(
        '.out')
    if caminho_arquivo_saida_esperada.exists():
        output_esperado = caminho_arquivo_saida_esperada.read_bytes().strip()
        output = bytes_stdout_programa.strip()
        if output != output_esperado:
            arquivo_output_programa = caminho_para_novo_arquivo_aleatorio(
                TEMPORARIOS, '.out')
            arquivo_output_programa.write_bytes(output)
            print(
                f'(X) Saída incorreta. Esperado: {caminho_arquivo_saida_esperada.name}. Gerado pelo programa: {arquivo_output_programa.name}.')
            return False
    return True


def caminho_para_novo_arquivo_aleatorio(diretorio: Path, extensao: str) -> Path:
    '''
    Retorna o caminho para um arquivo com nome gerado aleatoriamente,
    dentro do dado diretório. O arquivo não é criado caso não exista.
    '''
    res = (diretorio / hexlify(os.urandom(8)).decode()).with_suffix(extensao)
    while res.exists():
        res = (diretorio / hexlify(os.urandom(8)).decode()).with_suffix(extensao)
    return res


def comando_para_executar_arquivo(caminho_arquivo: Path) -> list[str]:
    '''
    Retorna o comando necessário para executar o dado arquivo, que é:
     - java <caminho> para arquivos .class,
     - python <caminho> para arquivos .py e
     - <caminho> para arquivos executáveis sem extensão.
    '''
    if caminho_arquivo.suffix == '.class' and '$' not in caminho_arquivo.name:
        return ['java', caminho_arquivo.stem]

    if caminho_arquivo.suffix == '.py':
        return [sys.executable, str(caminho_arquivo.absolute())]

    return [str(caminho_arquivo.absolute())]


def exportar_dataset(resultados: dict[str, dict[str, list[float]]]) -> None:
    '''
    Grava os resultados, no mesmo formato retornado pela função
    "executar_testes", em um dataset CSV.
    '''

    with ARQUIVO_SAIDA_DATASET.open('w') as arquivo_dataset:
        arquivo_dataset.write('algoritmo,linguagem,caso,tempo\n')
        for programa, _estrutura in resultados.items():
            algoritmo, linguagem = algoritmo_e_linguagem_pelo_nome_do_arquivo(
                programa)
            for caso, tempos in _estrutura.items():
                for tempo in tempos:
                    arquivo_dataset.write(
                        f'{algoritmo},{linguagem},{caso},{tempo}\n')


def algoritmo_e_linguagem_pelo_nome_do_arquivo(nome_arquivo: str) -> tuple[str, str]:
    '''
    Retorna o nome do algoritmo e a linguagem de programação utilizada (nessa ordem),
    em um formato legível por humanos, a partir do nome do arquivo.
    '''

    stem, suffix = os.path.splitext(nome_arquivo)
    algoritmo = stem.lower().replace('_', '').removesuffix('sort')
    linguagem = {'.class': 'Java', '.py': 'Python', '': 'C'}[suffix]
    return algoritmo, linguagem


if __name__ == '__main__':
    main()

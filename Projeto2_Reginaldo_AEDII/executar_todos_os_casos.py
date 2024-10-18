from io import TextIOBase
from external_mergesort import main as external_mergesort
from pathlib import Path
import time
import threading
from ctypes import pythonapi, py_object, c_long
import os
import sys


RAIZ = Path(__file__).parent
DIR_CASOS = RAIZ / 'casos_gerados'
NUMERO_EXECUCOES = 2
LIMITE_DE_TEMPO = 60*5
LIMITAR_TEMPO = ('tle' in ''.join(sys.argv[1:])) if len(sys.argv) > 1 else False
VERIFICAR = ('verificar' in ''.join(sys.argv[1:])) if len(sys.argv) > 1 else False


class LimiteDeTempo(BaseException):
    pass

class DevNull(TextIOBase):
    def __init__(self):
        super(DevNull, self).__init__()
        self.name = '/dev/null'

    def write(self, *_, **__):
        pass
    def flush(self, *_, **__):
        pass
    def close(self, *_, **__):
        pass


class ThreadInterrompivel(threading.Thread):
    def run(self) -> None:
        try:
            super(ThreadInterrompivel, self).run()
        except LimiteDeTempo:
            pass
    
    def interromper(self) -> None:
        pythonapi.PyThreadState_SetAsyncExc(c_long(obter_id_thread(self)), py_object(LimiteDeTempo))


def obter_id_thread(thr: threading.Thread) -> int | None:
    tid = None
    for k, v in threading._active.items():
        if v is thr:
            tid = k
            break
    return tid


def executar_caso_medindo_tempo(caso: TextIOBase, saida: TextIOBase, M: int) -> float:
    '''
    Executa o caso de teste `NUMERO_EXECUCOES` vezes e retorna o tempo médio total de execução.
    A última execução do caso produzirá uma saída, gravada no arquivo `saida` fornecido.
    Se `LIMITAR_TEMPO` for verdadeiro, a execução será interrompida caso demore
    mais do que `NUMERO_EXECUCOES * LIMITE_DE_TEMPO` segundos, e NaN será retornado.
    '''
    tempo = float('nan')
    def thread():
        nonlocal tempo
        inicio = time.perf_counter()
        for _ in range(NUMERO_EXECUCOES-1):
            external_mergesort(caso, DevNull(), M)
            caso.seek(0, 0)
        external_mergesort(caso, saida, M)
        caso.seek(0, 0)
        tempo = time.perf_counter() - inicio
        print(tempo)
    t = ThreadInterrompivel(target=thread)
    t.start()
    inicio_limite = time.perf_counter()
    time.sleep(0.5)
    while t.is_alive():
        if LIMITAR_TEMPO and time.perf_counter() - inicio_limite > LIMITE_DE_TEMPO * NUMERO_EXECUCOES:
            t.interromper()
            print('TLE')
        time.sleep(0.1)
    return tempo


def validar_saida(saida: TextIOBase, numero_de_linhas_entrada: int) -> None:
    '''
    Verifica se o arquivo de saída está ordenado e se possui o mesmo
    número de linhas que o arquivo de entrada.
    '''
    linha = saida.readline()
    anterior = float('-inf')
    numero_de_linhas_saida = 0
    while linha:
        numero_de_linhas_saida += 1
        valor, _, _ = map(int, linha.split(','))
        if valor < anterior:
            print(f'Falha no teste: saida={saida}')
            break
        anterior = valor
        linha = saida.readline()
    if numero_de_linhas_entrada != numero_de_linhas_saida:
        print(f'Falha no teste: nlinhas_entrada={numero_de_linhas_entrada}, nlinhas_saida={numero_de_linhas_saida}')


def contar_linhas(arq: TextIOBase) -> int:
    '''
    Conta quantas linhas o arquivo possui.
    '''
    pos = arq.tell()
    n = 0
    linha = arq.readline()
    while linha:
        n += 1
        linha = arq.readline()
    arq.seek(pos, 0)
    return n


def main(resultados: TextIOBase, testes_a_realizar: list[tuple[Path, int]]) -> None:
    print(f'Realizando {len(testes_a_realizar)} testes com LIMITAR_TEMPO={LIMITAR_TEMPO} VERIFICAR={VERIFICAR}')
    for caso_path, M in testes_a_realizar:
        with caso_path.open() as caso:
            numero_de_linhas_entrada = contar_linhas(caso)
            arquivo_saida = caso_path.with_suffix(f'.{M}.out')
            saida = arquivo_saida.open('w') if VERIFICAR else DevNull()
            try:
                print(f'M={M}, caso={caso_path}')
                tempo = executar_caso_medindo_tempo(caso, saida, M)
                resultados.write(f'{caso_path.name},{tempo/NUMERO_EXECUCOES},{M}\n')
                resultados.flush()
                if VERIFICAR:
                    with arquivo_saida.open('r') as saida:
                        validar_saida(saida, numero_de_linhas_entrada)
            finally:
                saida.close()
                    
    

if __name__ == '__main__':
    testes = []
    for arg in sys.argv:
        if os.path.isfile(arg) and arg.lower().endswith('.csv'):
            with open(arg) as csv:
                csv.readline()
                testes.extend((DIR_CASOS / caso, int(M)) for caso, tempo, M in (x.split(',') for x in csv.readlines() if x.strip()) if tempo == 'nan')
    if not testes:
        for caso in DIR_CASOS.iterdir():
            if 'txt' in caso.suffix:
                for M in (500, 5_000, 50_000, 500_000, 5_000_000, 50_000_000):
                    testes.append((caso, M))

    with (RAIZ / time.ctime(time.time()).replace(':', '-').replace('\\', '-').replace('/', '-')).with_suffix('.csv').open('w') as f:
        f.write('caso,tempo,M\n')
        main(f, testes)

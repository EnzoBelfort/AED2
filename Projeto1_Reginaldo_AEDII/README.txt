Esse diretório contém os códigos e materiais utilizados para o primeiro projeto avaliativo de Algoritmos e Estruturas de Dados II,
de 2024, com o professor Reginaldo Kuroshu na Universidade Federal de São Paulo.

Os subdiretórios estão estruturados da seguinte maneira:
 - src/: Código-fonte dos algoritmos implementados nas linguagens C, Python e Java.
 - bin/: Arquivos executáveis, gerados pelo script compilar_executar.py
 - casos/: Casos de teste escritos manualmente
 - casos_procedurais/: Casos de teste gerados pelo script gerar_casos.py
 - pdfs/: Arquivos PDF dos quais são extraídos nomes de pessoas pelo script gerar_casos.py
 - tmp/: Arquivos temporários gerados pelos scripts.

Já os arquivos tem as seguintes funções:
 - FORMATO DOS CASOS DE TESTE.txt: Contém informações sobre como estão formatados os casos de teste.
 - compilar_executar.py: Script que compila todos os arquivos em src/ e executa-os com todos os casos de teste, verificando a corretude dos resultados.
 - gerar_casos.py: Script que gera todos os casos de teste. É simples configurá-lo editando sua função main().
 - analise.ipynb: Notebook Jupyter em que foram produzidos os gráficos e análises para o relatório.
 - dataset_final.csv: O dataset usado em analise.ipynb.
 - comando_chrt.txt: Sugestão de comando para reprodução do teste.

Como reproduzir os testes e análises?
 1. Opcionalmente, configure o arquivo gerar_casos.py para escolher quais e quantos testes serão gerados.
Da forma que está configurado atualmente, a quantidade de testes é grande, e podem ser necessárias muitas
horas (>= 10 horas) para que todos os testes sejam completados.
 2. Executar o arquivo gerar_casos.py sem nenhum argumento para preparar os casos de teste.
 3. Executar o arquivo compilar_executar.py sem nenhum argumento para executar todos os testes/benchmarks.
    - Para que os resultados sejam mais precisos, pode-se usar o comando do arquivo comando_chrt.txt.
 4. Abrir o arquivo analise.ipynb em um notebook Jupyter e executar todas as células para reproduzir os gráficos e análises. 

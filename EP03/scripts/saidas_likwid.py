# Nome: Juliana Zambon
# GRR: 20224168
# Login DINF: jz22

import os
import re
import pandas as pd

# Diretórios
DIRETORIO_DADOS = './data'
DIRETORIO_RESULTADOS = './results'
os.makedirs(DIRETORIO_RESULTADOS, exist_ok=True)

# Padrões de métricas a serem extraídos (regex para encontrar o valor)
PADROES_METRICAS = {
    'L3CACHE': {
        'cache miss RATIO': r'|Cache miss RATIO\s*\|\s*(\d+\.\d+e[+-]?\d+)'
    },
    'ENERGY': {
        'Energy [J]': r'|Energy \[J\]\s*\|\s*(\d+\.\d+e[+-]?\d+)'
    },
    'FLOPS_DP': {
        'FLOPS DP': r'|FLOPS DP\s*\|\s*(\d+\.\d+e[+-]?\d+)',
        'FLOPS AVX DP': r'|FLOPS AVX DP\s*\|\s*(\d+\.\d+e[+-]?\d+)'
    }
}

# Coleta os arquivos de dados brutos
arquivos_dados = [f for f in os.listdir(DIRETORIO_DADOS) if f.endswith('.txt')]

# Dicionários para armazenar os resultados temporariamente
resultados_tsl = []
resultados_teg = []
resultados_cache_l3 = []
resultados_energia = []
resultados_flops_dp = []
resultados_flops_avx_dp = []

print("Processando dados brutos...")

for nome_arquivo in arquivos_dados:
    # Tenta casar com o padrão de arquivos de métrica (N_K_versão_métrica.txt)
    casamento = re.match(r'N(\d+)_K(\d+)_(\w+)_(\w+)\.txt', nome_arquivo)
    if not casamento:
        # Se não casar, tenta o padrão para os arquivos de tempo (N_K_versão_times.txt)
        casamento = re.match(r'N(\d+)_K(\d+)_(\w+)_times\.txt', nome_arquivo)
        if not casamento:
            print(f"Pulando arquivo desconhecido: {nome_arquivo}")
            continue

    N = int(casamento.group(1))
    K = int(casamento.group(2))
    versao = casamento.group(3)
    tipo_arquivo = casamento.group(4) 

    caminho_arquivo = os.path.join(DIRETORIO_DADOS, nome_arquivo)
    conteudo = open(caminho_arquivo, 'r').read()

    # Extrair tempos (tSL, tEG)
    if tipo_arquivo == 'times':
        # Procura a última linha (K tSL tEG)
        casamento_tempos = re.search(r'(\d+)\s+(\d+\.\d+e[+-]?\d+)\s+(\d+\.\d+e[+-]?\d+)', conteudo.strip().split('\n')[-1])
        if casamento_tempos:
            # K_extraido = int(casamento_tempos.group(1)) # Pode validar com o K do nome_arquivo
            tSL = float(casamento_tempos.group(2))
            tEG = float(casamento_tempos.group(3))
            resultados_tsl.append({'N': N, 'K': K, 'Versao': versao, 'tSL': tSL})
            resultados_teg.append({'N': N, 'K': K, 'Versao': versao, 'tEG': tEG})
        else:
            print(f"Não foi possível extrair os tempos de {nome_arquivo}")

    # Extrair métricas LIKWID
    else: # O tipo_arquivo deve ser um grupo de métrica LIKWID
        nome_grupo_metrica = tipo_arquivo 
        if nome_grupo_metrica in PADROES_METRICAS:
            for nome_metrica, padrao in PADROES_METRICAS[nome_grupo_metrica].items():
                casamento_metrica = re.search(padrao, conteudo)
                if casamento_metrica:
                    valor = float(casamento_metrica.group(1))
                    if nome_metrica == 'cache miss RATIO':
                        resultados_cache_l3.append({'N': N, 'K': K, 'Versao': versao, 'TaxaMissCache': valor})
                    elif nome_metrica == 'Energy [J]':
                        resultados_energia.append({'N': N, 'K': K, 'Versao': versao, 'Energia': valor})
                    elif nome_metrica == 'FLOPS DP':
                        resultados_flops_dp.append({'N': N, 'K': K, 'Versao': versao, 'FLOPS_DP': valor})
                    elif nome_metrica == 'FLOPS AVX DP':
                        resultados_flops_avx_dp.append({'N': N, 'K': K, 'Versao': versao, 'FLOPS_AVX_DP': valor})
                else:
                    print(f"Não foi possível encontrar '{nome_metrica}' em {nome_arquivo}")
        else:
            print(f"Grupo de métrica LIKWID desconhecido: {tipo_arquivo} em {nome_arquivo}")

# Converte listas de dicionários para DataFrames e salva como CSV
def salvar_para_csv(lista_dados, nome_coluna_metrica, sufixo_arquivo):
    if not lista_dados:
        print(f"Nenhum dado coletado para {nome_coluna_metrica}. Pulando a criação do CSV.")
        return

    df = pd.DataFrame(lista_dados)
    
    # Reorganiza para o formato wide ideal para plotagem (K como índice, colunas para N_vX_metrica)
    tabela_pivot = pd.pivot_table(df, values=nome_coluna_metrica, index='K', columns=['N', 'Versao'])
    # Renomeia as colunas para o formato N_vX_metrica
    tabela_pivot.columns = [f"N{N}_{versao}_{sufixo_arquivo}" for N, versao in tabela_pivot.columns]
    
    # Garante a ordem das colunas para os gráficos (N10_v1, N10_v2, N1000_v1, N1000_v2)
    # Lista de tuplas (N, V) para ordenar, depois mapear para string
    colunas_ordenadas = []
    for N_valor in [10, 1000]: # Garante a ordem de N
        for V_valor in ['v1', 'v2']: # Garante a ordem de versão
            nome_coluna = f"N{N_valor}_{V_valor}_{sufixo_arquivo}"
            if nome_coluna in tabela_pivot.columns:
                colunas_ordenadas.append(nome_coluna)
    
    tabela_pivot = tabela_pivot[colunas_ordenadas].reset_index() # Reseta o index 'K' para uma coluna

    caminho_csv = os.path.join(DIRETORIO_RESULTADOS, f'resultados_{sufixo_arquivo}.csv')
    tabela_pivot.to_csv(caminho_csv, index=False)
    print(f"Dados de {nome_coluna_metrica} salvos em {caminho_csv}")

salvar_para_csv(resultados_tsl, 'tSL', 'tsl')
salvar_para_csv(resultados_teg, 'tEG', 'teg')
salvar_para_csv(resultados_cache_l3, 'TaxaMissCache', 'taxa_miss_cache_l3')
salvar_para_csv(resultados_energia, 'Energia', 'energia')
salvar_para_csv(resultados_flops_dp, 'FLOPS_DP', 'flops_dp')
salvar_para_csv(resultados_flops_avx_dp, 'FLOPS_AVX_DP', 'flops_avx_dp')

print(f"Processamento de dados concluído. Os CSVs estão em '{DIRETORIO_RESULTADOS}'.")
print("Agora você pode usar o script generate_plots.py para criar os gráficos.")
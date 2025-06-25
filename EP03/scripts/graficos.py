# Nome: Juliana Zambon
# GRR: 20224168
# Login DINF: jz22

import re
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os

# Diretório dos resultados (onde estão os CSVs)
DIRETORIO_RESULTADOS = './results'
DIRETORIO_GRAFICOS = os.path.join(DIRETORIO_RESULTADOS, 'plots')
os.makedirs(DIRETORIO_GRAFICOS, exist_ok=True)

# Cores para as linhas 
CORES = {
    'N10_v1': 'skyblue',        # Um azul claro e amigável
    'N10_v2': 'dodgerblue',     # Um azul mais forte (versão otimizada)
    'N1000_v1': 'lightcoral',   # Um vermelho mais suave
    'N1000_v2': 'darkred'       # Um vermelho escuro (versão otimizada)
}

# Informações dos gráficos (para automatizar a criação)
# Formato: (arquivo_csv, coluna_y_prefixo, titulo, label_y, y_logscale)
CONFIGURACOES_GRAFICOS = [
    # **Ajuste aqui: 'y_logscale' definido como True para os tempos**
    ('resultados_tsl.csv', 'tsl', 'Tempo de Geração do SL vs. K', 'Tempo (ms)', True),
    ('resultados_teg.csv', 'teg', 'Tempo de Solução do SL vs. K', 'Tempo (ms)', True),
    ('resultados_taxa_miss_cache_l3.csv', 'taxa_miss_cache_l3', 'Taxa de Cache Miss L3 vs. K', 'Taxa de Cache Miss', False),
    ('resultados_energia.csv', 'energia', 'Energia Consumida vs. K', 'Energia (J)', False),
    ('resultados_flops_dp.csv', 'flops_dp', 'FLOPS DP vs. K', 'FLOPS DP (MFLOP/s)', False),
    ('resultados_flops_avx_dp.csv', 'flops_avx_dp', 'FLOPS AVX DP vs. K', 'FLOPS AVX DP (MFLOP/s)', False)
]

print("Gerando gráficos...")

for arquivo_csv, prefixo_coluna, titulo, rotulo_y, escala_log_y in CONFIGURACOES_GRAFICOS:
    caminho_csv = os.path.join(DIRETORIO_RESULTADOS, arquivo_csv)
    if not os.path.exists(caminho_csv):
        print(f"Arquivo CSV não encontrado: {caminho_csv}. Pulando geração do gráfico.")
        continue

    df = pd.read_csv(caminho_csv)

    plt.figure(figsize=(12, 7)) # Aumenta o tamanho da figura para melhor visualização

    # Colunas esperadas no CSV (formato N_vX_prefixocoluna)
    nomes_colunas_esperadas = [f"N10_v1_{prefixo_coluna}", f"N10_v2_{prefixo_coluna}", 
                               f"N1000_v1_{prefixo_coluna}", f"N1000_v2_{prefixo_coluna}"]
    
    # Plotar cada linha
    for col in nomes_colunas_esperadas:
        if col in df.columns:
            # Extrai N e Versão do nome da coluna para buscar a cor correta
            casamento = re.match(r'N(\d+)_(\w+)_\w+', col)
            if casamento:
                n_valor = int(casamento.group(1))
                v_valor = casamento.group(2)
                chave_cor = f"N{n_valor}_{v_valor}"
                plt.plot(df['K'], df[col], label=f"N={n_valor}, Versão {v_valor.upper()}", 
                         color=CORES.get(chave_cor, 'black'), marker='o', markersize=4, linewidth=1.5)
            else:
                # Fallback caso o nome da coluna não siga o padrão esperado
                plt.plot(df['K'], df[col], label=col, marker='o', markersize=4, linewidth=1.5) 
        else:
            print(f"Aviso: Coluna '{col}' não encontrada em '{arquivo_csv}'. Pulando.")

    # Configurar escalas dos eixos
    plt.xscale('log')
    if escala_log_y:
        plt.yscale('log')
        # Adiciona indicação de escala no rótulo Y se for logarítmica
        rotulo_y_final = rotulo_y + ' (Escala Logarítmica)' 
    else:
        rotulo_y_final = rotulo_y

    # Adicionar rótulos, título, legenda e grade
    plt.xlabel('Número de Pontos (K) - Escala Logarítmica', fontsize=12)
    plt.ylabel(rotulo_y_final, fontsize=12)
    plt.title(titulo, fontsize=14)
    plt.legend(title="Legenda", fontsize=10)
    plt.grid(True, which="both", ls="--", c='0.7', alpha=0.6) # Grade para eixos logarítmicos
    plt.tight_layout() # Ajusta o layout para evitar sobreposição

    # Salva o gráfico
    nome_arquivo_grafico = os.path.join(DIRETORIO_GRAFICOS, f"{arquivo_csv.replace('.csv', '.png')}")
    plt.savefig(nome_arquivo_grafico)
    print(f"Gráfico salvo: {nome_arquivo_grafico}")
    plt.close() # Fecha a figura para liberar memória

print(f"Geração de gráficos concluída. Os gráficos estão em '{DIRETORIO_GRAFICOS}'.")
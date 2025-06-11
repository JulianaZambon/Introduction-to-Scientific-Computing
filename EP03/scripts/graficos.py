import re
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os

# Diretório dos resultados (onde estão os CSVs)
RESULTS_DIR = '../results'
PLOTS_DIR = os.path.join(RESULTS_DIR, 'plots')
os.makedirs(PLOTS_DIR, exist_ok=True)

# Cores para as linhas (fixas para consistência)
COLORS = {
    'N10_v1': 'blue',
    'N10_v2': 'cyan',
    'N1000_v1': 'red',
    'N1000_v2': 'orange'
}

# Informações dos gráficos (para automatizar a criação)
# (arquivo_csv, coluna_y_prefixo, titulo, label_y, y_logscale)
GRAPH_CONFIGS = [
    ('results_tsl.csv', 'tsl', 'Tempo de Geração do SL vs. K', 'Tempo (ms)', True),
    ('results_teg.csv', 'teg', 'Tempo de Solução do SL vs. K', 'Tempo (ms)', True),
    ('results_cache_l3_ratio.csv', 'cache_l3_ratio', 'Cache Miss Ratio L3 vs. K', 'Cache Miss Ratio', False),
    ('results_energy.csv', 'energy', 'Energia Consumida vs. K', 'Energia (J)', False),
    ('results_flops_dp.csv', 'flops_dp', 'FLOPS DP vs. K', 'FLOPS DP (MFLOP/s)', False),
    ('results_flops_avx_dp.csv', 'flops_avx_dp', 'FLOPS AVX DP vs. K', 'FLOPS AVX DP (MFLOP/s)', False)
]

print("Gerando gráficos...")

for csv_file, col_prefix, title, ylabel, y_logscale in GRAPH_CONFIGS:
    csv_path = os.path.join(RESULTS_DIR, csv_file)
    if not os.path.exists(csv_path):
        print(f"Arquivo CSV não encontrado: {csv_path}. Pulando gráfico.")
        continue

    df = pd.read_csv(csv_path)

    plt.figure(figsize=(12, 7)) # Aumenta o tamanho da figura para melhor visualização

    # Colunas esperadas no CSV (formato N_vX_colprefix)
    column_names = [f"N10_v1_{col_prefix}", f"N10_v2_{col_prefix}", f"N1000_v1_{col_prefix}", f"N1000_v2_{col_prefix}"]
    
    # Plotar cada linha
    for col in column_names:
        if col in df.columns:
            # Extrai N e Version do nome da coluna para buscar a cor correta
            match = re.match(r'N(\d+)_(\w+)_\w+', col)
            if match:
                n_val = int(match.group(1))
                v_val = match.group(2)
                color_key = f"N{n_val}_{v_val}"
                plt.plot(df['K'], df[col], label=f"N={n_val}, Versão {v_val.upper()}", color=COLORS.get(color_key, 'black'), marker='o', markersize=4, linewidth=1.5)
            else:
                plt.plot(df['K'], df[col], label=col, marker='o', markersize=4, linewidth=1.5) # Fallback se o nome da coluna não seguir o padrão
        else:
            print(f"Aviso: Coluna '{col}' não encontrada em '{csv_file}'. Pulando.")

    # Configurar eixos logarítmicos
    plt.xscale('log')
    if y_logscale:
        plt.yscale('log')

    # Adicionar rótulos, título, legenda e grade
    plt.xlabel('Número de Pontos (K) - Escala Logarítmica', fontsize=12)
    plt.ylabel(ylabel + ' - Escala Logarítmica' if y_logscale else ylabel, fontsize=12)
    plt.title(title, fontsize=14)
    plt.legend(title="Legenda", fontsize=10)
    plt.grid(True, which="both", ls="--", c='0.7', alpha=0.6) # Grade para eixos logarítmicos
    plt.tight_layout() # Ajusta o layout para evitar sobreposição

    # Salva o gráfico
    plot_filename = os.path.join(PLOTS_DIR, f"{csv_file.replace('.csv', '.png')}")
    plt.savefig(plot_filename)
    print(f"Gráfico salvo: {plot_filename}")
    plt.close() # Fecha a figura para liberar memória

print("Geração de gráficos concluída. Os gráficos estão em '$PLOTS_DIR'.")
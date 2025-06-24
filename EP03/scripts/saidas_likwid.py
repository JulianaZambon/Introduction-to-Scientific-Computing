import os
import re
import pandas as pd

# Diretórios
DATA_DIR = './data'
RESULTS_DIR = '../results'
os.makedirs(RESULTS_DIR, exist_ok=True)

# Métricas a extrair (regex para encontrar o valor)
# Adapte as regex se o formato de saída do LIKWID for ligeiramente diferente
METRIC_PATTERNS = {
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
data_files = [f for f in os.listdir(DATA_DIR) if f.endswith('.txt')]

# Dicionários para armazenar os resultados temporariamente
results_tsl = []
results_teg = []
results_cache_l3 = []
results_energy = []
results_flops_dp = []
results_flops_avx_dp = []

print("Processando dados brutos...")

for filename in data_files:
    match = re.match(r'N(\d+)_K(\d+)_(\w+)_(\w+)\.txt', filename)
    if not match:
        # Tenta o padrão para os tempos (que não têm grupo de métrica)
        match = re.match(r'N(\d+)_K(\d+)_(\w+)_times\.txt', filename)
        if not match:
            print(f"Skipping unknown file: {filename}")
            continue

    N = int(match.group(1))
    K = int(match.group(2))
    version = match.group(3)
    file_type = match.group(4) # 'times' ou o nome da métrica LIKWID

    filepath = os.path.join(DATA_DIR, filename)
    content = open(filepath, 'r').read()

    # Extrair tempos (tSL, tEG)
    if file_type == 'times':
        # Procura a última linha (K tSL tEG)
        times_match = re.search(r'(\d+)\s+(\d+\.\d+e[+-]?\d+)\s+(\d+\.\d+e[+-]?\d+)', content.strip().split('\n')[-1])
        if times_match:
            # K_extracted = int(times_match.group(1)) # Pode validar com o K do filename
            tSL = float(times_match.group(2))
            tEG = float(times_match.group(3))
            results_tsl.append({'N': N, 'K': K, 'Version': version, 'tSL': tSL})
            results_teg.append({'N': N, 'K': K, 'Version': version, 'tEG': tEG})
        else:
            print(f"Could not parse times from {filename}")

    # Extrair métricas LIKWID
    else: # filename should be a METRIC
        metric_group_name = file_type # Ex: 'L3CACHE', 'ENERGY', 'FLOPS_DP'
        if metric_group_name in METRIC_PATTERNS:
            for metric_name, pattern in METRIC_PATTERNS[metric_group_name].items():
                metric_match = re.search(pattern, content)
                if metric_match:
                    value = float(metric_match.group(1))
                    if metric_name == 'cache miss RATIO':
                        results_cache_l3.append({'N': N, 'K': K, 'Version': version, 'CacheMissRatio': value})
                    elif metric_name == 'Energy [J]':
                        results_energy.append({'N': N, 'K': K, 'Version': version, 'Energy': value})
                    elif metric_name == 'FLOPS DP':
                        results_flops_dp.append({'N': N, 'K': K, 'Version': version, 'FLOPS_DP': value})
                    elif metric_name == 'FLOPS AVX DP':
                        results_flops_avx_dp.append({'N': N, 'K': K, 'Version': version, 'FLOPS_AVX_DP': value})
                else:
                    print(f"Could not find '{metric_name}' in {filename}")
        else:
            print(f"Unknown LIKWID metric group type: {file_type} in {filename}")

# Converte listas de dicionários para DataFrames e salva como CSV
def save_to_csv(data_list, metric_name, file_suffix):
    if not data_list:
        print(f"No data collected for {metric_name}. Skipping CSV creation.")
        return

    df = pd.DataFrame(data_list)
    
    # Reorganiza para o formato wide ideal para plotagem (K como índice, colunas para N_vX_metrica)
    pivot_df = pd.pivot_table(df, values=metric_name, index='K', columns=['N', 'Version'])
    # Renomeia as colunas para o formato N_vX_metrica
    pivot_df.columns = [f"N{N}_{version}_{file_suffix}" for N, version in pivot_df.columns]
    
    # Garante a ordem das colunas para os gráficos (N1_v1, N1_v2, N2_v1, N2_v2)
    # Lista de tuplas (N, V) para ordenar, depois mapear para string
    ordered_cols = []
    for N_val in [10, 1000]: # Garantir a ordem de N
        for V_val in ['v1', 'v2']: # Garantir a ordem de versão
            col_name = f"N{N_val}_{V_val}_{file_suffix}"
            if col_name in pivot_df.columns:
                ordered_cols.append(col_name)
    
    pivot_df = pivot_df[ordered_cols].reset_index() # Reseta o index 'K' para uma coluna

    csv_path = os.path.join(RESULTS_DIR, f'results_{file_suffix}.csv')
    pivot_df.to_csv(csv_path, index=False)
    print(f"Saved {metric_name} data to {csv_path}")

save_to_csv(results_tsl, 'tSL', 'tsl')
save_to_csv(results_teg, 'tEG', 'teg')
save_to_csv(results_cache_l3, 'CacheMissRatio', 'cache_l3_ratio')
save_to_csv(results_energy, 'Energy', 'energy')
save_to_csv(results_flops_dp, 'FLOPS_DP', 'flops_dp')
save_to_csv(results_flops_avx_dp, 'FLOPS_AVX_DP', 'flops_avx_dp')

print("Processamento de dados concluído. Os CSVs estão em '$RESULTS_DIR'.")
print("Agora você pode usar o script generate_plots.py para criar os gráficos.")
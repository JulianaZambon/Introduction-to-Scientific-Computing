# Nome: Juliana Zambon
# GRR: 20224168
# Login DINF: jz22

#!/bin/bash

# Configurações
VERSIONS=("v1" "v2") 
NS=(10 1000)
KS_BASE=(64 128 200 256 512 600 800 1024 2000 3000 4096 6000 7000 10000 50000 100000)
KS_N1_ADD=(1000000 10000000 100000000) # K para N=10

# Grupos de métricas do LIKWID que você quer coletar
METRIC_GROUPS=("L3CACHE" "ENERGY" "FLOPS_DP")
CORE=3 # Core a ser usada para o LIKWID

# Diretórios de saída
DATA_DIR="./data"
mkdir -p "$DATA_DIR" 

echo "Iniciando os testes..."

for N in "${NS[@]}"; do
    # Copia os valores base de K para o array K_values
    K_values=("${KS_BASE[@]}")

    # Adiciona os valores extras de K se N for 10
    if [ "$N" -eq 10 ]; then
        K_values+=("${KS_N1_ADD[@]}")
    fi

    for K in "${K_values[@]}"; do
        for VERSION in "${VERSIONS[@]}"; do
            PROGRAM="./ajustePol_${VERSION}"
            OUTPUT_PREFIX="${DATA_DIR}/N${N}_K${K}_${VERSION}"

            echo "Rodando N=${N}, K=${K}, Versão=${VERSION}"

            # 1. Coleta os tempos tSL e tEG do programa diretamente
            # A saída será redirecionada para um arquivo para ser parseada depois
            # O 2>&1 redireciona stderr para stdout, garantindo que erros também sejam capturados
            ./gera_entrada "$K" "$N" | "$PROGRAM" > "${OUTPUT_PREFIX}_times.txt" 2>&1

            # 2. Coleta as métricas do LIKWID para as regiões marcadas

            REGIONS_FOR_VERSION=("") # Inicializa vazio

            if [ "$VERSION" = "v1" ]; then
                REGIONS_FOR_VERSION+=("v1_montaSL" "v1_eliminacaoGauss")
            elif [ "$VERSION" = "v2" ]; then
                REGIONS_FOR_VERSION+=("v2_montaSL" "v2_eliminacaoGauss")
            fi

            for REGION in "${REGIONS_FOR_VERSION[@]}"; do
                for METRIC_GROUP in "${METRIC_GROUPS[@]}"; do
                    echo "  -> Coletando métrica: ${METRIC_GROUP} para região: ${REGION}"
                    ./gera_entrada "$K" "$N" | likwid-perfctr -C "$CORE" -g "$METRIC_GROUP" -m -r "$REGION" "$PROGRAM" > "${OUTPUT_PREFIX}_${REGION}_${METRIC_GROUP}.txt" 2>&1
                done
            done
        done
    done
done

echo "Todos os testes foram concluídos. Os dados brutos estão em '$DATA_DIR'."
echo "Agora você pode rodar o script parse_likwid_output.py para processar os dados."
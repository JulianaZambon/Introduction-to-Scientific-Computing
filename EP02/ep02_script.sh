#!/bin/bash

# Verifica se o programa resolveEDO está presente
if ! command -v resolveEDO &> /dev/null
then
    echo "O programa resolveEDO não foi encontrado."
    exit 1
fi

# Executa o programa resolveEDO com o arquivo de entrada, redirecionando a saída para /dev/null para não exibir
./resolveEDO input.txt > /dev/null 2>&1 &

# Captura o PID do processo resolveEDO
PID=$!

# Exibe as saídas das matrizes e os contadores de instruções aritméticas
# Inicia o LIKWID monitorando o contador FP_ARITH_INST_RETIRED_SCALAR_DOUBLE
# e mostra a saída de todas as operações
likwid-perfctr -p $PID -g FLOPS_DP -T FP_ARITH_INST_RETIRED_SCALAR_DOUBLE -C 0

# Espera o processo terminar
wait $PID


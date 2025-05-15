#!/bin/bash

# Caminho para o executável
EXEC=./resolveEDO

# Verifica se o executável existe
if [ ! -x "$EXEC" ]; then
    echo "Erro: o executável '$EXEC' não foi encontrado ou não é executável."
    exit 1
fi

# Verifica se o arquivo de entrada foi passado via redirecionamento
if [ -t 0 ]; then
    echo "Erro: forneça a entrada via redirecionamento"
    exit 1
fi

# 1. Executa o programa e imprime a saída normalmente
"$EXEC"

# Adiciona uma linha em branco para separar as saídas
echo ""

# 2. Executa o programa 3 vezes com LIKWID para medir operações FP em dupla precisão
# obtidos estes valores para o processo de fatoração LU e para o cálculo de cada solução das EDO's
# LIKWID_MARKER_START("solve_LU")
# LIKWID_MARKER_STOP("solve_EDO");
for i in {1..3}; do
    likwid-perfctr -C 0 -g FLOPS_DP -f -O "$EXEC" 2>&1 \
    | grep FP_ARITH_INST_RETIRED_SCALAR_DOUBLE \
    | awk -F',' '{gsub(/ /, "", $3); print $1 "," $3}'
done


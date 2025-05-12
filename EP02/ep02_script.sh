#!/bin/bash

# Caminho do executável
EXEC=./resolveEDO

# 1. Executa o programa normalmente (para obter a saída)
$EXEC

# Adiciona uma quebra de linha
echo ""

# 2. Mede instruções FP para a fatoração LU
likwid-perfctr -C 0 -g FLOPS_DP -f -O $EXEC < entrada.txt 2>&1 | grep FP_ARITH_INST_RETIRED_SCALAR_DOUBLE | awk -F ',' '{print $1 "," $2}'

# 3. Mede instruções FP para resolver a 1ª EDO
likwid-perfctr -C 0 -g FLOPS_DP -f -O $EXEC < entrada.txt 2>&1 | grep FP_ARITH_INST_RETIRED_SCALAR_DOUBLE | awk -F ',' '{print $1 "," $2}'

# 4. Mede instruções FP para resolver a 2ª EDO
likwid-perfctr -C 0 -g FLOPS_DP -f -O $EXEC < entrada.txt 2>&1 | grep FP_ARITH_INST_RETIRED_SCALAR_DOUBLE | awk -F ',' '{print $1 "," $2}'

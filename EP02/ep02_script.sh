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

# 2. Para cada conjunto de coeficientes r1 r2 r3 r4, executa o programa com LIKWID no modo marcador (-m)
#    e extrai a métrica FP_ARITH_INST_RETIRED_SCALAR_DOUBLE das regiões "solve_EDO" e "solve_LU".
#    Essa métrica representa a quantidade de instruções aritméticas de ponto flutuante em dupla precisão (escalares).
#    Exemplo de saída:
#       FP_ARITH_INST_RETIRED_SCALAR_DOUBLE,1517
#       FP_ARITH_INST_RETIRED_SCALAR_DOUBLE,312

likwid-perfctr -C 0 -g FLOPS_DP -m "$EXEC" 2>&1 \
    | grep 'FP_ARITH_INST_RETIRED_SCALAR_DOUBLE\|Tag solve_EDO\|Tag solve_LU' \
    | awk -F',' '
        /Tag solve_EDO/ { in_edo = 1; in_lu = 0; next }
        /Tag solve_LU/ { in_lu = 1; in_edo = 0; next }
        /FP_ARITH_INST_RETIRED_SCALAR_DOUBLE/ {
            if (in_edo) {
                print "solve_EDO_FP_ARITH_INST_RETIRED_SCALAR_DOUBLE," $3
            } else if (in_lu) {
                print "solve_LU_FP_ARITH_INST_RETIRED_SCALAR_DOUBLE," $3
            }
        }
    '
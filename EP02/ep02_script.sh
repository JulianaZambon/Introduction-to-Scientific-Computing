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

NUM_LINHAS_FIXAS=4

awk 'NF==4' entradateste.txt | while read r1 r2 r3 r4; do
    # Executa e captura as linhas de interesse
    OUTPUT=$( (head -n $NUM_LINHAS_FIXAS entradateste.txt; echo "$r1 $r2 $r3 $r4") | \
    # likwid-perfctr -C 0 -g FLOPS_DP -m ./resolveEDO < entradateste.txt
    script -q -c "likwid-perfctr -C 0 -g FLOPS_DP -m \"$EXEC\"" /dev/null 2>&1 )
    

    # Extrai e imprime só o solve_EDO, sem o prefixo "solve_EDO_"
    echo "$OUTPUT" | awk '
        /Region solve_EDO/ { region = "solve_EDO"; next }
        /Region solve_LU/ { region = "solve_LU"; next }
        /FP_ARITH_INST_RETIRED_SCALAR_DOUBLE/ {
            split($0, parts, "|");
            gsub(/^ +| +$/, "", parts[4]);
            if(region == "solve_EDO") {
                # remove prefixo "solve_EDO_"
                sub(/^solve_EDO_/, "", $0);
                print "FP_ARITH_INST_RETIRED_SCALAR_DOUBLE," parts[4];
            } else if(region == "solve_LU") {
                lu_value = parts[4];
            }
        }
        END {
            if(lu_value != "")
                print "LU_METRICS=" lu_value;
        }
    '

done > edo_temp.txt

# Pega o LU (último valor, que é o mesmo para todos)
LU=$(grep LU_METRICS edo_temp.txt | tail -n1 | cut -d= -f2)

# Imprime o LU só uma vez no final, sem prefixo
echo "FP_ARITH_INST_RETIRED_SCALAR_DOUBLE,$LU"

# Imprime tudo que é EDO (sem as linhas LU_METRICS)
grep -v LU_METRICS edo_temp.txt

rm edo_temp.txt


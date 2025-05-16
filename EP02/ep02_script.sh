# 
#  Nome: Juliana Zambon
#  GRR: 20224168
#  Login DINF: jz22
#  

#!/bin/bash
set -e

# ============================================================
# Script para executar o programa resolveEDO com LIKWID,
# coletando a métrica FP_ARITH_INST_RETIRED_SCALAR_DOUBLE
# para diferentes conjuntos de coeficientes r1 r2 r3 r4.
# As regiões de interesse são "solve_EDO" e "solve_LU".
# ============================================================

# Caminho para o executável
EXEC=./resolveEDO

# Verifica se o executável existe e tem permissão de execução
if [ ! -x "$EXEC" ]; then
    echo "Erro: o executável '$EXEC' não foi encontrado ou não é executável."
    exit 1
fi

# Verifica se a entrada foi passada via redirecionamento
if [ -t 0 ]; then
    echo "Erro: forneça a entrada via redirecionamento"
    exit 1
fi

# Executa o programa normalmente com a entrada original e imprime a saída
"$EXEC"

# Linha em branco para separar a saída principal das métricas
echo ""

# Número fixo de linhas iniciais (cabeçalho da entrada)
NUM_LINHAS_FIXAS=4

# Extrai linhas com 4 valores (coeficientes) do arquivo de entrada
# Para cada conjunto, monta uma nova entrada combinando com o cabeçalho
# e executa resolveEDO com LIKWID, coletando as métricas desejadas
awk 'NF==4' entradateste.txt | while read r1 r2 r3 r4; do
    OUTPUT=$( (head -n $NUM_LINHAS_FIXAS entradateste.txt; echo "$r1 $r2 $r3 $r4") | \
        script -q -c "likwid-perfctr -C 0 -g FLOPS_DP -m \"$EXEC\"" /dev/null 2>&1 )

    # Filtra a saída do LIKWID:
    # - Captura a métrica FP_ARITH_INST_RETIRED_SCALAR_DOUBLE das regiões "solve_EDO" e "solve_LU"
    # - Remove prefixos e guarda o valor de LU para imprimir no final
    echo "$OUTPUT" | awk '
        /Region solve_EDO/ { region = "solve_EDO"; next }
        /Region solve_LU/  { region = "solve_LU"; next }
        /FP_ARITH_INST_RETIRED_SCALAR_DOUBLE/ {
            split($0, parts, "|");
            gsub(/^ +| +$/, "", parts[4]);
            if (region == "solve_EDO") {
                # Remove prefixo "solve_EDO_" se presente
                sub(/^solve_EDO_/, "", $0);
                print "FP_ARITH_INST_RETIRED_SCALAR_DOUBLE," parts[4];
            } else if (region == "solve_LU") {
                lu_value = parts[4];
            }
        }
        END {
            if (lu_value != "")
                print "LU_METRICS=" lu_value;
        }
    '

done > edo_temp.txt

# Extrai o valor de LU (último da lista, pois é sempre o mesmo)
LU=$(grep LU_METRICS edo_temp.txt | tail -n1 | cut -d= -f2)

# Imprime o valor de LU uma única vez no final
echo "FP_ARITH_INST_RETIRED_SCALAR_DOUBLE,$LU"

# Imprime apenas os valores de solve_EDO, ignorando linhas temporárias
grep -v LU_METRICS edo_temp.txt

# Remove o arquivo temporário
rm edo_temp.txt

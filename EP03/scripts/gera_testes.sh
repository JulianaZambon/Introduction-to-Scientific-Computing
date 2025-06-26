# Nome: Juliana Zambon
# GRR: 20224168
# Login DINF: jz22

#!/bin/bash

# metricas a serem coletadas
METRICAS="L3CACHE ENERGY FLOPS_DP"

# cria diretorios para os resultados
if [ ! -d "results" ]; then
    mkdir results
fi
# dentro de results, cria diretorios para v1 e v2
if [ ! -d "results/v1" ]; then
    # dentro de v1 vai criar varios .csv com as metricas do likwid para os testes sem otimizacao
    # (ajustePol_v1)
    mkdir results/v1
fi
if [ ! -d "results/v2" ]; then
    # dentro de v2 vai criar varios .csv com as metricas do likwid para os testes com otimizacao
    # (ajustePol_v2)
    mkdir results/v2
fi

make all

# cria csv com resultados do tempo (tanto de v1 quanto v2)
echo "n,k,version,time1,time2" > "results/time.csv"

n=10
#                                                                              10⁶     10⁷      10⁸    
K="64 128 200 256 512 600 800 1024 2000 3000 4096 6000 7000 10000 50000 100000 1000000 10000000 100000000"
for k in $K
do
    # getting time (no need for likwid)
    ./gera $k $n | ./ajustePol_v1 | tail -n1 > temp.txt
    tempo_monta=$(cut -d' ' temp.txt -f2)
    tempo_elimina=$(cut -d' ' temp.txt -f3)
    echo $n","$k",1,"$tempo_monta","$tempo_elimina >> results/time.csv
    ./gera $k $n | ./ajustePol_v2 | tail -n1 > temp.txt
    tempo_monta=$(cut -d' ' temp.txt -f2)
    tempo_elimina=$(cut -d' ' temp.txt -f3)
    echo $n","$k",2,"$tempo_monta","$tempo_elimina >> results/time.csv
    # para cada métrica, cria um csv com aquela métrica e os respectivos k e n
    for metrica in $METRICAS
    do
        nome_arq=($metrica"_"$k"_"$n".csv")
        echo $nome_arq

        # results for v1
        ./gera $k $n | likwid-perfctr -O -C 3 -g $metrica -m ./ajustePol_v1 > temp.txt
        tam=$(wc -l temp.txt | cut -d' ' -f1)
        inicio=$(grep '\-\-' temp.txt -n | tail -n1 | cut -d':' -f1)
        pegar=$(($tam-$inicio))
        tail -n$pegar temp.txt > results/v1/$nome_arq

        # results for v2
        ./gera $k $n | likwid-perfctr -O -C 3 -g $metrica -m ./ajustePol_v2 > temp.txt
        tam=$(wc -l temp.txt | cut -d' ' -f1)
        inicio=$(grep '\-\-' temp.txt -n | tail -n1 | cut -d':' -f1)
        pegar=$(($tam-$inicio))
        tail -n$pegar temp.txt > results/v2/$nome_arq
    done
done


n=1000
#                                                                       10⁵
K="64 128 200 256 512 600 800 1024 2000 3000 4096 6000 7000 10000 50000 100000"
for k in $K
do
    # getting time (no need for likwid)
    ./gera $k $n | ./ajustePol_v1 | tail -n1 > temp.txt
    tempo_monta=$(cut -d' ' temp.txt -f2)
    tempo_elimina=$(cut -d' ' temp.txt -f3)
    echo $n","$k",1,"$tempo_monta","$tempo_elimina >> results/time.csv
    ./gera $k $n | ./ajustePol_v2 | tail -n1 > temp.txt
    tempo_monta=$(cut -d' ' temp.txt -f2)
    tempo_elimina=$(cut -d' ' temp.txt -f3)
    echo $n","$k",2,"$tempo_monta","$tempo_elimina >> results/time.csv
    for metrica in $METRICAS
    do
        nome_arq=($metrica"_"$k"_"$n".csv")
        echo $nome_arq

        # results for v1
        ./gera $k $n | likwid-perfctr -O -C 3 -g $metrica -m ./ajustePol_v1 > temp.txt
        tam=$(wc -l temp.txt | cut -d' ' -f1)
        inicio=$(grep '\-\-' temp.txt -n | tail -n1 | cut -d':' -f1)
        pegar=$(($tam-$inicio))
        tail -n$pegar temp.txt > results/v1/$nome_arq

        # results for v2
        ./gera $k $n | likwid-perfctr -O -C 3 -g $metrica -m ./ajustePol_v2 > temp.txt
        tam=$(wc -l temp.txt | cut -d' ' -f1)
        inicio=$(grep '\-\-' temp.txt -n | tail -n1 | cut -d':' -f1)
        pegar=$(($tam-$inicio))
        tail -n$pegar temp.txt > results/v2/$nome_arq
    done
done

rm temp.txt
make purge
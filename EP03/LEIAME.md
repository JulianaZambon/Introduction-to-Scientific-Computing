# EP 03 — Otimização de Ajuste de Curvas

**Nome:** Juliana Zambon  
**GRR:** 20224168  
**Login DINF:** jz22

**Disciplina:** Introdução à Computação Científica – CI1164  
**Departamento:** Departamento de Informática  
**Universidade:** Universidade Federal do Paraná (UFPR)

## Introdução 

Este trabalho, referente ao EP 03 da disciplina de Introdução à Computação Científica (CI1164), tem como objetivo principal otimizar o desempenho do programa `ajustePol`. Este programa realiza o ajuste de uma curva polinomial de grau N a partir de K pontos, utilizando o Método dos Mínimos Quadrados para a geração do Sistema Linear (SL) e o Método da Eliminação de Gauss com pivoteamento parcial para sua solução.

O EP03 consistiu em duas etapas principais: a implementação de otimizações no código original `ajustePol_v1.c` para criar uma versão aprimorada `ajustePol_v2.c`, e uma análise comparativa do desempenho de ambas as versões, utilizando a ferramenta LIKWID para coletar métricas de tempo, cache, energia e operações aritméticas.

---

## Estrutura de diretórios

```c
.
├── ajustePol_v1.c
├── ajustePol_v2.c
├── data
├── gera_entrada.c
├── LEIAME.md
├── Makefile
├── scripts
│   ├── graficos.py
│   ├── saidas_likwid.py
│   └── testes.sh
├── utils.c
└── utils.h
```

## Melhoria de Desempenho

Para obter a versão otimizada `ajustePol_v2.c` a partir do código original `ajustePol_v1.c`, foram implementadas as seguintes alterações:

1.  Alteração:
    Justificativa:

2. 

## Análise de Desempenho

A análise de desempenho foi realizada comparando as duas versões do programa `ajustePol_v1.c` e `ajustePol_v2.c` sob condições controladas, utilizando a ferramenta LIKWID.

### Ambiente de Teste

- Máquina (arquitetura do processador):

```c
~$ likwid-topology -g -c
--------------------------------------------------------------------------------
CPU name:	Intel(R) Core(TM) i5-8265U CPU @ 1.60GHz
CPU type:	Intel Kabylake processor
CPU stepping:	11
********************************************************************************
Hardware Thread Topology
********************************************************************************
Sockets:		1
CPU dies:		1
Cores per socket:	4
Threads per core:	2
--------------------------------------------------------------------------------
HWThread        Thread        Core        Die        Socket        Available
0               0             0           0          0             *                
1               0             1           0          0             *                
2               0             2           0          0             *                
3               0             3           0          0             *                
4               1             0           0          0             *                
5               1             1           0          0             *                
6               1             2           0          0             *                
7               1             3           0          0             *                
--------------------------------------------------------------------------------
Socket 0:		( 0 4 1 5 2 6 3 7 )
--------------------------------------------------------------------------------
********************************************************************************
Cache Topology
********************************************************************************
Level:			1
Size:			32 kB
Type:			Data cache
Associativity:		8
Number of sets:		64
Cache line size:	64
Cache type:		Non Inclusive
Shared by threads:	2
Cache groups:		( 0 4 ) ( 1 5 ) ( 2 6 ) ( 3 7 )
--------------------------------------------------------------------------------
Level:			2
Size:			256 kB
Type:			Unified cache
Associativity:		4
Number of sets:		1024
Cache line size:	64
Cache type:		Non Inclusive
Shared by threads:	2
Cache groups:		( 0 4 ) ( 1 5 ) ( 2 6 ) ( 3 7 )
--------------------------------------------------------------------------------
Level:			3
Size:			6 MB
Type:			Unified cache
Associativity:		12
Number of sets:		8192
Cache line size:	64
Cache type:		Inclusive
Shared by threads:	8
Cache groups:		( 0 4 1 5 2 6 3 7 )
--------------------------------------------------------------------------------
********************************************************************************
NUMA Topology
********************************************************************************
NUMA domains:		1
--------------------------------------------------------------------------------
Domain:			0
Processors:		( 0 4 1 5 2 6 3 7 )
Distances:		10
Free memory:		777.988 MB
Total memory:		7784.74 MB
--------------------------------------------------------------------------------


********************************************************************************
Graphical Topology
********************************************************************************
Socket 0:
+---------------------------------------------+
| +--------+ +--------+ +--------+ +--------+ |
| |  0 4   | |  1 5   | |  2 6   | |  3 7   | |
| +--------+ +--------+ +--------+ +--------+ |
| +--------+ +--------+ +--------+ +--------+ |
| |  32 kB | |  32 kB | |  32 kB | |  32 kB | |
| +--------+ +--------+ +--------+ +--------+ |
| +--------+ +--------+ +--------+ +--------+ |
| | 256 kB | | 256 kB | | 256 kB | | 256 kB | |
| +--------+ +--------+ +--------+ +--------+ |
| +-----------------------------------------+ |
| |                   6 MB                  | |
| +-----------------------------------------+ |
+---------------------------------------------+

```

- Compilador: GCC

- Opções de Compilação: `-O3 -mavx -march=native`

- Core Utilizada (LIKWID): Core 3 

- Ferramenta de Medição: ` likwid-perfctr -- Version 5.4.1 `

### Configuração dos Testes

Os testes foram executados para os seguintes valores de N (grau do polinômio) e K (quantidade de pontos):

Valores de N:

    N1​=10

    N2​=1000

Valores de K (para ambos N1​ e N2​):

    64,128,200,256,512,600,800,1024,2000,3000,4096,6000,7000,10000,50000,10⁵

Valores adicionais de K (apenas para N1​=10):

    10⁶,10⁷,10⁸


A geração dos dados de entrada para cada teste foi realizada utilizando o programa auxiliar `gera_entrada`, em pipeline com o `ajustePol` e o likwid-perfctr, conforme os exemplos:

```c
    ./gera_entrada <Kpontos> <GrauPol>  |  ./ajustePol    (sem LIKWID)
    ./gera_entrada <Kpontos> <GrauPol>  |  likwid-perfctr -C 3 -g <metrica> -m ./ajustePol   (com LIKWID)
```

## Resultado dos Testes e Gráficos

Os resultados são apresentados em conjuntos de gráficos de linha, onde o eixo das abscissas (K) e, para tempos, o eixo das ordenadas (tempo), estão em escala logarítmica. Cada gráfico contém quatro linhas, representando as combinações: N1​+v1, N1​+v2, N2​+v1 e N2​+v2.

Foi comparado o desempenho das duas versões em relação a: (A) geração do SL pelo Método dos Mínimos Quadrados e (B) solução do SL pelo Método da Eliminação de Gauss.

### Gráficos de Tempo de Execução
Esses gráficos mostram o tempo gasto para cada etapa do programa.

- Gráfico 01
[Inserir gráfico aqui]

Explicação Gráfico 01: [Inserir explicação aqui]

### Gráficos de Cache Miss L3
Esses gráficos mostram a taxa de cache misses na cache L3.

- Gráfico

### Gráficos de Energia
Esses gráficos mostram o consumo de energia durante a execução.

- Gráfico 

### Gráficos de Operações Aritméticas
Esses gráficos mostram o desempenho em operações de ponto flutuante, utilizando o grupo FLOPS_DP do LIKWID e reportar FLOPS DP e FLOPS AVX DP, em MFLOP/s.

- Gráfico

## Conclusão

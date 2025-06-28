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
├── gera_entrada.c
├── graphics
├── LEIAME.md
├── Makefile
├── results
├── scripts
│   ├── graficos.py
│   ├── saidas_likwid.py
│   └── testes.sh
├── utils.c
└── utils.h
```

## Melhoria de Desempenho

Para obter a versão otimizada `ajustePol_v2.c` a partir do código original `ajustePol_v1.c`, foram **testadas** as seguintes alterações:

1.  **Alteração NÃO realizada:** Na versão original otimizada (v2), o cálculo das potências de `x[k]` (usadas na construção do sistema linear) era feito previamente, com armazenamento em uma matriz `x_powers[k][2n-1]`. Essa abordagem visava eliminar chamadas repetidas à função `pow()` e melhorar o desempenho via acesso contíguo à memória.
Na nova versão, essa matriz foi removida. Em vez disso, as potências de `x[k]` são calculadas de forma local, no momento em que são necessárias, utilizando um vetor temporário de tamanho fixo `(x_pow[2n-1])` dentro do loop de montagem do sistema.

    **Justificativa:** A versão anterior, embora eficiente em termos de tempo para tamanhos de entrada pequenos e médios, apresentava uso de memória proporcional a `K * (2n-1)`. Com valores grandes de K (por exemplo, K = 10⁸), o consumo ultrapassava facilmente 16 GB de RAM, levando a falhas por falta de memória (segfaults), mesmo em máquinas com especificações razoáveis.
    A nova abordagem mantém a eficiência ao: Evitar chamadas caras a `pow()` usando multiplicações iterativas; Reduzir o uso de memória para um valor constante, independente de K; Melhorar a localidade de cache, já que `x_pow` permanece na memória rápida (L1/L2); Permitir que o programa escale para valores de K muito grandes sem travar o sistema ou causar falhas de alocação.
    Embora essa mudança traga uma pequena perda de desempenho para valores pequenos de K, ela é negligenciável para N pequeno (como N = 10), e é completamente compensada pela viabilidade de execução em larga escala, principal foco deste trabalho. Nos testes realizados, a nova abordagem: Eliminou os erros de segmentação para K grandes (10⁷, 10⁸);


Para obter a versão otimizada `ajustePol_v2.c` a partir do código original `ajustePol_v1.c`, foram **implementadas** as seguintes alterações:

1.  **Redução de Uso de Memória e Otimização da Geração de Potências (Função `montaSL`)**
    * **Problema Original:** A versão original poderia alocar uma matriz `x_powers[K][2n-1]` para armazenar todas as potências de `x` para cada ponto. Para `K` muito grande (e.g., $10^8$), isso causaria um estouro de memória (`Out Of Memory`).
    * **Otimização:** Em `v2`, a matriz `x_powers` foi substituída por um vetor temporário `x_pow` de tamanho fixo (`max_power = 2 * n - 1`). Este vetor é alocado uma única vez e suas potências são calculadas **localmente para cada ponto** $x[k]$ (`x_pow[i] = x_pow[i-1] * x[k]`).
    * **Benefício:** Esta alteração permite que o programa rode com `K` muito grande (até $10^8$ ou mais) sem estourar a memória. Além disso, o cálculo iterativo `x_pow[i] = x_pow[i-1] * x[k]` é significativamente mais rápido do que chamadas repetitivas à função `pow()` da biblioteca matemática, que é mais genérica e computacionalmente mais cara. Isso impacta diretamente o tempo e a energia na fase de **geração do SL (A)**.

2.  **Reuso de Vetor Temporário para Potências (Função `montaSL`)**
    * **Problema Original:** Se houvesse cálculo de potências dentro do loop mais interno, poderia haver recalculos ou má utilização de cache.
    * **Otimização:** O vetor `x_pow` é explicitamente alocado e reutilizado para cada ponto `k`. As potências de `x[k]` são calculadas uma única vez para aquele `k` e então reutilizadas nos loops internos para preencher `A` e `b`.
    * **Benefício:** Reduz o custo computacional e o número de acessos à memória, melhorando a localidade temporal e espacial. Contribui para a eficiência na **geração do SL (A)**.

3.  **Troca de Linhas por Ponteiros (Função `eliminacaoGauss`)**
    * **Problema Original:** Em implementações ingênuas do pivoteamento parcial na Eliminação de Gauss, a troca de linhas pode envolver a cópia de grandes blocos de memória (elementos de toda a linha da matriz `A` e do vetor `b`).
    * **Otimização:** A `v2` implementa a troca de linhas da matriz `A` através da **troca de ponteiros** para as linhas. Em vez de copiar os dados, apenas os endereços de memória das linhas são trocados.
    * **Benefício:** Evita operações de cópia de memória dispendiosas, que são intensivas em tempo e largura de banda da memória, especialmente para matrizes grandes ($N=1000$). Isso melhora o desempenho na fase de **solução do SL (B)**.

4.  **Loop Unrolling na Retro-substituição (Função `retrossubs`)**
    * **Problema Original:** Loops com poucas operações por iteração podem ser ineficientes devido ao overhead de controle de laço e à subutilização das unidades de execução do processador.
    * **Otimização:** A retro-substituição foi otimizada com a técnica de *loop unrolling* com um fator de `UNROLL_FATOR = 4`. Isso significa que, dentro do loop, 4 iterações consecutivas são processadas de uma vez antes que a condição do loop seja verificada. Um loop final (`for (; j < n; j++)`) lida com os elementos restantes que não formam um grupo completo de `UNROLL_FATOR`.
    * **Benefício:** Reduz o número de verificações de condição de laço e saltos, permite que o compilador realize agendamento de instruções mais eficaz (especialmente com `-O3`), e potencialmente aproveita melhor o paralelismo em nível de instrução (ILP) e as unidades de execução do processador (como as unidades AVX/SIMD se as operações forem vetorizáveis). Contribui para a eficiência na fase de **solução do SL (B)**, especialmente para $N=1000$.

5.  **Cálculo Iterativo de $f(x)$ na Função `P()`**
    * **Problema Original:** A função `P()` (cálculo do valor do polinômio $f(x_i)$ para os resíduos) poderia usar a função `pow()` repetidamente para calcular $x^i$.
    * **Otimização:** Em vez de `pow(x_val, i)`, o cálculo de $x^i$ é feito de forma iterativa (`x_power *= x_val`).
    * **Benefício:** Elimina as chamadas de função `pow()`, que são operações genéricas e mais lentas, substituindo-as por multiplicações simples. Isso acelera o cálculo dos resíduos, que, embora não seja a parte principal do tempo de execução, contribui para a performance geral ao final do programa.

6.  **Uso de `calloc` para Inicialização de Memória**
    * **Problema Original:** `malloc` aloca memória sem inicializá-la, podendo conter "lixo" de operações anteriores.
    * **Otimização:** As alocações dos vetores `x`, `y`, `b` e `alpha` agora utilizam `calloc` em vez de `malloc`.
    * **Benefício:** `calloc` aloca a memória e a inicializa automaticamente com zeros. Isso garante que as estruturas de dados estejam limpas antes do uso, o que pode melhorar a estabilidade numérica e evitar comportamentos inesperados em certas situações, embora o impacto direto no desempenho em tempo de execução para os casos testados possa ser marginal se os dados fossem sobrescritos de qualquer forma.

7.  **Alocação Contígua da Matriz `A`**
    * **Problema Original:** A alocação de `double **A` seguida de `malloc` para cada linha individualmente resulta em linhas espalhadas na memória.
    * **Otimização:** A matriz `A` é alocada de forma contígua em um único bloco de memória (`A_data`). Em seguida, os ponteiros em `A` são configurados para apontar para as posições corretas dentro desse bloco contíguo.
    * **Benefício:** Melhora significativamente a **localidade de cache** (acessos sequenciais a elementos da matriz tendem a estar em blocos de cache adjacentes) e reduz o número de falhas de TLB (Translation Lookaside Buffer). Isso é crucial para o desempenho de algoritmos que varrem grandes matrizes como a Eliminação de Gauss e a geração do SL, levando a menos *cache misses* e acessos mais rápidos à memória. Afeta ambas as fases do programa, **geração do SL (A)** e **solução do SL (B)**.


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

Os resultados são apresentados em conjuntos de gráficos de linha, onde o eixo das abscissas (K) e, para tempos, o eixo das ordenadas (tempo), estão em escala logarítmica. Cada gráfico contém quatro linhas, representando as combinações: `N1​+v1, N1​+v2, N2​+v1 e N2​+v2`.

Foi comparado o desempenho das duas versões em relação a: `(A) geração do SL pelo Método dos Mínimos Quadrados` e `(B) solução do SL pelo Método da Eliminação de Gauss`. O objetivo é entender como as alterações no código impactam o tempo de execução, o uso de cache, o consumo de energia e as operações aritméticas, tanto na geração do sistema linear (SL) quanto na sua solução.


### Gráficos de Tempo de Execução
Esses gráficos mostram o tempo gasto para cada etapa do programa  usando `timestamp()`.

- **Gráfico 01: (A) geração do SL pelo Método dos Mínimos Quadrados**

![alt text](graphics/a/TIME.png)

**Explicação Gráfico 01:** Este gráfico apresenta o tempo de execução (em milissegundos, em escala logarítmica) para a Geração do Sistema Linear (SL). As curvas v1 10 (azul tracejado) e v1 1000 (verde tracejado) mostram um crescimento de tempo de execução significativo e aproximadamente linear em escala logarítmica, o que é esperado para um algoritmo de complexidade polinomial em relação a K. A v1 1000 é consistentemente mais lenta que a v1 10, dada a complexidade do problema com um N maior.

A versão v2 10 (amarelo sólido) e v2 1000 (preto sólido) demonstram melhorias substanciais em relação às suas contrapartes da v1. Para N=10, a v2 10 é ordens de magnitude mais rápida que a v1 10 em toda a faixa de K, com um comportamento de crescimento mais suave. Isso evidencia a alta eficácia das otimizações para graus polinomiais menores. Para N=1000, a v2 1000 também mostra um tempo de execução significativamente menor que a v1 1000, embora o crescimento com K ainda seja visível e o custo seja elevado devido ao N grande. As linhas v2 10 e v2 1000 acompanham de perto as tendências de suas respectivas v1 na proporção logarítmica, mas com um deslocamento para baixo, indicando um ganho de performance consistente.


- **Gráfico 02: (B) solução do SL pelo Método da Eliminação de Gauss**

![alt text](graphics/b/TIME.png)

Explicação Gráfico 02: 

### Gráficos de Cache Miss L3
Esses gráficos mostram a taxa de cache misses na cache L3 com o resultado de "cache miss RATIO".

- **Gráfico 01: (A) geração do SL pelo Método dos Mínimos Quadrados**

![alt text](graphics/a/L3_MISS_RATIO.png)

**Explicação Gráfico 01:**  Este gráfico ilustra a taxa de cache misses na L3 cache. Para v1 10 (azul tracejado) e v1 1000 (verde tracejado), a taxa de misses é alta e muito errática para K menores, com picos e vales acentuados. Isso sugere um padrão de acesso à memória ineficiente e falta de localidade, levando a frequentes recarregamentos de dados da memória principal para a cache L3.

As curvas da versão v2 (v2 10 em amarelo sólido e v2 1000 em preto sólido) mostram um comportamento diferente e muito mais favorável. Para v2 10, a taxa de misses começa baixa e se mantém inferior às curvas da v1 para a maioria dos valores de K, indicando aproveitamento da cache L3. A v2 1000 também demonstra uma taxa de misses mais controlada e geralmente inferior à v1 1000, especialmente para K menores. No entanto, para K muito grandes (acima de 10⁵), v2 10 e v2 1000 mostram um aumento na taxa de misses (atingindo cerca de 0.45 para v2 10), o que é esperado, pois o volume total de dados (K) eventualmente excederá a capacidade da cache L3, forçando acessos à memória principal. Mesmo nesse ponto, a v2 ainda se comporta de forma mais previsível e geralmente melhor que a v1 na gestão da cache.

- **Gráfico 02: (B) solução do SL pelo Método da Eliminação de Gauss**

![alt text](graphics/b/L3_MISS_RATIO.png)

Explicação Gráfico 02: Para resolver o sistema, as "falhas de cache" dependem principalmente do tamanho da matriz (N). Para N=10 (v1 10 e v2 10), a matriz é minúscula e cabe na cache, então as falhas são quase zero.

Para N=1000 (v1 1000 e v2 1000), a matriz é gigantesca e não cabe na cache L3. Por isso, ambas as versões têm muitas falhas de cache, e o comportamento é bem parecido entre v1 e v2 nesta métrica para N=1000. As otimizações não conseguiram mudar muito essa característica, pois o problema é naturalmente intensivo em acessos à memória para um N tão grande.


### Gráficos de Energia
Esses gráficos mostram o consumo de energia durante a execução, utilizando o grupo ENERGY do LIKWID com o resultado de "Energy [J]".

- **Gráfico 01: (A) geração do SL pelo Método dos Mínimos Quadrados**

![alt text](graphics/a/ENERGY.png)

**Explicação Gráfico 01:** Este gráfico demonstra o consumo de energia em Joules (eixo Y linear) durante a Geração do SL. As curvas v1 10 (azul tracejado) e v1 1000 (verde tracejado) mostram um crescimento acentuado e quase exponencial no consumo de energia com o aumento de K. A v1 1000 atinge valores de energia muito altos para K grandes, evidenciando o alto custo energético da versão original para problemas maiores.

As curvas v2 10 (amarelo sólido) e v2 1000 (preto sólido) apresentam um consumo de energia menor em comparação com as versões v1. Para N=10, a v2 10 praticamente se mantém próxima ao eixo X, indicando um consumo de energia insignificante para a maioria dos valores de K em comparação com a v1 10. Mesmo para N=1000, a v2 1000 exibe um crescimento de energia muito mais suave e muito inferior ao da v1 1000. Essa significativa redução no consumo de energia é um reflexo direto da redução no tempo de execução e na quantidade total de trabalho computacional realizado pela v2. O gráfico confirma a superioridade energética da versão otimizada.


- **Gráfico 02: (B) solução do SL pelo Método da Eliminação de Gauss**

![alt text](graphics/b/ENERGY.png)

**Explicação Gráfico 02:** Aqui, o consumo de energia para resolver o sistema também depende principalmente de N. Para N=10 (v1 10 e v2 10), o gasto de energia é extremamente baixo, quase nada, porque o cálculo é muito rápido.

Para N=1000 (v1 1000 e v2 1000), o consumo de energia é bem maior, mas ambas as versões gastam de forma parecida. Isso indica que as otimizações não diminuíram o custo de energia da Eliminação de Gauss em si para N grande, já que o algoritmo é inerentemente caro para esse tamanho. O consumo de energia acompanha o tempo, e os tempos para N=1000 são próximos entre v1 e v2.


### Gráficos de Operações Aritméticas
Esses gráficos mostram o desempenho em operações de ponto flutuante, utilizando o grupo FLOPS_DP do LIKWID e reportar FLOPS DP e FLOPS AVX DP, em MFLOP/s.

- **Gráfico 01: (A) geração do SL pelo Método dos Mínimos Quadrados em FLOPS DP**

![alt text](graphics/a/FLOPS_DP.png)

**Explicação Gráfico 01:** Este gráfico mostra o desempenho total em MFLOP/s para operações de ponto flutuante de precisão dupla. As curvas v1 10 (azul tracejado) e v1 1000 (verde tracejado) mantêm-se em patamares relativamente altos e estáveis (acima de 3000 MFLOP/s para v1 10 e 3000-3500 MFLOP/s para v1 1000), mas com alguma variabilidade. Isso indica que a v1 está realizando um grande número de operações de ponto flutuante por segundo.

Em contraste, as curvas v2 10 (amarelo sólido) e v2 1000 (preto sólido) apresentam taxas de FLOPS DP visivelmente mais baixas que as versões v1, especialmente para K maiores, onde se estabilizam em torno de 1500-2000 MFLOP/s. Este resultado, que parece contraintuitivo (menos FLOPS, mas mais rápido), é, na verdade, um indicativo de uma otimização na v2. Uma taxa de FLOPS DP menor, combinada com um tempo de execução reduzido (como visto no gráfico TIME.png), significa que a v2 está realizando menos operações de ponto flutuante no total para atingir o mesmo resultado. Isso é conseguido através de otimizações algorítmicas, como o cálculo iterativo de potências e a redução de cálculos redundantes, que minimizam a quantidade de trabalho efetivamente necessária. Em outras palavras, a v2 não apenas executa as operações mais rapidamente, mas também as reduz em número, tornando-se muito mais eficiente.

- **Gráfico 02: (A) geração do SL pelo Método dos Mínimos Quadrados em FLOPS AVX DP**

![alt text](graphics/a/FLOPS_AVX_DP.png)

**Explicação Gráfico 02:** Este gráfico mostra a taxa de operações de ponto flutuante de precisão dupla que utilizam as instruções AVX (MFLOP/s). As curvas v1 10 (azul tracejado) e v1 1000 (verde tracejado) permanecem em valores muito baixos, quase zero, em toda a faixa de K. Isso indica que a versão original do programa não faz uso significativo das instruções de vetorização AVX, o que é esperado para um código não otimizado para esse fim.

Em contraste, as curvas v2 10 (amarelo sólido) e v2 1000 (preto sólido) exibem uma taxa de FLOPS AVX DP significativamente maior e mais estável em relação às versões v1. A v2 1000 atinge os maiores valores de FLOPS AVX DP, indicando que as otimizações implementadas na v2 (como a alocação contígua da matriz A e a otimização de laços) permitiram que gerasse e executasse um volume muito maior de operações vetorizadas, explorando eficientemente as capacidades AVX do processador. Para v2 10, o uso de AVX também é notável e relativamente estável, embora em um nível mais baixo que v2 1000, o que é coerente com um N menor. Este gráfico é uma evidência direta do sucesso das otimizações na v2 em aproveitar o paralelismo de dados via AVX.


- **Gráfico 03:(B) solução do SL pelo Método da Eliminação de Gauss em FLOPS DP**

![alt text](graphics/b/FLOPS_DP.png)

**Explicação Gráfico 03:** Aqui vemos as operações matemáticas na resolução do sistema. Para N=10 (v1 10 e v2 10), o número de operações é baixo, pois a matriz é pequena.

Para N=1000 (v1 1000 e v2 1000), o número de operações é muito alto, como esperado para a Eliminação de Gauss. Ambas as versões (v1 e v2) fazem muitas operações, e são bem parecidas. A v2 1000 pode ter picos um pouco mais altos, mostrando uma pequena otimização no ritmo de execução.

- **Gráfico 04: (B) solução do SL pelo Método da Eliminação de Gauss em FLOPS AVX DP**

![alt text](graphics/b/FLOPS_AVX_DP.png)

**Explicação Gráfico 04:** Este gráfico mostra o uso de instruções especiais do processador (AVX) para fazer cálculos mais rápidos. As versões antigas (v1 10 e v1 1000) quase não usam AVX.

Já as versões novas (v2 10 e v2 1000) mostram um uso significativamente maior e mais estável de AVX. A v2 1000 é a que mais usa AVX. Isso prova que as otimizações na v2 permitiram que o programa aproveitasse melhor o processador para fazer cálculos em paralelo.

#### Resultados de AVX Flops

Os gráficos de FLOPS AVX DP mostram como a v2 usa as partes mais rápidas do processador (AVX). Para a Montagem do SL (A), a v2 (especialmente para N=10) tem menos FLOPS AVX totais, mas é mais rápida. Isso significa que ela não só executa mais rápido, mas também precisa fazer menos cálculos no total para chegar ao resultado. Para a Solução do SL (B), a v2 (para N=1000) mostra um aumento nos FLOPS AVX, o que significa que ela está fazendo as operações da Eliminação de Gauss de forma mais paralela e eficiente, usando as instruções AVX para acelerar o processo.

## Conclusão

Este trabalho demonstrou a importância e o impacto significativo de otimizações de código de baixo nível no desempenho de algoritmos numéricos intensivos. Através da comparação entre a versão original (`v1`) e a versão otimizada (`v2`) do programa `ajustePol`, foi possível observar ganhos substanciais em diversas métricas de desempenho.

Em suma, este trabalho valida a eficácia das otimizações propostas. Na Montagem do Sistema (A), a v2 foi muito mais rápida e gastou muito menos energia, especialmente para poucos graus de polinômio (N=10). Isso aconteceu porque reorganizamos o uso da memória e fizemos o programa fazer menos trabalho desnecessário no total (por isso, menos FLOPS, mas mais velocidade). Conseguimos rodar o programa com K gigantesco, o que antes causava erros de memória.

Na Solução do Sistema (B), as melhorias foram mais evidentes para N=1000. A v2 foi um pouco mais rápida e eficiente, principalmente por usar melhor as capacidades de processamento paralelo do computador (instruções AVX). Para N=10, como o cálculo já era muito rápido, as otimizações não mudaram muito o resultado.

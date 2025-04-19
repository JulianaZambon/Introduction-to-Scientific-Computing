# EP-02 - Sistemas Lineares

## Objetivo

Implementar um programa para calcular a solução de uma família de Equações Diferenciais Ordinárias (EDO's) usando o Método da Fatoração LU.

---

## Enunciado

Considere uma EDO genérica do tipo: y′′ + p·y′ + q·y = r(x)
, onde p e q são coeficientes reais e r(x) = r1x + r2x² + r3cos(x) + r4ex , sendo r1, r2, r3  e r4 coeficientes reais.

Faça um programa chamado resolveEDO que leia pela entrada padrão (stdin) a definição de uma família de EDO's genéricas e gera na saída a solução destas EDO.

O formato de entrada dos dados consiste de:

    1ª linha: quantidade de pontos da malha da EDO;
    2ª linha: intervalo a e b onde a EDO é válida;
    3ª linha: os valores de contorno  y(a) e y(b);
    4ª linha: os coeficientes p e q da EDO genérica;
    5ª linha em diante: uma ou mais linhas contendo os coeficientes r1, r2, r3  e r4 da definição da função r(x), representando diversas  EDO's que diferem apenas no valor de r(x).

O programa deverá gerar na saída padrão (stdout), para cada EDO, a ordem do Sistema linear tri-diagonal 1ª linha, seguida da matriz aumentada do SL tridiagonal (matriz de coeficientes e termos independentes, veja exemplo abaixo). Após, deve ser exibida a solução do SL, obtida usando-se o Método de Fatoração LU.

Finalmente deve ser impresso o tempo em milisegundos, gasto no cálculo da solução.

Todos os cálculos de ponto flutuante devem ser efetuados com precisão dupla e com arredondamento para baixo (use a função  fesetround ).

Os valores de saída devem ser mostrados com 15  casas decimais (formato "%.15e"). O valor do tempo da solução deve ser exibido com 8 casas decimais (formato "%.8e").

ATENÇÃO: nada deve ser impresso além dos valores citados, pois os resultados serão verificados via script. Implementações que não respeitarem essa restrição receberão nota zero. Para testar se os dados estão sendo impressos de maneira correta, execute o script anexo a este enunciado.

---

### Exemplo de Execução do programa

Entrada:

5
0 3.14159265
2 -2
0 1
0 0 0 0
0 0 1 0

Saída:

5
 -1.725844322818500e+00  1.000000000000000e+00  0.000000000000000e+00  0.000000000000000e+00  0.000000000000000e+00 -2.000000000000000e+00
  1.000000000000000e+00 -1.725844322818500e+00  1.000000000000000e+00  0.000000000000000e+00  0.000000000000000e+00  0.000000000000000e+00
  0.000000000000000e+00  1.000000000000000e+00 -1.725844322818500e+00  1.000000000000000e+00  0.000000000000000e+00  0.000000000000000e+00
  0.000000000000000e+00  0.000000000000000e+00  1.000000000000000e+00 -1.725844322818500e+00  1.000000000000000e+00 -0.000000000000000e+00
  0.000000000000000e+00  0.000000000000000e+00  0.000000000000000e+00  1.000000000000000e+00 -1.725844322818500e+00  2.000000000000000e+00

  1.744564700038279e+00  1.010847083350622e+00  3.147679729430032e-14 -1.010847083350569e+00 -1.744564700038249e+00
  3.02900187e-03

5
 -1.725844322818500e+00  1.000000000000000e+00  0.000000000000000e+00  0.000000000000000e+00  0.000000000000000e+00 -1.762574218887082e+00
  1.000000000000000e+00 -1.725844322818500e+00  1.000000000000000e+00  0.000000000000000e+00  0.000000000000000e+00  1.370778388748534e-01
  0.000000000000000e+00  1.000000000000000e+00 -1.725844322818500e+00  1.000000000000000e+00  0.000000000000000e+00  4.920811303936179e-10
  0.000000000000000e+00  0.000000000000000e+00  1.000000000000000e+00 -1.725844322818500e+00  1.000000000000000e+00 -1.370778380225440e-01
  0.000000000000000e+00  0.000000000000000e+00  0.000000000000000e+00  1.000000000000000e+00 -1.725844322818500e+00  1.762574219379162e+00

  1.468180054997135e+00  7.712759939050777e-01  7.928499164372629e-08 -7.712758565794439e-01 -1.468179975712145e+00
  6.71999529e-04


---

No exemplo acima, duas EDO's foram solucionadas, ambas válidas no intervalo x∈(0,π) e com condições de contorno y(0)=−2 e y(π)=2 :
- y′′+ y= 0
- y′′+ y= cos(x)




---

## Execução do LIKWID

Faça um script que após executar o programa resolveEDO, use o LIKWID para exibir APENAS as informações abaixo:

- Operações aritméticas: utilizar o grupo FLOPS_DP do LIKWID e reportar o contador FP_ARITH_INST_RETIRED_SCALAR_DOUBLE e a métrica  DP (em MFLOP/s). Devem ser obtidos estes valores para o cálculo de cada solução das EDO's (vide abaixo).


### Exemplo de execução do script

Entrada:

5
0 3.14159265
2 -2
0 1
0 0 0 0
0 0 1 0

Saída:

5
 -1.725844322818500e+00  1.000000000000000e+00  0.000000000000000e+00  0.000000000000000e+00  0.000000000000000e+00 -2.000000000000000e+00
  1.000000000000000e+00 -1.725844322818500e+00  1.000000000000000e+00  0.000000000000000e+00  0.000000000000000e+00  0.000000000000000e+00
  0.000000000000000e+00  1.000000000000000e+00 -1.725844322818500e+00  1.000000000000000e+00  0.000000000000000e+00  0.000000000000000e+00
  0.000000000000000e+00  0.000000000000000e+00  1.000000000000000e+00 -1.725844322818500e+00  1.000000000000000e+00 -0.000000000000000e+00
  0.000000000000000e+00  0.000000000000000e+00  0.000000000000000e+00  1.000000000000000e+00 -1.725844322818500e+00  2.000000000000000e+00

  1.744564700038279e+00  1.010847083350622e+00  3.147679729430032e-14 -1.010847083350569e+00 -1.744564700038249e+00
  3.02900187e-03

5
 -1.725844322818500e+00  1.000000000000000e+00  0.000000000000000e+00  0.000000000000000e+00  0.000000000000000e+00 -1.762574218887082e+00
  1.000000000000000e+00 -1.725844322818500e+00  1.000000000000000e+00  0.000000000000000e+00  0.000000000000000e+00  1.370778388748534e-01
  0.000000000000000e+00  1.000000000000000e+00 -1.725844322818500e+00  1.000000000000000e+00  0.000000000000000e+00  4.920811303936179e-10
  0.000000000000000e+00  0.000000000000000e+00  1.000000000000000e+00 -1.725844322818500e+00  1.000000000000000e+00 -1.370778380225440e-01
  0.000000000000000e+00  0.000000000000000e+00  0.000000000000000e+00  1.000000000000000e+00 -1.725844322818500e+00  1.762574219379162e+00

  1.468180054997135e+00  7.712759939050777e-01  7.928499164372629e-08 -7.712758565794439e-01 -1.468179975712145e+00
  6.71999529e-04


--- 

FP_ARITH_INST_RETIRED_SCALAR_DOUBLE,38
DP [MFLOP/s],12.2784
FP_ARITH_INST_RETIRED_SCALAR_DOUBLE,38
DP [MFLOP/s],42.9362



---

É imprescindível que sejam respeitadas as seguintes condições:

- Os códigos devem ser compilados com GCC e a opção -O0

- O código deve ser instrumentado com a biblioteca do LIKWID;

- Os códigos devem ser compilados na mesma máquina utilizada para os testes. Você pode escolher um computador de sua preferência, desde que possua os contadores Likwid especificados. Não utilize as servidoras de processamento do DInf que tenham uso compartilhado. Elas podem ser máquinas virtuais e o compartilhamento impede medidas de desempenho. Em caso de dúvida, consulte o professor.

---

## Makefile

O arquivo Makefile deve possuir as regras necessárias para compilar os módulos individualmente e gerar o programa executável. As seguintes regras devem existir OBRIGATORIAMENTE:

all: compila e produz um executável chamado resolveEDO no diretório corrente;

purge: remove todos os arquivos temporários e os arquivos gerados pela compilação do programa (*.o, executável, etc.). 

---

## Código Modular

Você deve deixar seu código modular, com funções e arquivos específicos para implementar cada parte do programa. O programa principal deve estar em um módulo separado dos demais (resolveEDO.c), contendo apenas a função main().

As funções/código-fonte devem conter comentários adequados descrevendo cada função.

A documentação do código deve conter:

- Nome e GRR (em cada código-fonte)

- A descrição de cada função e dos seus parâmetros

- A descrição das estruturas de dados

- Todos os possíveis códigos de erro que a função pode produzir, explicando o significado de cada um deles

- Explicações sobre o algoritmo que por ventura sejam necessárias para a compreensão do código


---

## Arquivos auxiliares 

O arquivo edos.dat apresenta vários exemplos de matrizes para inversão.  Observe  que o programa resolveEDO deve ler APENAS 1 (UMA) matriz e  apresentar o resultado. O arquivo edos.dat  é apenas um conjunto de exemplos, não a entrada do programa.

Os arquivos utils.* contém funções de uso geral, incluindo a função timestamp(), a ser usada neste trabalho e nos próximos.

O arquivo Makefile é um exemplo que pode ser usado e já contém as regras all, clean, purge, e adicionalmente a regra dist, que pode ser usada para gerar o arquivo tgz a ser entregue.



#include <stdio.h>
#include <math.h>
#include <float.h>
#include <fenv.h> // Para arredondamento

#include "utils.h"
#include "ZeroFuncao.h"

/*
 * Instruções:
 *
 * 1. Todos os cálculos de ponto flutuante devem ser feitos com arredondamento para baixo.
 *    Utilize a função 'fesetround' para definir o arredondamento para o modo de truncamento.
 *
 * 2. Os valores de saída devem ser exibidos com 15 casas decimais utilizando o formato: "%.15e".
 *
 * 3. O tempo de cálculo deve ser exibido com 8 casas decimais utilizando o formato: "%.8e".
 */

/*
 * FORMATAÇÃO DA SAÍDA:
 *
 * RAPIDO:
 * A saída será composta pelas seguintes linhas:
 * - bissec <raiz> <valor_crit-01> <it> <tempo>
 * - bissec <raiz> <valor_crit-02> <it> <tempo>
 * - bissec <raiz> <valor_crit-03> <it> <tempo>
 * - newton <raiz> <valor_crit-01> <it> <tempo>
 * - newton <raiz> <valor_crit-02> <it> <tempo>
 * - newton <raiz> <valor_crit-03> <it> <tempo>
 *
 * LENTO:
 * A saída será composta pelas seguintes linhas:
 * - bissec <raiz> <valor_crit-01> <it> <tempo>
 * - bissec <raiz> <valor_crit-02> <it> <tempo>
 * - bissec <raiz> <valor_crit-03> <it> <tempo>
 * - newton <raiz> <valor_crit-01> <it> <tempo>
 * - newton <raiz> <valor_crit-02> <it> <tempo>
 * - newton <raiz> <valor_crit-03> <it> <tempo>
 */

// Variável global para armazenar o ponteiro da função de cálculo do polinômio
void (*calcPolinomio)(Polinomio, real_t, real_t *, real_t *);

// Funções de execução de métodos, cada uma com 3 critérios de parada


int main()
{
  // arredondamento para baixo 
  fesetround(FE_DOWNWARD);

  real_t a, b;
  Polinomio pol;

  // Leitura do polinômio:
  scanf("%d", &pol.grau);
  for (int i = pol.grau; i >= 0; --i) // Lê os coeficientes do polinômio
    scanf("%lf", &pol.p[i]);
  scanf("%lf %lf", &a, &b); // Intervalo onde está uma das raízes


  // vai executar os métodos cada um 3 vezes com calcPolinomio_rapido
  // com os critérios de parada 1, 2 e 3
  printf("RAPIDO:\n");
  calcPolinomio = calcPolinomio_rapido;


  // vai executar os métodos cada um 3 vezes com calcPolinomio_lento
  // com os critérios de parada 1, 2 e 3
  printf("LENTO:\n");
  calcPolinomio = calcPolinomio_lento;
  

  return 0;
}

#include <stdio.h>
#include <math.h>
#include <float.h>

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


int main()
{
  real_t a, b;
  Polinomio pol;

  // Leitura do polinômio:
  // - O primeiro valor lido será o grau do polinômio.
  // - Os próximos valores serão os coeficientes do polinômio, começando do coeficiente do maior grau.
  // - Os dois últimos valores representam o intervalo [a,b] onde pelo menos uma das raízes do polinômio se encontra.
  scanf("%d", &pol.grau);
  for (int i = pol.grau; i >= 0; --i)
    scanf("%lf", &pol.p[i]);
  scanf("%lf %lf", &a, &b); // Intervalo onde está uma das raízes

  // Restante do programa a partir daqui

  return 0;
}

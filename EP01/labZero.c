#include <stdio.h>
#include <math.h>
#include <float.h>
#include <fenv.h>

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

// Função para executar os métodos de bissecção e Newton-Raphson com os critérios
// de parada e calcPolinomioLento e calcPolinomioRapido passados como parâmetros
void executarMetodos(Polinomio pol, real_t a, real_t b)
{
  int it;
  real_t raiz, erro;

  // Bissecção
  for (int i = 1; i <= 3; ++i)
  {
    erro = bisseccao(pol, a, b, i, &it, &raiz);
    double tempo = timestamp();
    printf("bissec %.15e %.15e %d %.8e\n", raiz, erro, it, tempo);
  }

  // Newton-Raphson
  for (int i = 1; i <= 3; ++i)
  {
    erro = newtonRaphson(pol, a, i, &it, &raiz);
    double tempo = timestamp();
    printf("newton %.15e %.15e %d %.8e\n", raiz, erro, it, tempo);
  }
}

int main()
{
  // arredondamento para baixo
  fesetround(FE_DOWNWARD);

  real_t a, b;
  Polinomio pol;

  // Leitura do polinômio
  scanf("%d", &pol.grau);
  pol.p = malloc((pol.grau + 1) * sizeof(real_t));
  if (!pol.p)
  {
    fprintf(stderr, "Erro de alocação\n");
    return 1;
  }

  for (int i = pol.grau; i >= 0; --i) // Lê os coeficientes do polinômio
    scanf("%lf", &pol.p[i]);
  scanf("%lf %lf", &a, &b); // Intervalo onde está uma das raízes

  // vai executar os métodos cada um 3 vezes com calcPolinomio_rapido
  // com os critérios de parada 1, 2 e 3
  printf("RAPIDO:\n");
  calcPolinomio = calcPolinomio_rapido;
  executarMetodos(pol, a, b);

  // vai executar os métodos cada um 3 vezes com calcPolinomio_lento
  // com os critérios de parada 1, 2 e 3
  printf("LENTO:\n");
  calcPolinomio = calcPolinomio_lento;
  executarMetodos(pol, a, b);

  return 0;
}

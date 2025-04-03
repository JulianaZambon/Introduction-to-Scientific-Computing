#include <stdio.h>
#include <math.h>
#include <float.h>
#include <fenv.h>

#include "utils.h"
#include "ZeroFuncao.h"

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
  // Arredondamento para baixo
  fesetround(FE_DOWNWARD);

  real_t a, b;
  Polinomio pol;

  // Leitura do polinômio
  scanf("%d", &pol.grau);
  pol.p = malloc((pol.grau + 1) * sizeof(real_t));
  if (!pol.p)
    return 1;

  for (int i = pol.grau; i >= 0; --i) // Lê os coeficientes do polinômio
    scanf("%lf", &pol.p[i]);
  scanf("%lf %lf", &a, &b); // Intervalo onde está uma das raízes

  printf("RAPIDO:\n");
  printf("\n");
  calcPolinomio = calcPolinomio_rapido;
  real_t resultado;
  calcPolinomio(pol, 0, &resultado, NULL);
  printf("P(0) = %.6lf (esperado: -30.000000)\n", resultado);
  printf("\n");
  executarMetodos(pol, a, b);
  printf("\n");

  printf("LENTO:\n");
  printf("\n");
  calcPolinomio = calcPolinomio_lento;
  executarMetodos(pol, a, b);

  return 0;
}

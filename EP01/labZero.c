#include <stdio.h>
#include <math.h>
#include <float.h>

#include "utils.h"
#include "ZeroFuncao.h"

int main()
{
  real_t a, b;
  Polinomio pol;

  // Leitura do polinômio = grau n, coeficientes p e intervalo [a,b] contendo as raízes
  scanf("%d", &pol.grau);
  for (int i = pol.grau; i >= 0; --i)
    scanf("%lf", &pol.p[i]);
  scanf("%lf %lf", &a, &b); // Intervalo onde está uma das raizes

  // Restante do programa a partir daqui

  return 0;
}

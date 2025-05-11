/*
 * Nome: Juliana Zambon
 * GRR: 20224168
 * Login DINF: jz22
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "utils.h"
#include "edo.h"

// Recebe a estrutura de dados EDo e retorna a estrutura Tridiag,
// que representa o sistema linear tridiagonal

// A matriz tridiagonal é representada pelos vetores:
// D: diagonal principal
// Di: diagonal inferior (abaixo da diagonal principal)
// Ds: diagonal superior (acima da diagonal principal)
// B: vetor de termos independentes (lado direito do sistema)
// A função também calcula a malha espacial com base nos parâmetros fornecidos na EDO.
Tridiag *genTridiag(EDo *edo)
{
  Tridiag *sl;
  real_t x, rx;
  int n = edo->n;

  sl = (Tridiag *)malloc(sizeof(Tridiag));
  sl->n = edo->n;

  sl->D = (real_t *)calloc(n, sizeof(real_t));
  sl->Di = (real_t *)calloc(n, sizeof(real_t));
  sl->Ds = (real_t *)calloc(n, sizeof(real_t));
  sl->B = (real_t *)calloc(n, sizeof(real_t));

  real_t h = (edo->b - edo->a) / (n + 1);

  for (int i = 0; i < n; ++i)
  {
    x = edo->a + (i + 1) * h;
    // Calcula o valor da função no ponto x usando a equação diferencial dada
    // rx = r1 * x + r2 * x^2 + r3 * cos(x) + r4 * exp(x)
    rx = edo->r1 * x + edo->r2 * x * x + edo->r3 * cos(x) + edo->r4 * exp(x);

    // Preenche as diagonais e o vetor B do sistema linear tridiagonal
    sl->B[i] = h * h * rx;
    sl->Di[i] = 1 - h * edo->p / 2.0;
    sl->D[i] = -2 + h * h * edo->q;
    sl->Ds[i] = 1 + h * edo->p / 2.0;
  }

  sl->B[0] -= edo->ya * (1 - h * edo->p / 2.0);
  sl->B[n - 1] -= edo->yb * (1 + h * edo->p / 2.0);

  return sl;
}

// Exibe o sistema linear tridiagonal na saída padrão no formato:
// D1 Di1 Ds1 B1
// D2 Di2 Ds2 B2
// ...
// Dn Din Dsn Bn
// Onde D, Di, Ds são os elementos da diagonal principal, inferior e superior,
// respectivamente, e B é o vetor de termos independentes.
// O número de pontos internos (n) é impresso na primeira linha.
void prnEDOsl(EDo *edoeq)
{
  int n = edoeq->n, i, j;
  real_t x, b, d, di, ds, rx;
  real_t h = (edoeq->b - edoeq->a) / (n + 1);

  printf("%d\n", n);

  for (i = 0; i < n; ++i)
  {
    x = edoeq->a + (i + 1) * h;
    // Calcula o valor da função no ponto x usando a equação diferencial dada
    // rx = r1 * x + r2 * x^2 + r3 * cos(x) + r4 * exp(x)
    rx = edoeq->r1 * x + edoeq->r2 * x * x + edoeq->r3 * cos(x) + edoeq->r4 * exp(x);

    // Preenche as diagonais e o vetor B do sistema linear tridiagonal
    b = h * h * rx;
    di = 1 - h * edoeq->p / 2.0;
    d = -2 + h * h * edoeq->q;
    ds = 1 + h * edoeq->p / 2.0;

    // Imprime os elementos do sistema linear tridiagonal
    for (j = 0; j < n; ++j)
    {
      if (i == j)
        printf(FORMAT, d);
      else if (j == i - 1 && i)
        printf(FORMAT, di);
      else if (j == i + 1 && i != n - 1)
        printf(FORMAT, ds);
      else
        printf(FORMAT, 0.0);
    }

    if (i == 0)
      b -= edoeq->ya * (1 - h * edoeq->p / 2.0);
    else if (i == n - 1)
      b -= edoeq->yb * (1 + h * edoeq->p / 2.0);

    // gerava saída que não era compatível com o especificado no enunciado, causando erro no script de teste
    // printf(" | ");
    printf(FORMAT, b);

    printf("\n");
  }
}

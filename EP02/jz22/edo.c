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

// Gera sistema linear tridiagonal a partir dos dados da EDO.
// Retorna ponteiro para estrutura com as diagonais (D, Di, Ds) e vetor B.
//
// D  = diagonal principal
// Di = diagonal inferior (abaixo da diagonal principal)
// Ds = diagonal superior (acima da diagonal principal)
// B  = vetor dos termos independentes (lado direito do sistema)
//
// Calcula também o vetor espacial x com base no intervalo [a,b] e nº de pontos n.
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
    // Calcula valor da função r(x) que depende dos coeficientes r1..r4 e x
    rx = edo->r1 * x + edo->r2 * x * x + edo->r3 * cos(x) + edo->r4 * exp(x);

    // Preenche vetor B e diagonais da matriz segundo o esquema de diferenças finitas
    sl->B[i] = h * h * rx;
    sl->Di[i] = 1 - h * edo->p / 2.0;
    sl->D[i] = -2 + h * h * edo->q;
    sl->Ds[i] = 1 + h * edo->p / 2.0;
  }
  // Ajusta vetor B para incluir condições de contorno y(a) e y(b)
  sl->B[0] -= edo->ya * (1 - h * edo->p / 2.0);
  sl->B[n - 1] -= edo->yb * (1 + h * edo->p / 2.0);

  return sl;
}

// Imprime sistema linear tridiagonal no formato especificado:
// Linha 1: número de pontos internos n
// Seguintes linhas: matriz com diagonais D, Di, Ds e vetor B da EDO.
//
// Imprime zeros fora da faixa da tridiagonal, pra manter formato da matriz cheia.
//
// Ajusta termos independentes B com as condições de contorno.
void prnEDOsl(EDo *edoeq)
{
  int n = edoeq->n, i, j;
  real_t x, b, d, di, ds, rx;
  real_t h = (edoeq->b - edoeq->a) / (n + 1);

  printf("%d\n", n);

  for (i = 0; i < n; ++i)
  {
    x = edoeq->a + (i + 1) * h;
    rx = edoeq->r1 * x + edoeq->r2 * x * x + edoeq->r3 * cos(x) + edoeq->r4 * exp(x);

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

    // Aplica condições de contorno só nas extremidades
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

// Método de fatoração LU para resolver sistema linear tridiagonal
// sl: estrutura com os vetores da matriz (D, Di, Ds) e vetor B (termo independente)
// sol: vetor onde será armazenada a solução
void resolveLU(Tridiag *sl, real_t *sol)
{
  int n = sl->n;

  // Transformação da matriz tridiagonal para forma LU,
  // armazenando os coeficientes modificados em sl->D e sl->Di.
  for (int i = 1; i < n; ++i)
  {
    sl->Di[i] /= sl->D[i - 1];             // Atualiza multiplicador L(i, i-1)
    sl->D[i] -= sl->Di[i] * sl->Ds[i - 1]; // Atualiza diagonal principal (U)
    sl->B[i] -= sl->Di[i] * sl->B[i - 1];  // Modifica vetor B para refletir fatoração
  }

  // Resolução do sistema Ly = B (com L triangular inferior),
  // seguida de Ux = y (com U triangular superior).
  // Como a fatoração é feita in-place, a solução é calculada por substituição regressiva.

  sol[n - 1] = sl->B[n - 1] / sl->D[n - 1]; // Calcula última variável

  for (int i = n - 2; i >= 0; --i)
  {
    sol[i] = (sl->B[i] - sl->Ds[i] * sol[i + 1]) / sl->D[i]; // Substituição regressiva
  }
}

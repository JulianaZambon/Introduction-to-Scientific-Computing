/*
 * Nome: Juliana Zambon
 * GRR: 20224168
 * Login DINF: jz22
 */

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <fenv.h>
#include <math.h>
#include <stdint.h>

#include "utils.h" // Assumindo que este arquivo contém a função timestamp()

// ---
// ## Ajuste de Curvas
// ---

void montaSL(double **A, double *b, int n, long long int p, double *x, double *y)
{
  // Pré-calcula potências de x[k]
  // Isso evita múltiplas chamadas a pow() dentro dos loops, que é uma operação custosa.
  // Para cada x[k], armazenamos suas potências de 0 até 2*(n-1).
  // O expoente máximo é (n-1) + (n-1) = 2n - 2.
  // A alocação aqui é por linha para facilitar o acesso, mas os dados dentro de cada linha
  // de x_powers[k] são contíguos.
  double **x_powers = (double **)malloc(sizeof(double *) * p);

  int max_power = 2 * n - 1; // Potências de x[k]^0 até x[k]^(2n-2)

  for (long long int k = 0; k < p; ++k)
  {
    x_powers[k] = (double *)malloc(sizeof(double) * max_power);

    x_powers[k][0] = 1.0; // x[k]^0

    if (max_power > 1)
    {
      x_powers[k][1] = x[k]; // x[k]^1
      for (int pow_idx = 2; pow_idx < max_power; ++pow_idx)
      {
        x_powers[k][pow_idx] = x_powers[k][pow_idx - 1] * x[k];
      }
    }
  }

  // Preenche a matriz A e o vetor b usando as potências pré-calculadas
  for (int i = 0; i < n; ++i)
  {
    for (int j = 0; j < n; ++j)
    {
      A[i][j] = 0.0;
      for (long long int k = 0; k < p; ++k)
      {
        A[i][j] += x_powers[k][i + j];
      }
    }
  }

  for (int i = 0; i < n; ++i)
  {
    b[i] = 0.0;
    for (long long int k = 0; k < p; ++k)
    {
      b[i] += x_powers[k][i] * y[k];
    }
  }

  // Libera a memória das potências pré-calculadas
  for (long long int k = 0; k < p; ++k)
  {
    free(x_powers[k]);
  }
  free(x_powers);
}

void eliminacaoGauss(double **A, double *b, int n)
{
  for (int i = 0; i < n; ++i)
  {
    // Encontra o pivô máximo na coluna atual
    int iMax = i;
    for (int k = i + 1; k < n; ++k)
    {
      if (fabs(A[k][i]) > fabs(A[iMax][i]))
      { // Usar fabs para pivoteamento parcial
        iMax = k;
      }
    }

    // Troca as linhas se o pivô máximo não estiver na posição atual
    if (iMax != i)
    {
      // Troca de ponteiros para as linhas da matriz A (mais eficiente que copiar elementos)
      double *tmp_A = A[i];
      A[i] = A[iMax];
      A[iMax] = tmp_A;

      // Troca os elementos correspondentes no vetor b
      double aux_b = b[i];
      b[i] = b[iMax];
      b[iMax] = aux_b;
    }

    // Eliminação
    for (int k = i + 1; k < n; ++k)
    {
      double m = A[k][i] / A[i][i]; // Multiplicador
      A[k][i] = 0.0;                // Este elemento se tornará zero após a operação

      // Loop para subtrair a linha pivô das linhas abaixo
      // O acesso sequencial a A[k][j] e A[i][j] é otimizado pelo cache.
      for (int j = i + 1; j < n; ++j)
      {
        A[k][j] -= A[i][j] * m;
      }
      b[k] -= b[i] * m;
    }
  }
}

void retrossubs(double **A, double *b, double *x, int n)
{
  for (int i = n - 1; i >= 0; --i)
  {
    x[i] = b[i];
    // Loop para subtrair os termos já conhecidos
    // O acesso sequencial a A[i][j] e x[j] é otimizado pelo cache.
    for (int j = i + 1; j < n; ++j)
    {
      x[i] -= A[i][j] * x[j];
    }
    x[i] /= A[i][i];
  }
}

// Otimização para P: calcula potências de x de forma iterativa, evitando chamadas repetidas a pow().
double P(double x_val, int N, double *alpha)
{
  double Px = alpha[0];
  double x_power = 1.0; // x^0

  for (int i = 1; i <= N; ++i)
  {
    x_power *= x_val; // Calcula x^i a partir de x^(i-1)
    Px += alpha[i] * x_power;
  }
  return Px;
}

// ---
// ## Função Principal
// ---

int main()
{
  int N, n;
  long long int K, p;

  scanf("%d %lld", &N, &K);
  p = K;     // quantidade de pontos
  n = N + 1; // tamanho do SL (grau N + 1)

  double *x = (double *)malloc(sizeof(double) * p);
  double *y = (double *)malloc(sizeof(double) * p);

  // ler numeros
  for (long long int i = 0; i < p; ++i)
    scanf("%lf %lf", x + i, y + i);

  // Alocação da matriz A de forma contígua em memória.
  // Isso é crucial para o desempenho do cache, especialmente para N=1000.
  double **A = (double **)malloc(sizeof(double *) * n);

  // Aloca um único bloco grande de memória para todos os elementos da matriz
  double *A_data = (double *)malloc(sizeof(double) * n * n);

  // Faz com que cada A[i] aponte para o início da linha i no bloco contíguo
  for (int i = 0; i < n; ++i)
  {
    A[i] = &(A_data[i * n]);
  }

  // Alocação de memória para os vetores b e alpha (coeficientes do ajuste)
  double *b = (double *)malloc(sizeof(double) * n);
  double *alpha = (double *)malloc(sizeof(double) * n);

  // (A) Geração do Sistema Linear (SL)
  double tSL = timestamp();
  montaSL(A, b, n, p, x, y);
  tSL = timestamp() - tSL;

  // (B) Resolução do Sistema Linear
  double tEG = timestamp();
  eliminacaoGauss(A, b, n);
  retrossubs(A, b, alpha, n);
  tEG = timestamp() - tEG;

  // Imprime os coeficientes do polinômio ajustado
  for (int i = 0; i < n; ++i)
  {
    printf("%1.15e ", alpha[i]);
  }
  puts("");

  // Imprime os resíduos (diferença absoluta entre y_real e P(x_real))
  for (long long int i = 0; i < p; ++i)
  {
    printf("%1.15e ", fabs(y[i] - P(x[i], N, alpha)));
  }
  puts("");

  // Imprime os tempos de execução
  printf("%lld %1.10e %1.10e\n", K, tSL, tEG);

  // Liberação de toda a memória alocada
  free(x);
  free(y);
  free(A_data); // Libera o bloco contíguo de dados da matriz A
  free(A);      // Libera os ponteiros para as linhas da matriz A
  free(b);
  free(alpha);

  return 0;
}
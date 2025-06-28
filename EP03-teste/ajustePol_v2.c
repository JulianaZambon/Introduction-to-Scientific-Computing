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
#include <likwid.h>
#include "utils.h"

#define UNROLL_FATOR 4  // Otimização: loop unrolling para a retro-substituição

// ----------------------------
// Otimização 1: Redução de uso de memória
// Ao invés de alocar uma matriz x_powers[K][2n-1], calculamos localmente as potências
// em um vetor temporário de tamanho fixo, reaproveitado por iteração.
// Isso permite rodar K = 10^8 sem estouro de memória.
// ----------------------------

void montaSL(double **A, double *b, int n, long long int p, double *x, double *y) {
    int max_power = 2 * n - 1;

    // Inicialização das matrizes
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            A[i][j] = 0.0;

    for (int i = 0; i < n; ++i)
        b[i] = 0.0;

    // Otimização 2: Reuso de vetor temporário para potências
    double *x_pow = (double *)malloc(sizeof(double) * max_power);
    if (x_pow == NULL) {
        fprintf(stderr, "Erro: memória insuficiente para x_pow\n");
        exit(EXIT_FAILURE);
    }

    for (long long int k = 0; k < p; ++k) {
        x_pow[0] = 1.0;
        for (int i = 1; i < max_power; ++i)
            x_pow[i] = x_pow[i - 1] * x[k];  // Cálculo iterativo, evita pow()

        // Acumuladores para A e b
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                A[i][j] += x_pow[i + j];
            }
            b[i] += x_pow[i] * y[k];
        }
    }

    free(x_pow);
}

// ----------------------------
// Otimização 3: Troca de linhas via ponteiros na eliminação de Gauss
// Evita cópia de dados e melhora performance
// ----------------------------
void eliminacaoGauss(double **A, double *b, int n) {
    for (int i = 0; i < n; ++i) {
        int iMax = i;
        for (int k = i + 1; k < n; ++k) {
            if (fabs(A[k][i]) > fabs(A[iMax][i]))
                iMax = k;
        }

        // Troca de ponteiros de linha (mais eficiente que copiar)
        if (iMax != i) {
            double *tmp_A = A[i];
            A[i] = A[iMax];
            A[iMax] = tmp_A;

            double aux_b = b[i];
            b[i] = b[iMax];
            b[iMax] = aux_b;
        }

        // Eliminação
        for (int k = i + 1; k < n; ++k) {
            double m = A[k][i] / A[i][i];
            A[k][i] = 0.0;
            for (int j = i + 1; j < n; ++j)
                A[k][j] -= A[i][j] * m;
            b[k] -= b[i] * m;
        }
    }
}

// ----------------------------
// Otimização 4: Loop unrolling na retrossubstituição
// Melhora desempenho em laços longos (especialmente para N = 1000)
// ----------------------------
void retrossubs(double **A, double *b, double *x, int n) {
    for (int i = n - 1; i >= 0; --i) {
        x[i] = b[i];
        int j = i + 1;
        for (; j < n - n % UNROLL_FATOR; j += UNROLL_FATOR) {
            x[i] -= A[i][j] * x[j];
            x[i] -= A[i][j + 1] * x[j + 1];
            x[i] -= A[i][j + 2] * x[j + 2];
            x[i] -= A[i][j + 3] * x[j + 3];
        }
        for (; j < n; j++)
            x[i] -= A[i][j] * x[j];
        x[i] /= A[i][i];
    }
}

// ----------------------------
// Otimização 5: Cálculo iterativo de f(x) na função P()
// Evita chamadas à função pow() em tempo de execução
// ----------------------------
double P(double x_val, int N, double *alpha) {
    double Px = alpha[0];
    double x_power = 1.0;
    for (int i = 1; i <= N; ++i) {
        x_power *= x_val;
        Px += alpha[i] * x_power;
    }
    return Px;
}

int main() {
    LIKWID_MARKER_INIT;

    int N, n;
    long long int K, p;

    if (scanf("%d %lld", &N, &K) != 2) {
        fprintf(stderr, "Erro de leitura\n");
        return 1;
    }

    p = K;
    n = N + 1;

    // Otimização 6: Uso de calloc (inicializa com zero, melhora estabilidade numérica)
    double *x = (double *)calloc(p, sizeof(double));
    double *y = (double *)calloc(p, sizeof(double));
    if (!x || !y) {
        fprintf(stderr, "Erro: memória insuficiente para x/y\n");
        return 1;
    }

    for (long long int i = 0; i < p; ++i) {
        if (scanf("%lf %lf", &x[i], &y[i]) != 2) {
            fprintf(stderr, "Erro de leitura dos pontos\n");
            return 1;
        }
    }

    // Otimização 7: Alocação contígua da matriz A
    // Aumenta a localidade de cache e reduz falhas de TLB
    double **A = (double **)malloc(sizeof(double *) * n);
    double *A_data = (double *)malloc(sizeof(double) * n * n);
    if (!A || !A_data) {
        fprintf(stderr, "Erro: memória insuficiente para matriz A\n");
        return 1;
    }
    for (int i = 0; i < n; ++i)
        A[i] = &(A_data[i * n]);

    double *b = (double *)calloc(n, sizeof(double));
    double *alpha = (double *)calloc(n, sizeof(double));
    if (!b || !alpha) {
        fprintf(stderr, "Erro: memória insuficiente para b/alpha\n");
        return 1;
    }

    // LIKWID para medição de desempenho
    LIKWID_MARKER_START("v2_montaSL");
    double tSL = timestamp();
    montaSL(A, b, n, p, x, y);
    tSL = timestamp() - tSL;
    LIKWID_MARKER_STOP("v2_montaSL");

    LIKWID_MARKER_START("v2_eliminacaoGauss");
    double tEG = timestamp();
    eliminacaoGauss(A, b, n);
    retrossubs(A, b, alpha, n);
    tEG = timestamp() - tEG;
    LIKWID_MARKER_STOP("v2_eliminacaoGauss");

    // Impressão dos coeficientes
    for (int i = 0; i < n; ++i)
        printf("%1.15e ", alpha[i]);
    puts("");

    // Impressão dos resíduos
    for (long long int i = 0; i < p; ++i)
        printf("%1.15e ", fabs(y[i] - P(x[i], N, alpha)));
    puts("");

    // Impressão dos tempos
    printf("%lld %1.10e %1.10e\n", K, tSL, tEG);

    // Liberação da memória alocada
    free(x);
    free(y);
    free(A_data);
    free(A);
    free(b);
    free(alpha);

    LIKWID_MARKER_CLOSE;
    return 0;
}

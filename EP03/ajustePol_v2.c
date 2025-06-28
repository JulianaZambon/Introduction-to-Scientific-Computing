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

#define UNROLL_FATOR 4

void montaSL(double **A, double *b, int n, long long int p, double *x, double *y) {
    int max_power = 2 * n - 1;

    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            A[i][j] = 0.0;

    for (int i = 0; i < n; ++i)
        b[i] = 0.0;

    double *x_pow = (double *)malloc(sizeof(double) * max_power);
    if (x_pow == NULL) {
        fprintf(stderr, "Erro: memória insuficiente para x_pow\n");
        exit(EXIT_FAILURE);
    }

    for (long long int k = 0; k < p; ++k) {
        x_pow[0] = 1.0;
        for (int i = 1; i < max_power; ++i)
            x_pow[i] = x_pow[i - 1] * x[k];

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                A[i][j] += x_pow[i + j];
            }
            b[i] += x_pow[i] * y[k];
        }
    }

    free(x_pow);
}

void eliminacaoGauss(double **A, double *b, int n) {
    for (int i = 0; i < n; ++i) {
        int iMax = i;
        for (int k = i + 1; k < n; ++k) {
            if (fabs(A[k][i]) > fabs(A[iMax][i]))
                iMax = k;
        }

        if (iMax != i) {
            double *tmp_A = A[i];
            A[i] = A[iMax];
            A[iMax] = tmp_A;

            double aux_b = b[i];
            b[i] = b[iMax];
            b[iMax] = aux_b;
        }

        for (int k = i + 1; k < n; ++k) {
            double m = A[k][i] / A[i][i];
            A[k][i] = 0.0;
            for (int j = i + 1; j < n; ++j)
                A[k][j] -= A[i][j] * m;
            b[k] -= b[i] * m;
        }
    }
}

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

    for (int i = 0; i < n; ++i)
        printf("%1.15e ", alpha[i]);
    puts("");

    for (long long int i = 0; i < p; ++i)
        printf("%1.15e ", fabs(y[i] - P(x[i], N, alpha)));
    puts("");

    printf("%lld %1.10e %1.10e\n", K, tSL, tEG);

    free(x);
    free(y);
    free(A_data);
    free(A);
    free(b);
    free(alpha);

    LIKWID_MARKER_CLOSE;
    return 0;
}

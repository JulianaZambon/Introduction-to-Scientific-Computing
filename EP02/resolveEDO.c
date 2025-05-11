#define _POSIX_C_SOURCE 199309L

/*
 * Nome: Juliana Zambon
 * GRR: 20224168
 * Login DINF: jz22
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h> // Para medir o tempo de execução
#include <fenv.h> // Adicionado para controle do arredondamento

#include "utils.h"
#include "edo.h"

// Método de fatoração LU para resolver o sistema linear tridiagonal
void resolveLU(Tridiag *sl, real_t *sol)
{
    int n = sl->n;

    // Fatoração LU
    for (int i = 1; i < n; ++i)
    {
        sl->Di[i] /= sl->D[i - 1];
        sl->D[i] -= sl->Di[i] * sl->Ds[i - 1];
        sl->B[i] -= sl->Di[i] * sl->B[i - 1];
    }

    // Resolução do sistema linear
    sol[n - 1] = sl->B[n - 1] / sl->D[n - 1];
    for (int i = n - 2; i >= 0; --i)
    {
        sol[i] = (sl->B[i] - sl->Ds[i] * sol[i + 1]) / sl->D[i];
    }
}

int main()
{
    // Configurar arredondamento para baixo
    fesetround(FE_DOWNWARD);

    // Estrutura para armazenar os dados da EDO
    EDo edo;

    // 1ª linha: Leitura da quantidade de pontos da malha
    scanf("%d", &edo.n);

    // 2ª linha: Leitura do intervalo [a, b]
    scanf("%lf %lf", &edo.a, &edo.b);

    // 3ª linha: Leitura dos valores de contorno y(a) e y(b)
    scanf("%lf %lf", &edo.ya, &edo.yb);

    // 4ª linha: Leitura dos coeficientes p e q da EDO genérica
    // EDO genérica: y''(x) + p(x)y'(x) + q(x)y(x) = r(x)
    scanf("%lf %lf", &edo.p, &edo.q);

    // 5ª linha em diante: Leitura dos coeficientes r1, r2, r3 e r4
    // da a definição da função r(x), representando diversas EDO's
    char linha[100];
    fgets(linha, sizeof(linha), stdin);
    sscanf(linha, "%lf %lf %lf %lf", &edo.r1, &edo.r2, &edo.r3, &edo.r4);

    // Geração do sistema linear tridiagonal correspondente à EDO
    Tridiag *sl = genTridiag(&edo);

    // Impressão do sistema linear tridiagonal
    prnEDOsl(&edo);

    // Alocação de memória para o vetor solução
    real_t *sol = (real_t *)calloc(edo.n, sizeof(real_t));
    if (sol == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória para o vetor solução.\n");
        exit(EXIT_FAILURE);
    }

    // Medição do tempo de execução usando clock_gettime()
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Resolução do sistema linear tridiagonal
    resolveLU(sl, sol);

    // Medição do tempo de execução
    clock_gettime(CLOCK_MONOTONIC, &end);
    double time_spent = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_nsec - start.tv_nsec) / 1000000.0; // em milissegundos

    // Saída padrão (stdout) para cada EDO:
    // 1. Ordem do sistema linear
    // 2. Matriz aumentada
    // 3. Solução
    // 4. Tempo

    // Impressão da saída
    printf("%d\n", sl->n);

    // Matriz aumentada
    for (int i = 0; i < sl->n; ++i)
    {
        for (int j = 0; j < sl->n; ++j)
        {
            double val = 0.0;
            if (j == i)
                val = sl->D[i];
            else if (j == i - 1)
                val = sl->Ds[i - 1];
            else if (j == i + 1)
                val = sl->Di[i];
            printf(FORMAT, val);
        }
        // Vetor B ao final da linha
        printf(FORMAT, sl->B[i]);
        printf("\n");
    }

    // Solução do sistema linear
    for (int i = 0; i < sl->n; ++i)
    {
        printf(FORMAT, sol[i]);
    }

    // Tempo de solução
    printf("\n");
    printf("%.8e\n", time_spent);

    // Liberação da memória alocada
    free(sol);
    free(sl->D);
    free(sl->Di);
    free(sl->Ds);
    free(sl->B);
    free(sl);

    return 0; // Retorno de sucesso
}
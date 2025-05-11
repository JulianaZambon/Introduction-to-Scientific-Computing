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
    // Estrutura para armazenar os dados da EDO
    EDo edo;

    // Leitura da quantidade de pontos da malha
    scanf("%d", &edo.n);

    // Leitura do intervalo [a, b]
    scanf("%lf %lf", &edo.a, &edo.b);

    // Leitura dos valores de contorno y(a) e y(b)
    scanf("%lf %lf", &edo.ya, &edo.yb);

    // Leitura dos coeficientes p e q da EDO
    scanf("%lf %lf", &edo.p, &edo.q);

    // Leitura dos coeficientes r1, r2, r3, r4 da função r(x)
    scanf("%lf %lf %lf %lf", &edo.r1, &edo.r2, &edo.r3, &edo.r4);

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

    // Medição do tempo de execução
    clock_t start = clock();

    // Resolução do sistema linear tridiagonal
    // Deve ser feito usando o método da Fatoração LU

    resolveLU(sl, sol);

    // Medição do tempo de execução
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC * 1000; // em milissegundos

    // Saída padrão (stdout) para cada EDO:
    // - Os valores numéricos devem ser mostrados com 15 casas decimais (formato "%.15e").
    // - O tempo de solução deve ser exibido com 8 casas decimais (formato "%.8e").

    // A saída deve conter:
    // 1. A ordem do sistema linear tridiagonal (em uma única linha).
    // 2. A matriz aumentada do sistema tridiagonal (coeficientes e termos independentes).
    // 3. A solução do sistema linear, obtida por meio do Método de Fatoração LU.
    // 4. O tempo, em milissegundos, gasto no cálculo da solução.

    // Impressão da saída
    // ordem do sistema linear
    printf("%d\n", sl->n);

    // matriz aumentada
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

            printf("% .15e ", val);
        }

        printf("% .15e\n", sl->B[i]); // vetor B ao final da linha
    }

    // solução do sistema linear
    for (int i = 0; i < sl->n; ++i)
    {
        printf("%.15e%c", sol[i], (i == sl->n - 1) ? '\n' : ' ');
    }

    // tempo de solução
    printf("%.8e\n", time_spent);

    // Liberação da memória alocada
    free(sol);
    free(sl->D);
    free(sl->Di);
    free(sl->Ds);
    free(sl->B);
    free(sl);
}
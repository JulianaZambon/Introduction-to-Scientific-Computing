#define _POSIX_C_SOURCE 199309L
/*
 * Nome: Juliana Zambon
 * GRR: 20224168
 * Login DINF: jz22
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>   // Para medir o tempo de execução
#include <fenv.h>   // Para controle do arredondamento
#include <likwid.h> // Para medir o desempenho

#include "utils.h"
#include "edo.h"

int main()
{
    // Inicializa os marcadores do LIKWID
    LIKWID_MARKER_INIT;

    // Define modo de arredondamento para baixo (FE_DOWNWARD)
    fesetround(FE_DOWNWARD);

    EDo edo;

    // ----------------------------
    // Leitura dos dados de entrada
    // ----------------------------

    // Número de pontos da malha
    scanf("%d", &edo.n);

    // Intervalo [a, b]
    scanf("%lf %lf", &edo.a, &edo.b);

    // Condições de contorno: y(a) e y(b)
    scanf("%lf %lf", &edo.ya, &edo.yb);

    // Coeficientes p e q da EDO
    scanf("%lf %lf", &edo.p, &edo.q);

    // Coeficientes r1, r2, r3, r4 (para o termo independente)
    int status = scanf("%lf %lf %lf %lf", &edo.r1, &edo.r2, &edo.r3, &edo.r4);

    // ---------------------------------
    // Loop para resolver múltiplas EDOs
    // ---------------------------------
    while (status == 4)
    {
        // --------------------------
        // Região marcada: solve_EDO
        // --------------------------
        LIKWID_MARKER_START("solve_EDO");

        // Marca início do tempo
        rtime_t start = timestamp();

        // Gera o sistema linear tridiagonal a partir dos dados da EDO
        Tridiag *sl = genTridiag(&edo);

        // Imprime dados da EDO formatados
        prnEDOsl(&edo);
        printf("\n");

        // Aloca vetor para a solução numérica
        real_t *sol = (real_t *)calloc(edo.n, sizeof(real_t));

        LIKWID_MARKER_STOP("solve_EDO");

        // --------------------------
        // Região marcada: solve_LU
        // --------------------------
        LIKWID_MARKER_START("solve_LU");

        // Resolve o sistema via fatoração LU
        resolveLU(sl, sol);

        LIKWID_MARKER_STOP("solve_LU");

        // Imprime solução numérica
        for (int i = 0; i < edo.n; ++i)
            printf(FORMAT, sol[i]);
        printf("\n");

        // Marca o tempo final e imprime duração
        rtime_t end = timestamp();
        printf("  %.8e\n", end - start);

        // Lê próximo conjunto de coeficientes (se houver)
        status = scanf("%lf %lf %lf %lf", &edo.r1, &edo.r2, &edo.r3, &edo.r4);
        if (status == 4)
            printf("\n");

        // Libera memória alocada
        free(sl->D);
        free(sl->Di);
        free(sl->Ds);
        free(sl->B);
        free(sl);
        free(sol);
    }

    // Encerra os marcadores do LIKWID
    LIKWID_MARKER_CLOSE;

    return 0;
}

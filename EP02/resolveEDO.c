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
#include <fenv.h> // Para controle do arredondamento

#include "utils.h"
#include "edo.h"

#include <likwid.h> // Para medir o desempenho

int main()
{
    // Inicializa o LIKWID
    LIKWID_MARKER_INIT;

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
    scanf("%lf %lf", &edo.p, &edo.q);

    // 5ª linha: Leitura dos coeficientes r1, r2, r3 e r4
    int status;
    status = scanf("%lf %lf %lf %lf", &edo.r1, &edo.r2, &edo.r3, &edo.r4);
    while (status == 4)
    {
        // Métrica do cálculo de cada solução das EDO's 
        /*--------------------------------------------*/
        LIKWID_MARKER_START("solve_EDO");

        rtime_t start = timestamp();

        Tridiag *sl = genTridiag(&edo);

        prnEDOsl(&edo);
        printf("\n");

        real_t *sol = (real_t *)calloc(edo.n, sizeof(real_t));

        LIKWID_MARKER_STOP("solve_EDO");
        /*--------------------------------------------*/


        // Métrica do processo de fatoração LU
        /*--------------------------------------------*/
        LIKWID_MARKER_START("solve_LU");

        resolveLU(sl, sol);

        LIKWID_MARKER_STOP("solve_LU");
        /*--------------------------------------------*/

        for (int i = 0; i < edo.n; ++i)
            printf(FORMAT, sol[i]);
        printf("\n");

        rtime_t end = timestamp();
        printf("  %.8e\n", end - start);

        status = scanf("%lf %lf %lf %lf", &edo.r1, &edo.r2, &edo.r3, &edo.r4);

        if (status == 4)
            printf("\n");

        free(sl->D);
        free(sl->Di);
        free(sl->Ds);
        free(sl->B);
        free(sl);
        free(sol);
    }

    LIKWID_MARKER_CLOSE;

    return 0;
}

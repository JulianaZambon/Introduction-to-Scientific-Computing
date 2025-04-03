#include <stdio.h>
#include <math.h>
#include <float.h>

#include "utils.h"
#include "ZeroFuncao.h"

void (*calcPolinomio)(Polinomio, real_t, real_t *, real_t *);

double ulp_distance(real_t x, real_t y)
{
    // Calcula a distância em ULPs entre dois números de ponto flutuante
    return fabs(x - y) / DBL_EPSILON;
}

/*
 * Critérios de parada:
 * - Criterio-01: |xk−xk−1|⩽10⁻⁶
 * - Criterio-02: |f(xk)|⩽DBL_EPSILON
 * - Criterio-03: ULPs entre xk e xk−1 ⩽ 2
 */


/**
 * Método de Newton-Raphson para encontrar raízes de um polinômio
 * @param p Polinômio a ser resolvido
 * @param x0 Chute inicial
 * @param criterioParada Critério de parada
 * @param it Número de iterações realizadas
 * @param raiz Ponteiro para armazenar a raiz encontrada
 * @return Valor do erro final
 */
real_t newtonRaphson(Polinomio p, real_t x0, int criterioParada, int *it, real_t *raiz)
{
    real_t px, dpx, xk, erro = 0; 
    *it = 0;

    calcPolinomio(p, x0, &px, &dpx);
    if (fabs(dpx) < DBL_EPSILON) 
        return -1; 

    do
    {
        if (fabs(dpx) < DBL_EPSILON)
            return -1; 

        xk = x0 - px / dpx;

        // Se xk explodir, cancela
        if (!isfinite(xk)) 
            return -1;

        calcPolinomio(p, xk, &px, &dpx);

        // Se calcPolinomio retornar valores absurdos, aborta
        if (!isfinite(px) || !isfinite(dpx))
            return -1;

        if (criterioParada == 1)
            erro = fabs(xk - x0);
        else if (criterioParada == 2)
            erro = fabs(px);
        else if (criterioParada == 3)
        {
            erro = ulp_distance(xk, x0);
            if (!isfinite(erro) || erro < 0) // Adiciona proteção extra
                return -1;
        }

        x0 = xk;
        (*it)++;
    } while (erro > 1e-6 && *it < MAXIT);

    *raiz = xk;
    return erro;
}

/**
 * Método da Bissecção para encontrar raízes de um polinômio
 * @param p Polinômio a ser resolvido
 * @param a Limite inferior do intervalo
 * @param b Limite superior do intervalo
 * @param criterioParada Critério de parada
 * @param it Número de iterações realizadas
 * @param raiz Ponteiro para armazenar a raiz encontrada
 * @return Valor do erro final
 */
real_t bisseccao(Polinomio p, real_t a, real_t b, int criterioParada, int *it, real_t *raiz) {
    real_t fa, fb, xm, fm, erro, xm_anterior;
    *it = 0;

    // Calcular os valores do polinômio nos extremos
    calcPolinomio(p, a, &fa, NULL);
    calcPolinomio(p, b, &fb, NULL);
    printf("f(%lf) = %lf, f(%lf) = %lf\n", a, fa, b, fb);
    // Verificar se o intervalo contém uma raiz
    if (fa * fb > 0) {
        printf("Erro: O intervalo [%.6lf, %.6lf] não contém uma raiz.\n", a, b);
        return -1;
    }

    xm_anterior = a; // Inicializa com um dos extremos
    erro = fabs(b - a) / 2.0; // Inicializa o erro para evitar lixo de memória

    do {
        xm = (a + b) / 2.0;
        calcPolinomio(p, xm, &fm, NULL);

        // Escolher o critério de parada
        if (criterioParada == 1) {
            erro = fabs(b - a) / 2.0;
        } else if (criterioParada == 2) {
            erro = fabs(fm);
        } else if (criterioParada == 3) {
            erro = ulp_distance(xm, xm_anterior);
        } else {
            printf("Erro: Critério de parada inválido (%d).\n", criterioParada);
            return -1;
        }

        // Atualizar intervalo
        if (fa * fm < 0) {
            b = xm;
            fb = fm;
        } else {
            a = xm;
            fa = fm;
        }

        xm_anterior = xm;
        (*it)++;

    } while (erro > 1e-6 && *it < MAXIT);

    *raiz = xm;
    return erro;
}

/**
 * Implementação do método de Horner para calcular um polinômio e sua derivada.
 */
void calcPolinomio_rapido(Polinomio p, real_t x, real_t *px, real_t *dpx)
{
    int grau = p.grau;
    real_t *coef = p.p;

    *px = coef[grau]; // Começa com o termo de maior grau
    real_t derivada = coef[grau] * grau; // Primeiro coeficiente derivado

    printf("\nCalculando P(x) e P'(x) usando Horner:\n");

    for (int i = grau - 1; i >= 0; i--)
    {
        derivada = derivada * x + coef[i] * i;
        *px = *px * x + coef[i];

        printf("Iteração %d: P(x) = %.6lf, P'(x) = %.6lf\n", i, *px, derivada);
    }

    if (dpx)
        *dpx = derivada;
}

/**
 * Implementação direta usando pow().
 */
void calcPolinomio_lento(Polinomio p, real_t x, real_t *px, real_t *dpx)
{
    int grau = p.grau;
    real_t *coef = p.p;

    *px = 0.0;
    real_t derivada = 0.0;

    for (int i = 0; i <= grau; i++)  // Acessa os coeficientes corretamente
    {
        *px += coef[grau - i] * pow(x, i); // Ajuste na indexação
        if (dpx && i > 0)
            derivada += coef[grau - i] * i * pow(x, i - 1); // Ajuste na indexação
    }

    if (dpx)
        *dpx = derivada;
}





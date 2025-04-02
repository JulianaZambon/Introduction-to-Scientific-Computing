#include <stdio.h>
#include <math.h>
#include <float.h>

#include "utils.h"
#include "ZeroFuncao.h"


double ulp_distance(real_t x, real_t y) {
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
    real_t px, dpx, xk, erro;
    *it = 0;

    calcPolinomio_rapido(p, x0, &px, &dpx);
    if (dpx == 0.0)
        return NAN; // Derivada nula, não pode continuar

    do
    {
        xk = x0 - px / dpx;
        calcPolinomio_rapido(p, xk, &px, &dpx);

        if (criterioParada == 1)
            erro = fabs(xk - x0);
        else if (criterioParada == 2)
            erro = fabs(px);
        else if (criterioParada == 3)
            erro = ulp_distance(xk, x0); // Distância real em ULPs

        x0 = xk;
        (*it)++;
    } while (erro > 1e-6 && *it < MAXIT); // Critério 1 como referência

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
real_t bisseccao(Polinomio p, real_t a, real_t b, int criterioParada, int *it, real_t *raiz)
{
    real_t fa, fb, c, fc;
    calcPolinomio_rapido(p, a, &fa, NULL);
    calcPolinomio_rapido(p, b, &fb, NULL);

    if (fa * fb > 0)
        return -1.0; // Não há raiz no intervalo

    *it = 0;
    while (*it < MAXIT)
    {
        c = (a + b) / 2.0;
        calcPolinomio_rapido(p, c, &fc, NULL);

        if (fc == 0.0)
        {
            *raiz = c;
            return 0.0;
        }

        if (fa * fc < 0)
            b = c;
        else
            a = c;

        if (criterioParada == 1 && fabs(b - a) <= 1e-6)
            break;
        if (criterioParada == 2 && fabs(fc) <= DBL_EPSILON)
            break;
        if (criterioParada == 3 && ulp_distance(b, a) <= 2)
            break;

        (*it)++;
    }

    *raiz = c;
    return fabs(fc);
}


/**
 * Implementação do método de Horner para calcular um polinômio e sua derivada.
 */
void calcPolinomio_rapido(Polinomio p, real_t x, real_t *px, real_t *dpx) {
    *px = p.p[p.grau];

    if (p.grau > 0) {
        *dpx = p.p[p.grau] * p.grau; // Inicializa derivada corretamente
    } else {
        *dpx = 0.0; // Se grau for zero, a derivada é zero
    }

    for (int i = p.grau - 1; i >= 0; --i) {
        *px = (*px * x) + p.p[i];
        if (i > 0)
            *dpx = (*dpx * x) + p.p[i] * i;
    }
}

/**
 * Implementação direta usando pow().
 */
void calcPolinomio_lento(Polinomio p, real_t x, real_t *px, real_t *dpx) {
    *px = 0.0;
    *dpx = 0.0;

    for (int i = 0; i <= p.grau; ++i) {
        *px += p.p[i] * pow(x, i);
        if (i > 0)
            *dpx += i * p.p[i] * pow(x, i - 1);
    }
}


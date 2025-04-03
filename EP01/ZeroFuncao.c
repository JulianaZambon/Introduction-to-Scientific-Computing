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
    real_t px, dpx, xk, erro;
    *it = 0;

    calcPolinomio(p, x0, &px, &dpx); // Agora usa a função global
    if (dpx == 0.0)
        return -1; // Derivada nula, não pode continuar

    do
    {
        xk = x0 - px / dpx;
        calcPolinomio(p, xk, &px, &dpx); // Aqui também

        if (criterioParada == 1)
            erro = fabs(xk - x0);
        else if (criterioParada == 2)
            erro = fabs(px);
        else if (criterioParada == 3)
            erro = ulp_distance(xk, x0);

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
real_t bisseccao(Polinomio p, real_t a, real_t b, int criterioParada, int *it, real_t *raiz)
{
    real_t px_a, px_b, xk, erro;
    *it = 0;

    calcPolinomio(p, a, &px_a, NULL);
    calcPolinomio(p, b, &px_b, NULL);

    if (px_a * px_b > 0.0)
        return NAN;

    do
    {
        xk = (a + b) / 2.0;
        real_t px_k;
        calcPolinomio(p, xk, &px_k, NULL);

        if (px_k == 0.0)
            break;

        if (px_a * px_k < 0.0)
            b = xk;
        else
            a = xk;

        if (criterioParada == 1)
            erro = fabs(b - a);
        else if (criterioParada == 2)
            erro = fabs(px_k);
        else if (criterioParada == 3)
            erro = ulp_distance(xk, a);

        (*it)++;
    } while (erro > 1e-6 && *it < MAXIT);

    *raiz = xk;
    return erro;
}


/**
 * Implementação do método de Horner para calcular um polinômio e sua derivada.
 */
void calcPolinomio_rapido(Polinomio p, real_t x, real_t *px, real_t *dpx)
{
    int grau = p.grau;
    real_t *coef = p.p;

    // Inicializa o polinômio e sua derivada
    *px = coef[0];
    real_t derivada = 0.0;

    for (int i = 1; i <= grau; i++)
    {
        derivada = derivada * x + *px; // Calcula derivada usando Horner
        *px = *px * x + coef[i];       // Calcula p(x) usando Horner
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





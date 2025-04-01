#include <stdio.h>
#include <math.h>

#include "utils.h"
#include "ZeroFuncao.h"

/*
 * Critérios de parada:
 * - Criterio-01: |xk−xk−1|⩽10−6
 * - Criterio-02: |f(xk)|⩽DBL_EPSILON
 * - Criterio-03: ULP's entre xk e xk−1⩽2
 *
 */

/**
 * Método de Newton-Raphson para encontrar raízes de um polinômio.
 *
 * @param p Polinômio a ser resolvido
 * @param x0 Chute inicial
 * @param criterioParada Critério de parada
 * @param it Número de iterações realizadas
 * @param raiz Ponteiro para armazenar a raiz encontrada
 * @return Valor do erro final
 */
real_t newtonRaphson(Polinomio p, real_t x0, int criterioParada, int *it, real_t *raiz)
{
}

/**
 * Método da Bissecção para encontrar raízes de um polinômio.
 *
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

}

/* Cálculo do valor px de um polinômio p */
void calcPolinomio_rapido(Polinomio p, real_t x, real_t *px, real_t *dpx)
{

}

/* Implementação com o cálculo direto do polinômio e sua derivada, usando obrigatoriamente a função pow */
void calcPolinomio_lento(Polinomio p, real_t x, real_t *px, real_t *dpx)
{

}

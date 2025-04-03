#include <stdio.h>
#include <math.h>
#include <float.h>

#include "utils.h"
#include "ZeroFuncao.h"

// Ponteiro de função que pode armazenar diferentes implementações
// para o cálculo de polinômios (ex: rápido usando Horner ou lento usando pow)
void (*calcPolinomio)(Polinomio, real_t, real_t *, real_t *);

/**
 * Calcula a distância em ULPs (Unidades de Menor Precisão) entre dois números de ponto flutuante.
 * ULPs são úteis para avaliar precisão numérica em comparações.
 */
double ulp_distance(real_t x, real_t y)
{
    return fabs(x - y) / DBL_EPSILON;
}

/*
 * Critérios de parada rigorosos para os métodos numéricos:
 * - Critério 1 (Erro Absoluto): |xk - xk-1| ≤ 10⁻⁶
 * - Critério 2 (Erro da Função): |f(xk)| ≤ sqrt(DBL_EPSILON) → tolerância da máquina
 * - Critério 3 (Erro Relativo): ULPs entre xk e xk-1 ≤ 2
 */

/**
 * Método de Newton-Raphson para encontrar raízes de um polinômio.
 * Utiliza a fórmula de iteração:
 *     x_k+1 = x_k - f(x_k) / f'(x_k)
 * 
 * @param p Polinômio a ser resolvido
 * @param x0 Chute inicial para a raiz
 * @param criterioParada Critério de parada (1, 2 ou 3)
 * @param it Ponteiro para armazenar o número de iterações realizadas
 * @param raiz Ponteiro para armazenar a raiz encontrada
 * @return Valor do erro final
 */
real_t newtonRaphson(Polinomio p, real_t x0, int criterioParada, int *it, real_t *raiz)
{
    real_t fx, dfx, x1;
    *it = 0;

    // Calcula f(x0) e f'(x0)
    calcPolinomio(p, x0, &fx, &dfx);

    // Se a derivada for zero, o método falha (evita divisão por zero)
    if (dfx == 0)
        return NAN;

    do
    {
        // Iteração de Newton-Raphson
        x1 = x0 - fx / dfx;
        (*it)++;

        // Recalcula f(x) e f'(x) para o novo ponto x1
        calcPolinomio(p, x1, &fx, &dfx);

        // Verifica se algum critério de parada foi atingido
        if (criterioParada == 1 && fabs(x1 - x0) <= 1e-6)
            break;
        if (criterioParada == 2 && fabs(fx) <= sqrt(DBL_EPSILON))
            break;
        if (criterioParada == 3 && ulp_distance(x1, x0) <= 2)
            break;

        x0 = x1; // Atualiza o valor de x0 para a próxima iteração

    } while (*it < MAXIT); // Limita o número de iterações para evitar loops infinitos

    *raiz = x1; // Armazena a raiz encontrada
    return fabs(fx); // Retorna o erro final
}

/**
 * Método da Bissecção para encontrar raízes de um polinômio.
 * O método reduz iterativamente o intervalo [a, b] até encontrar uma raiz.
 * 
 * @param p Polinômio a ser resolvido
 * @param a Limite inferior do intervalo
 * @param b Limite superior do intervalo
 * @param criterioParada Critério de parada (1, 2 ou 3)
 * @param it Ponteiro para armazenar o número de iterações realizadas
 * @param raiz Ponteiro para armazenar a raiz encontrada
 * @return Valor do erro final
 */
real_t bisseccao(Polinomio p, real_t a, real_t b, int criterioParada, int *it, real_t *raiz)
{
    real_t fa, fb, c, fc;
    *it = 0;

    // Calcula f(a) e f(b)
    calcPolinomio(p, a, &fa, NULL);
    calcPolinomio(p, b, &fb, NULL);

    // Se f(a) * f(b) > 0, o teorema do valor intermediário não garante raiz
    if (fa * fb > 0)
        return NAN;

    do
    {
        // Calcula o ponto médio do intervalo
        c = (a + b) / 2.0;
        calcPolinomio(p, c, &fc, NULL);
        (*it)++;

        // Se f(c) = 0, encontramos a raiz exata
        if (fc == 0)
            break;

        // Decide em qual subintervalo continuar a busca
        if (fa * fc < 0)
            b = c;
        else
            a = c;

        // Verifica se algum critério de parada foi atingido
        if (criterioParada == 1 && fabs(b - a) <= 1e-6)
            break;
        if (criterioParada == 2 && fabs(fc) <= DBL_EPSILON)
            break;
        if (criterioParada == 3 && ulp_distance(c, (a + b) / 2) <= 2)
            break;

    } while (*it < MAXIT); // Limita o número de iterações

    *raiz = c; // Armazena a raiz encontrada
    return fabs(fc); // Retorna o erro final
}

/**
 * Cálculo eficiente de polinômios usando o método de Horner.
 * O método reduz o número de multiplicações necessárias.
 * 
 * @param p Polinômio a ser avaliado
 * @param x Valor no qual o polinômio deve ser calculado
 * @param px Ponteiro para armazenar f(x)
 * @param dpx Ponteiro para armazenar f'(x) (se necessário)
 */
void calcPolinomio_rapido(Polinomio p, real_t x, real_t *px, real_t *dpx)
{
    int grau = p.grau;
    real_t *coef = p.p;

    if (!px)
        return;

    // Inicializa px com o coeficiente de maior grau
    *px = coef[grau];
    real_t derivada = (grau > 0) ? coef[grau] * grau : 0.0;

    // Aplica a Regra de Horner para calcular f(x) e f'(x)
    for (int i = grau - 1; i >= 0; i--)
    {
        *px = *px * x + coef[i];

        if (i > 0) // Evita multiplicar por i == 0
            derivada = derivada * x + coef[i] * i;
    }

    if (dpx)
        *dpx = derivada;
}

/**
 * Cálculo de polinômios usando uma abordagem direta com pow().
 * Essa abordagem é menos eficiente que Horner devido ao alto custo computacional de pow().
 * 
 * @param p Polinômio a ser avaliado
 * @param x Valor no qual o polinômio deve ser calculado
 * @param px Ponteiro para armazenar f(x)
 * @param dpx Ponteiro para armazenar f'(x) (se necessário)
 */
void calcPolinomio_lento(Polinomio p, real_t x, real_t *px, real_t *dpx)
{
    if (!px)
        return;

    int grau = p.grau;
    real_t *coef = p.p;

    *px = 0.0;
    real_t derivada = 0.0;

    // Avalia o polinômio e sua derivada usando pow()
    for (int i = 0; i <= grau; i++)
    {
        *px += coef[i] * pow(x, i);

        if (i > 0)
            derivada += coef[i] * i * pow(x, i - 1);
    }

    if (dpx)
        *dpx = derivada;
}

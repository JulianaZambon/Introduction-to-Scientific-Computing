#include <stdio.h>
#include <math.h>
#include <float.h>
#include <fenv.h>

#include "utils.h"
#include "ZeroFuncao.h"

/**
 * Executa os métodos de Bissecção e Newton-Raphson para encontrar raízes de um polinômio.
 * Os métodos são executados com diferentes critérios de parada e utilizando tanto
 * a implementação rápida (Horner) quanto a implementação lenta (uso de pow).
 *
 * @param pol Polinômio a ser resolvido.
 * @param a Limite inferior do intervalo de busca da raiz.
 * @param b Limite superior do intervalo de busca da raiz.
 */
void executarMetodos(Polinomio pol, real_t a, real_t b)
{
    int it; // Variável para armazenar o número de iterações
    real_t raiz, erro; // Variáveis para armazenar a raiz encontrada e o erro
    double tempo_inicio, tempo_fim, tempo_execucao; // Variáveis para medir o tempo de execução

    // Execução do método da Bissecção com os 3 critérios de parada
    for (int i = 1; i <= 3; ++i)
    {
        tempo_inicio = timestamp();  // Marca o tempo antes da execução
        erro = bisseccao(pol, a, b, i, &it, &raiz);
        tempo_fim = timestamp();     // Marca o tempo depois da execução

        tempo_execucao = (tempo_fim - tempo_inicio) * 1000; // Convertendo para milissegundos

        printf("bissec %.15e %.15e %d %.8e\n", raiz, erro, it, tempo_execucao);
    }

    // Execução do método de Newton-Raphson com os 3 critérios de parada
    for (int i = 1; i <= 3; ++i)
    {
        tempo_inicio = timestamp();
        erro = newtonRaphson(pol, a, i, &it, &raiz);
        tempo_fim = timestamp();

        tempo_execucao = (tempo_fim - tempo_inicio) * 1000; // Convertendo para milissegundos

        printf("newton %.15e %.15e %d %.8e\n", raiz, erro, it, tempo_execucao);
    }
}

int main()
{
    // Define o modo de arredondamento para baixo para operações de ponto flutuante
    fesetround(FE_DOWNWARD);

    real_t a, b; // Intervalo onde está uma das raízes
    Polinomio pol; // Estrutura do polinômio

    // Leitura do polinômio
    scanf("%d", &pol.grau); // Lê o grau do polinômio
    pol.p = malloc((pol.grau + 1) * sizeof(real_t)); // Aloca memória para os coeficientes

    if (!pol.p)
        return 1; // Retorna erro caso a alocação falhe

    // Lê os coeficientes do polinômio do maior para o menor grau
    for (int i = pol.grau; i >= 0; --i)
        scanf("%lf", &pol.p[i]);

    // Lê os valores do intervalo [a, b] onde está a raiz
    scanf("%lf %lf", &a, &b);

    // Execução dos métodos usando a versão rápida (Horner)
    printf("RAPIDO:\n\n");
    calcPolinomio = calcPolinomio_rapido;
    executarMetodos(pol, a, b);
    printf("\n");

    // Execução dos métodos usando a versão lenta (uso de pow)
    printf("LENTO:\n\n");
    calcPolinomio = calcPolinomio_lento;
    executarMetodos(pol, a, b);

    // Libera a memória alocada para os coeficientes do polinômio
    free(pol.p);
    pol.p = NULL; // Evita ponteiro solto

    return 0;
}

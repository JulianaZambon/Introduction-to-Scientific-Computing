#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "utils.h"

#define DIFF 0.0

#define NRAND    ((double) rand() / RAND_MAX)  // drand48() 
#define SRAND(a) srand(a) // srand48(a)

// Integral Monte Carlo da função Styblinski-Tang de 2 variáveis
double styblinskiTang(double a, double b, int namostras)
{
  double resultado;
  double soma = 0.0;
  
  printf("Metodo de Monte Carlo (x, y).\n");
  printf("a = (%f), b = (%f), n = (%d), variaveis = 2\n", a, b, namostras);
  
  double t_inicial = timestamp();
  
  /*
    
    AQUI IMPLEMENTE O CÁLCULO DA INTEGRAL  PELO
    MÉTODO DE MONTE CARLO
    
  */
  
  double t_final = timestamp();
  printf("Tempo decorrido: %f seg.\n", t_final - t_inicial);
  
  return resultado;
}


double retangulos_xy(double a, double b, int npontos) {

  double h;
  double resultado;
  double soma = 0;
  
  printf("Metodo dos Retangulos (x, y).\n");
  printf("a = (%f), b = (%f), n = (%d), h = (%lg)\n", a, b, npontos, h);
  
  double t_inicial = timestamp();
  
  /*
    
    AQUI IMPLEMENTE O CÁLCULO DA INTEGRAL PELO
    MÉTODO DOS RETÂNGULOS
    
  */
  
  double t_final = timestamp();
  printf("Tempo decorrido: %f seg.\n", t_final - t_inicial);
  
  return resultado;
}


int main(int argc, char **argv) {

  if (argc < 5) {
    printf("Utilização: %s inicial final n_amostras n_variaveis\n", argv[0]);
    return 1;
  }

  // INICIAR VALOR DA SEMENTE
    
  // CHAMAR FUNÇÕES DE INTEGRAÇÃO E EXIBIR RESULTADOS
  
  return 0;
}


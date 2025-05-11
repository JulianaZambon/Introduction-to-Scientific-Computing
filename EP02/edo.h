/*
 * Nome: Juliana Zambon
 * GRR: 20224168
 * Login DINF: jz22
 */

#ifndef __EQDIFF_H__
#define __EQDIFF_H__

typedef double real_t;

// Macro para definir o número de casas decimais na impressão
// Os valores de saída devem ser mostrados com 15  casas decimais (formato "%.15e")
#define FORMAT "%23.15e"

// Sistema linear Tri-diagonal
typedef struct
{
  real_t *D, *Di, *Ds, *B;
  int n;
} Tridiag;

// Equação Diferencial Ordinária
typedef struct
{
  int n;                       // número de pontos internos na malha
  real_t a, b;                 // intervalo
  real_t ya, yb;               // condições contorno
  real_t p, q, r1, r2, r3, r4; // coeficientes EDO genérica
} EDo;

// Funções auxiliares

Tridiag *genTridiag(EDo *edoeq);
void prnEDOsl(EDo *edoeq);

#endif // __EQDIFF_H__

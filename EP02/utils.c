#define _POSIX_C_SOURCE 199309L

/*
 * Nome: Juliana Zambon
 * GRR: 20224168
 * Login DINF: jz22
 */

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "utils.h"

/*  Retorna tempo em milisegundos desde EPOCH

    Forma de uso:

    rtime_t tempo;
    tempo = timestamp();
    <trecho de programa do qual se deseja medir tempo>
    tempo = timestamp() - tempo;
*/

rtime_t timestamp(void)
{
  struct timespec tp;
  clock_gettime(CLOCK_MONOTONIC_RAW, &tp);
  return ((rtime_t)tp.tv_sec * 1.0e3 + (rtime_t)tp.tv_nsec * 1.0e-6);
}

/* Gera string '<baseName>_n'
 * Por exemplo, se baseName = "ABC" e n = 10,
 * Função retorna a string "ABC_10"
 * Útil para gerar marcadores para LIKWID
 */
string_t markerName(string_t baseName, int n)
{
  string_t mark = (string_t)malloc((strlen(baseName) + 1) + numDigits(n) + 1);

  sprintf(mark, "%s_%u", baseName, n);

  // printf("*** %s\n", mark);

  return mark;
}

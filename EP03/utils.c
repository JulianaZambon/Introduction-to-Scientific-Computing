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

rtime_t timestamp (void)
{
  struct timespec tp;
  clock_gettime(CLOCK_MONOTONIC_RAW, &tp);
  return ( (rtime_t) tp.tv_sec*1.0e3 + (rtime_t) tp.tv_nsec*1.0e-6 );
}

/* Gera string '<baseName>_n'
 * Por exemplo, se baseName = "ABC" e n = 10,
 *  Função retorna a string "ABC_10"
 * Útil para gerar marcadores para LIKWID
 */
string_t markerName(string_t baseName, int n)
{
    string_t mark = (string_t) malloc( (strlen(baseName)+1) + numDigits(n) + 1 );

  sprintf(mark, "%s_%u", baseName,n);

  // printf("*** %s\n", mark);

  return mark;

}

/*
// Retorna TRUE se 'n' é uma potência de 2
// [OBSOLETO] 
// Definida macro em 'utils.h' para substituir esta função
//
int isPot2(int n)
{
  int k;
  return (k = log2(n)) == log2(n) ;
}
*/


/*  Retorna tempo em milisegundos desde EPOCH

    Forma de uso:
 
    rtime_t tempo;
    tempo = timestamp();
    <trecho de programa do qual se deseja medir tempo>
    tempo = timestamp() - tempo;
*/

/*  
rtime_t timestamp(void)
{
  struct timeval tp;
  gettimeofday(&tp, NULL);
  return ( (rtime_t) tp.tv_sec*1.0e3 + (rtime_t) tp.tv_usec*1.0e-3 );
}

*/

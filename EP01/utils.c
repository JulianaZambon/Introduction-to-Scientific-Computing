#define _POSIX_C_SOURCE 199309L  

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>  

#include "utils.h"

/*  Retorna tempo em milisegundos

    Forma de uso:
 
    double tempo;
    tempo = timestamp();
    <trecho de programa do qual se deseja medir tempo>
    tempo = timestamp() - tempo;
*/

double timestamp(void)
{
  struct timespec tp;
  clock_gettime(CLOCK_MONOTONIC, &tp);
  return((double)(tp.tv_sec*1.0e3 + tp.tv_nsec*1.0e-6));
}

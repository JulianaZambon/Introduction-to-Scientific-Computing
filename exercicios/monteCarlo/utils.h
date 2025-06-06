#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

// real_t: tipo usado para representar valores em ponto flutuante
typedef double real_t;

// string_t: tipo usado para representar ponteiros para char/strings
typedef char * string_t;

// rtime_t: tipo usado para representar valores de tempo em ponto flutuante
typedef double rtime_t;

// Valor absoluto de um número. Alternativa ao uso da função 'fabs()'
#define ABS(num)  ((num) < 0.0 ? -(num) : (num))

// Número máximo de dígitos decimais em um número
#define numDigits(n)  ( (int) log10(n) + 1 )

// Macro para verificar se valor 'n' é potência de 2 ou não
#define isPot2(n) (n && !(n & (n - 1)))

// SIMD alignment macros
#define ALIGN_64 __attribute__((aligned(64)))
#define ALIGN_32 __attribute__((aligned(32)))
#define ALIGN_16 __attribute__((aligned(16)))

// Funções
double timestamp(void);
string_t markerName(string_t baseName, int n);

#endif // __UTILS_H__


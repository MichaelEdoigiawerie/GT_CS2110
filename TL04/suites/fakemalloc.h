#include <stdlib.h>
#include <stdio.h>

#define malloc(n)    ((!malloc_use_pattern || ((long unsigned int)malloc_pattern_ind < (sizeof(malloc_pattern) / sizeof(int)) && malloc_pattern[malloc_pattern_ind++])) ? malloc(n)    : NULL)
#define calloc(n,s)  ((!malloc_use_pattern || ((long unsigned int)malloc_pattern_ind < (sizeof(malloc_pattern) / sizeof(int)) && malloc_pattern[malloc_pattern_ind++])) ? calloc(n,s)  : NULL)
#define realloc(n,s) ((!malloc_use_pattern || ((long unsigned int)malloc_pattern_ind < (sizeof(malloc_pattern) / sizeof(int)) && malloc_pattern[malloc_pattern_ind++])) ? realloc(n,s) : NULL)

extern int malloc_use_pattern;
extern int malloc_pattern_ind;
extern int malloc_pattern[10];

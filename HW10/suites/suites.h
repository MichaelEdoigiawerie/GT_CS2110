#ifndef SUITES_H
#define SUITES_H

#include <check.h>
#include <stdbool.h>
#include "../my_malloc.h"

#define HEAP_SIZE 0x2000

Suite *malloc_suite(void);
Suite *free_suite(void);
Suite *calloc_suite(void);
Suite *realloc_suite(void);
Suite *helpers_suite(void);

/* Used to access the address and size freelists */
extern metadata_t *address_list;
// extern metadata_t *size_list;

/*These variables are explained in malloc_suite*/
extern uint8_t *ret;
extern metadata_t *A, *B, *C;
extern bool my_sbrk_called;
extern bool my_sbrk_call_expected;
extern uint8_t *my_sbrk_fake_heap;
extern int my_sbrk_imaginary_prev_calls;
//extern int my_sbrk_calls;

#define A_START_OFF 512
#define A_START_SIZE 256
#define B_START_OFF 928
#define B_START_SIZE 64
#define C_START_OFF 1216
#define C_START_SIZE (SBRK_SIZE - C_START_OFF - TOTAL_METADATA_SIZE)
#define D_START_OFF 32
#define D_START_SIZE 128
#define E_START_SIZE 64
#define E_START_OFF (SBRK_SIZE - E_START_SIZE - TOTAL_METADATA_SIZE)

#define F_START_OFF 128
#define F_START_SIZE 64
#define G_START_OFF F_START_OFF + TOTAL_METADATA_SIZE + F_START_SIZE
#define G_START_SIZE 64
#define H_START_OFF G_START_OFF + TOTAL_METADATA_SIZE + G_START_SIZE
#define H_START_SIZE 64
#define I_START_OFF H_START_OFF + TOTAL_METADATA_SIZE + H_START_SIZE
#define I_START_SIZE (SBRK_SIZE - F_START_SIZE - G_START_SIZE - H_START_SIZE - (TOTAL_METADATA_SIZE*3))

// metadata_t *F,*G,*H,*I;



/*These functions are defined/explained in malloc_suite*/
void setup_malloc_malloc(void);
void teardown_malloc_malloc(void);
void *my_sbrk(int increment);
void init_malloc_test(int prev_sbrk_calls, bool should_sbrk);
void create_situation_1(metadata_t **Aout, metadata_t **Bout, metadata_t **Cout);
void setup_malloc_malloc(void);
void teardown_malloc_malloc(void);

/*This is the function to print the freelist*/
void call_freelist(void);
#endif
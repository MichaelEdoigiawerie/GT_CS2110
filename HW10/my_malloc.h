#ifndef __MY_MALLOC_H__
#define __MY_MALLOC_H__

#include <stddef.h>
#include <stdint.h>

#define SBRK_SIZE 2048
#define TOTAL_METADATA_SIZE (sizeof(metadata_t))
#define MIN_BLOCK_SIZE (TOTAL_METADATA_SIZE + 1)

#define UNUSED_PARAMETER(param) (void)(param)

typedef struct metadata {
    struct metadata *next;
    unsigned long size; // size of the user usable part of the block in number of bytes
} metadata_t;

enum my_malloc_err {
    NO_ERROR,
    OUT_OF_MEMORY,
    SINGLE_REQUEST_TOO_LARGE,
};

extern enum my_malloc_err my_malloc_errno;

metadata_t *find_right(metadata_t *freed_block);

metadata_t *find_left(metadata_t *freed_block);

void merge(metadata_t *left, metadata_t *right);

metadata_t *split_block(metadata_t *block, size_t size);

void add_to_addr_list(metadata_t *block);

void remove_from_addr_list(metadata_t *block);

metadata_t *find_best_fit(size_t size);

void *my_sbrk(int);

void *my_malloc(size_t size);

void my_free(void *ptr);

void *my_realloc(void *ptr, size_t size);

void *my_calloc(size_t nmemb, size_t size);

uintptr_t pp_pointer(metadata_t* ptr);
#endif /* __MY_MALLOC_H__ */

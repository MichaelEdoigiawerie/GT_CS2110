// Homework 11 Tests, Fall 2019
// Written by Sean Crowley, copied most of this from Maddie Brickell, who copied most of this from Austin Adams
//
// Lies ^^^^ Ya boi Sean has been doing OS and Josh and Farzam have been killing it!
//
// Warning: much the structure of this file is shamelessly copypasted from
// https://libcheck.github.io/check/doc/check_html/check_3.html

#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "suites.h"

#define SUBPTR(a) ((uint8_t*)((((uint8_t*)(a)) - ((uint8_t*)(my_sbrk_fake_heap))) + 1))

#define ck_ptr(A, B) do { \
    uint8_t* _a = SUBPTR((A)); \
    uint8_t* _b = SUBPTR((B)); \
    ck_assert_msg(_a == _b, "Assertion '%s' failed: %s == %p, %s == %p", #A" == "#B, #A, _a, #B, _b); \
} while(0)

bool my_sbrk_called;
bool my_sbrk_call_expected;
uint8_t *my_sbrk_fake_heap;         // Pointer to the beginning of the fake heap -- initiallized in my_sbrk
int my_sbrk_imaginary_prev_calls;   // Determines the position of the break at the beginning of the tests
int my_sbrk_calls;

                            /********** HELPER FUNCTIONS **********/

// Fake my_sbrk()
void *my_sbrk(int increment) {
    ck_assert_int_eq(increment, SBRK_SIZE);
    if (my_sbrk_call_expected) {
        ck_assert_msg(my_sbrk_call_expected, "There was not a block available for this call to my_malloc so you were supposed to call my_sbrk, but did not");
    } else {
        ck_assert_msg(my_sbrk_call_expected, "There was a block available for this call to my_malloc, but you still made a call to my_sbrk when you did not have to");
    }
    ck_assert_msg(!my_sbrk_called, "You should be only calling my_sbrk once in an invocation of my_malloc. Check your my_malloc code, it looks like you are calling my_sbrk more than once.");
    my_sbrk_called = true;

    // Handle OOM
    if (my_sbrk_imaginary_prev_calls == HEAP_SIZE/SBRK_SIZE) {
        return (void *) -1;
    } else {
        my_sbrk_calls = my_sbrk_calls + 1;
        return my_sbrk_fake_heap + my_sbrk_imaginary_prev_calls * SBRK_SIZE;
    }
}

// Create situation
void create_situation_1(metadata_t **Aout, metadata_t **Bout, metadata_t **Cout) {
    // Setup C
    metadata_t *C = (metadata_t *)(my_sbrk_fake_heap + C_START_OFF);
    C->size = C_START_SIZE;


    // Setup A
    metadata_t *A = (metadata_t *)(my_sbrk_fake_heap + A_START_OFF);
    A->size = A_START_SIZE;


    // Setup B
    metadata_t *B = (metadata_t *)(my_sbrk_fake_heap + B_START_OFF);
    B->size = B_START_SIZE;

    // Setup linked_lists by address
    address_list = A;
    A -> next = B;
    B -> next = C;
    C -> next = NULL;

    // Return all
    if (Aout) *Aout = A;
    if (Cout) *Cout = C;
    if (Bout) *Bout = B;
}


// Called by many tests to set up freelist and fake heap //
void init_malloc_test(int prev_sbrk_calls, bool should_sbrk) {
    my_sbrk_imaginary_prev_calls = prev_sbrk_calls;
    my_sbrk_call_expected = should_sbrk;
}


// *************************************************************************************** //
// ************************ test case setup/teardown functions *************************** //
// *************************************************************************************** //

void setup_malloc_malloc(void) {
    address_list = NULL;
    my_sbrk_called = false;
    my_sbrk_fake_heap = calloc(1, HEAP_SIZE);   // Allocates two pages each of 4096 bytes for the heap
    memset(my_sbrk_fake_heap, 0x69, HEAP_SIZE);
    ck_assert(my_sbrk_fake_heap);               // Out of memory. Shouldn't happen but check anyway
}

void teardown_malloc_malloc(void) {
    free(my_sbrk_fake_heap);
    my_sbrk_fake_heap = NULL;
}


// *************************************************************************************** //
// ************************ setup functions for specific tests *************************** //
// *************************************************************************************** //

/* MALLOC_INIT */
uint8_t *ret;
void malloc1_setup(void) {
    printf("HIT malloc1_setup\n");
    init_malloc_test(0, true);
    // Set errno to something else to check if student is setting it
    my_malloc_errno = OUT_OF_MEMORY;
    ret = my_malloc(128);
}

/*PERFECT BLOCK 1*/
uint8_t* ret1;
metadata_t *A, *B, *C;
void malloc_perf_block1_setup(void) {
    printf("HIT malloc_perf_block1_setup\n");
    init_malloc_test(1, false);
    //change our errno to make sure it gets set
    my_malloc_errno = OUT_OF_MEMORY;
    create_situation_1(&A, &B, &C);
    ret1 = my_malloc(C->size);
}

/*PERFECT BLOCK 2*/
void malloc_perf_block2_setup(void) {
    printf("HIT malloc_perf_block2_setup\n");
    init_malloc_test(1, false);
    //change our errno to make sure it gets set
    my_malloc_errno = OUT_OF_MEMORY;
    create_situation_1(&A, &B, &C);
    ret1 = my_malloc(B->size);
}

/*PERFECT BLOCK 3*/
void malloc_perf_block3_setup(void) {
    printf("HIT malloc_perf_block3_setup\n");
    init_malloc_test(1, false);
    //change our errno to make sure it gets set
    my_malloc_errno = OUT_OF_MEMORY;
    create_situation_1(&A, &B, &C);
    ret1 = my_malloc(A->size);
}

/*SPLIT BLOCK 1*/
void malloc_split_block1_setup(void) {
    printf("HIT malloc_split_block1_setup\n");
    init_malloc_test(1, false);

    create_situation_1(&A, &B, &C);

    my_malloc_errno = OUT_OF_MEMORY;
    ret = my_malloc(128);
}

/*SPLIT BLOCK 2*/
void malloc_split_block2_setup(void) {
    printf("HIT malloc_split_block2_setup\n");
    init_malloc_test(1, false);

    create_situation_1(&A, &B, &C);

    my_malloc_errno = OUT_OF_MEMORY;
    ret = my_malloc(260);
}

/*SPLIT BLOCK 3*/
void malloc_split_block3_setup(void) {
    printf("HIT malloc_split_block3_setup\n");
    init_malloc_test(1, false);

    create_situation_1(&A, &B, &C);

    my_malloc_errno = OUT_OF_MEMORY;
    ret = my_malloc(607);
}

void malloc_split_min_block_size_setup(void) {
    printf("HIT malloc_split_min_block_size_setup\n");
    init_malloc_test(1, false);

    create_situation_1(&A, &B, &C);

    my_malloc_errno = OUT_OF_MEMORY;
    // should pick B and split when exactly MBS left
    ret = my_malloc(B_START_SIZE - MIN_BLOCK_SIZE);
}

void malloc_nosplit_block1_setup(void) {
    printf("HIT malloc_nosplit_block1_setup\n");
    init_malloc_test(1, false);

    create_situation_1(&A, &B, &C);

    my_malloc_errno = OUT_OF_MEMORY;
    // should pick A but not split
    ret = my_malloc(A_START_SIZE - (MIN_BLOCK_SIZE - 1));
}

void malloc_nosplit_block2_setup(void) {
    printf("HIT malloc_nosplit_block2_setup\n");
    init_malloc_test(1, false);

    create_situation_1(&A, &B, &C);

    my_malloc_errno = OUT_OF_MEMORY;
    // should pick B but not split
    // note that we add 1 here to provoke size_t underflow if you check if you
    // check if you should split like this:
    //   block->size - size - TMS >= MIN_BLOCK_SIZE - TMS;
    // instead, you should write this:
    //   block->size - size >= MIN_BLOCK_SIZE;
    ret = my_malloc(B_START_SIZE - (MIN_BLOCK_SIZE - 1) + 1);
}

void malloc_nosplit_block3_setup(void) {
    printf("HIT malloc_nosplit_block3_setup\n");
    init_malloc_test(1, false);

    create_situation_1(&A, &B, &C);

    my_malloc_errno = OUT_OF_MEMORY;
    // should pick C but not split
    ret = my_malloc(C_START_SIZE - (MIN_BLOCK_SIZE - 1));
}

/* Split Block with SBRK MERGE */
metadata_t* D;
metadata_t* E;
void malloc_split_block_sbrk_merge(void) {
    printf("HIT malloc_split_block_sbrk_merge\n");
    init_malloc_test(1, true);
    D = (metadata_t*)(my_sbrk_fake_heap + D_START_OFF);
    D -> size = D_START_SIZE;

    E = (metadata_t *)(my_sbrk_fake_heap + E_START_OFF);
    E -> size = E_START_SIZE;

    printf("E = %p\n", (void*)E);
    printf("D = %p\n", (void*)D);

    address_list = D;
    D -> next = E;
    E -> next = NULL;

    my_malloc_errno = OUT_OF_MEMORY;
    ret1 = my_malloc(256);
}


/* SBRK WORKS */
void malloc_initial_sbrk_setup(void) {
    printf("HIT malloc_initial_sbrk_setup\n");
    init_malloc_test(1, false);
    address_list = (metadata_t *)my_sbrk_fake_heap;
    address_list->size = SBRK_SIZE - TOTAL_METADATA_SIZE;
    address_list->next = NULL;

    // Set errno to something else to check if student is setting it
    my_malloc_errno = OUT_OF_MEMORY;
    ret = my_malloc(64);
}

/*OOM*/
void malloc_oom_setup(void) {
    printf("HIT malloc_oom_setup\n");
   init_malloc_test(HEAP_SIZE/SBRK_SIZE, true);

    // Set errno to something else to check if student is setting it
    my_malloc_errno = NO_ERROR;
    ret1 = my_malloc(8);
}

/*ZERO*/
void malloc_zero_setup(void) {
    printf("HIT malloc_zero_setup\n");
    init_malloc_test(0, false);

    // Set errno to something else to check if student is setting it
    my_malloc_errno = OUT_OF_MEMORY;
    ret1 = my_malloc(0);
}

/*TOOBIG*/
void malloc_toobig_setup(void) {
    printf("HIT malloc_toobig_setup\n");
    init_malloc_test(0, false);

    // Set errno to something else to check if student is setting it
    my_malloc_errno = OUT_OF_MEMORY;
    ret1 = my_malloc(SBRK_SIZE);
}



// ********************************************************************************************************************* //
// *********************************************** malloc() TESTS ****************************************************** //
// ********************************************************************************************************************* //

                                /***** Malloc_Init Test Case Tests *****/

START_TEST(test_malloc1_retvalue) {
    ck_assert(ret); // Check return value
    uint8_t *split_right_choice =  my_sbrk_fake_heap + SBRK_SIZE - 128;
    ck_ptr(ret, split_right_choice);
    // They set errno
}
END_TEST

START_TEST(test_malloc1_metadata) {
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    metadata_t *meta = (metadata_t *)ret - 1;
    ck_assert_int_eq(meta->size, 128);
    // ck_assert(!meta -> next);
}
END_TEST

//checking sbrk call
START_TEST(test_malloc1_sbrkcall) {
    ck_assert(my_sbrk_called);
}
END_TEST

//checking to see if address_list is perfectly perfect
START_TEST(test_malloc1_lists) {
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    ck_assert(address_list);
    ck_assert(!address_list -> next);
    ck_assert_int_eq(address_list->size, SBRK_SIZE - 128 - (TOTAL_METADATA_SIZE * 2));

    ck_ptr(address_list,  my_sbrk_fake_heap);
}
END_TEST






                                /***** Malloc_Init_SBRK Test Case Tests *****/

//checks to see if the return value is correct and correct error code is set
START_TEST(test_malloc_initial_sbrk_retval) {
//    call_freelist("actual/Malloc_Init_Sbrk.txt");
    ck_assert(ret);
    uint8_t *split_right_choice =  my_sbrk_fake_heap + SBRK_SIZE - 64;
    ck_ptr(ret,  split_right_choice);
}
END_TEST

//checks to see if the metadata of the block returned to the user is correct
START_TEST(test_malloc_initial_sbrk_metadata) {
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    metadata_t *meta = (metadata_t *)ret - 1;
    ck_assert_int_eq(meta->size, 64);
    // ck_assert(!meta -> next);
}
END_TEST

//checks to see if address_list are correct
START_TEST(test_malloc_initial_sbrk_lists) {
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    ck_assert(address_list);
    ck_assert(!address_list->next);
    ck_assert_int_eq(address_list->size, SBRK_SIZE - 64 - 2 * TOTAL_METADATA_SIZE);
    ck_ptr(address_list,  my_sbrk_fake_heap);

}
END_TEST





                                /***** Malloc_Perf_Block1 Test Case Tests *****/

//checking to see if the return value is correct and proper error code is set
START_TEST(test_malloc_perf_block1_retvalue) {
    //call_freelist("actual/Malloc_Perf_Block1.txt");
    ck_assert(ret1);
    ck_ptr(ret1,  (C + 1));
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
}
END_TEST

//checking to see if the metadata of the block returned to the user is correct
START_TEST(test_malloc_perf_block1_metadata) {
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    metadata_t *meta = (metadata_t*)ret1 - 1;
    ck_assert_int_eq(meta -> size, C -> size);
    // ck_assert(!meta -> next);
}
END_TEST

//checks the address_list
START_TEST(test_malloc_perf_block1_lists) {
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);

    //check address_list front to back
    ck_ptr(A,  address_list);
    ck_ptr(A -> next,  B);
    ck_assert(!B -> next);
}
END_TEST







                                /***** Malloc_Perf_Block2 Test Case Tests *****/

//checks to see if returned right block and proper error code set
START_TEST(test_malloc_perf_block2_retvalue) {
    //call_freelist("actual/Malloc_Perf_Block2.txt");
    ck_assert(ret1);
    ck_ptr(ret1,  (B + 1));
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
}
END_TEST

//checks to see if metadata of the returned block is correct
START_TEST(test_malloc_perf_block2_metadata) {
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    metadata_t *meta = (metadata_t*)ret1 - 1;
    ck_assert_int_eq(meta -> size, B -> size);
    // ck_assert(!meta -> next);
}
END_TEST

//checks to see if address_list is correct
START_TEST(test_malloc_perf_block2_lists) {
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    //check address_list front to back
    ck_ptr(A,  address_list);
    ck_ptr(A -> next,  C);
    ck_assert(!C -> next);
}
END_TEST






                                /***** Malloc_Perf_Block3 Test Case Tests *****/

//checks to see if returned right block
START_TEST(test_malloc_perf_block3_retvalue) {
    //call_freelist("actual/Malloc_Perf_Block3.txt");
    ck_assert(ret1);
    ck_ptr(ret1,  (A + 1));
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
}
END_TEST

//checks to make sure metadata of returned block is correct
START_TEST(test_malloc_perf_block3_metadata) {
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    metadata_t *meta = (metadata_t*)ret1 - 1;
    ck_assert_int_eq(meta -> size, A -> size);
    // ck_assert(!meta -> next);
}
END_TEST

//checks to make sure the address_list and address_list are correct
START_TEST(test_malloc_perf_block3_lists) {
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    //check address_list front to back
    ck_ptr(B,  address_list);
    ck_ptr(B -> next,  C);
    ck_assert(!C -> next);
}
END_TEST






                                /***** Malloc_Split_Block1 Test Case Tests *****/

//checking to see if you returned the correct block
START_TEST(test_malloc_split_block1_retvalue) {
    //call_freelist("actual/Malloc_Split_Block1.txt");
    ck_assert(ret);
    //metadata_t* meta = (metadata)ret;
    // ret should be 128 right of the end of A
    uint8_t *split_right = my_sbrk_fake_heap + A_START_OFF + TOTAL_METADATA_SIZE + A_START_SIZE - 128;
    ck_ptr(ret,  split_right);
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
}
END_TEST

//making sure the remainder of the split block is correct in the freelist
START_TEST(test_malloc_split_block1_splitblock) {
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    //make sure the address is correct
    uint8_t *split_left = (uint8_t*)my_sbrk_fake_heap + A_START_OFF;
    ck_ptr(A,  split_left);
    //check to see if the remainder is correct in the list
    ck_assert_int_eq(A -> size, A_START_SIZE - 128 - TOTAL_METADATA_SIZE);
    //the pointers of this block will be checked in the freelist section so no need to duplicate that check here
}
END_TEST

//checking to see if the metadata of the returned block is correct
START_TEST(test_malloc_split_block1_metadata) {
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    //checks to make sure all pointers are nulled out
    metadata_t *meta = (metadata_t*)ret - 1;
    // ck_assert(!meta -> next);
    //checks to make sure size is correct
    ck_assert_int_eq(meta -> size, 128);
}
END_TEST

//checking to see if the lists are correct after the operation
START_TEST(test_malloc_split_block1_lists) {
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);

    // 11/15 Ordered changed based on addressing
    ck_ptr(A,  address_list);
    ck_ptr(A -> next,  B);
    ck_ptr(B -> next,  C);
    ck_assert(!C -> next);
}
END_TEST



                                /***** Malloc_Split_Block2 Test Case Tests *****/

//checking to see if the return value is correct
START_TEST(test_malloc_split_block2_retvalue) {
    //call_freelist("actual/Malloc_Split_Block2.txt");
    ck_assert(ret);
    //metadata_t* meta = (metadata)ret;
    uint8_t *split_right = my_sbrk_fake_heap + C_START_OFF + TOTAL_METADATA_SIZE + (C_START_SIZE - 260);
    ck_ptr(ret,  split_right);
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
}
END_TEST

//checking to see if the remainder is correct in the freelist
START_TEST(test_malloc_split_block2_splitblock) {
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    //make sure the address is correct
    uint8_t *split_left = (uint8_t*)my_sbrk_fake_heap + C_START_OFF;
    ck_ptr(C,  split_left);
    //check to see if the remainder is correct in the list
    unsigned long c_size = C_START_SIZE - 260 - TOTAL_METADATA_SIZE;
    ck_assert_int_eq(C -> size, c_size);
    //the pointers of this block will be checked in the freelist section so no need to duplicate that check here
}
END_TEST

//checking to see if the metadata of the returned block is correct
START_TEST(test_malloc_split_block2_metadata) {
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    //checks to make sure all pointers are nulled out
    metadata_t *meta = (metadata_t*)ret - 1;
    // ck_assert(!meta -> next);
    //checks to make sure size is correct
    ck_assert_int_eq(meta -> size, 260);
}
END_TEST

//check to see if freelists are correct
START_TEST(test_malloc_split_block2_lists) {
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    ck_assert(address_list);
    //checks size list front to back
    // printf("A = %p\n", (void*)A);
    // printf("B = %p\n", (void*)B);
    // printf("C = %p\n", (void*)C);
    // printf("A->next = %p\n", (void*)A->next);
    // printf("B->next = %p\n", (void*)B->next);
    // printf("C->next = %p\n", (void*)C->next);
    ck_ptr(A,  address_list);
    ck_ptr(A -> next,  B);
    ck_ptr(B -> next,  C);
    ck_assert(!C -> next);
}
END_TEST




                                /***** Malloc_Split_Block3 Test Case Tests *****/

//check retvalue correct
START_TEST(test_malloc_split_block3_retvalue) {
    //call_freelist("actual/Malloc_Split_Block3.txt");
    ck_assert(ret);
    //metadata_t* meta = (metadata)ret;
    uint8_t *split_right = my_sbrk_fake_heap + C_START_OFF + TOTAL_METADATA_SIZE + (C_START_SIZE - 607);
    ck_ptr(ret,  split_right);
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
}
END_TEST

//check remainder of block in freelist is correct
START_TEST(test_malloc_split_block3_splitblock) {
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    //make sure the address is correct
    uint8_t *split_left = (uint8_t*)my_sbrk_fake_heap + C_START_OFF;
    ck_ptr(C,  split_left);
    //check to see if the remainder is correct in the list
    unsigned long c_size = C_START_SIZE - 607 - TOTAL_METADATA_SIZE;
    ck_assert_int_eq(C -> size, c_size);
    //the pointers of this block will be checked in the freelist section so no need to duplicate that check here
}
END_TEST

//check metadata of block returned correct
START_TEST(test_malloc_split_block3_metadata) {
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    //checks to make sure all pointers are nulled out
    metadata_t *meta = (metadata_t*)ret - 1;
    // ck_assert(!meta -> next);
    //checks to make sure size is correct
    ck_assert_int_eq(meta -> size, 607);
}
END_TEST

//check for correct freelist
START_TEST(test_malloc_split_block3_lists) {
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);

    //checks address list front to back
    ck_ptr(A,  address_list);
    ck_ptr(A -> next,  B);
    ck_ptr(B -> next,  C);
    ck_assert(!C -> next);
}
END_TEST

                                /***** Malloc_Split_Min_Block_Size Test Case Tests *****/

//checking to see if you returned the correct block
START_TEST(test_malloc_split_min_block_size_retvalue) {
    //call_freelist("actual/Malloc_Split_Block1.txt");
    ck_assert(ret);
    //metadata_t* meta = (metadata)ret;
    // ret should be 128 right of the end of A
    uint8_t *split_right = my_sbrk_fake_heap + B_START_OFF + MIN_BLOCK_SIZE + TOTAL_METADATA_SIZE;
    ck_ptr(ret,  split_right);
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
}
END_TEST

//making sure the remainder of the split block is correct in the freelist
START_TEST(test_malloc_split_min_block_size_splitblock) {
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    //make sure the address is correct
    uint8_t *split_left = (uint8_t*)my_sbrk_fake_heap + A_START_OFF;
    ck_ptr(A,  split_left);
    //check to see if the remainder is correct in the list
    ck_assert_int_eq(B -> size, MIN_BLOCK_SIZE - TOTAL_METADATA_SIZE);
    //the pointers of this block will be checked in the freelist section so no need to duplicate that check here
}
END_TEST

//checking to see if the metadata of the returned block is correct
START_TEST(test_malloc_split_min_block_size_metadata) {
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    //checks to make sure all pointers are nulled out
    metadata_t *meta = (metadata_t*)ret - 1;
    // ck_assert(!meta -> next);
    //checks to make sure size is correct
    ck_assert_int_eq(meta -> size, B_START_SIZE - MIN_BLOCK_SIZE);
}
END_TEST

//checking to see if the lists are correct after the operation
START_TEST(test_malloc_split_min_block_size_lists) {
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);

    ck_ptr(A,  address_list);
    ck_ptr(A -> next,  B);
    ck_ptr(B -> next,  C);
    ck_assert(!C -> next);
}
END_TEST

                                /***** Malloc_No_Split_Block1 Test Case Tests *****/

//checking to see if you returned the correct block
START_TEST(test_malloc_nosplit_block1_retvalue) {
    //call_freelist("actual/Malloc_Split_Block1.txt");
    ck_assert(ret);
    //metadata_t* meta = (metadata)ret;
    uint8_t *expected = my_sbrk_fake_heap + A_START_OFF + TOTAL_METADATA_SIZE;
    ck_ptr(ret, expected);
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
}
END_TEST

//checking to see if the metadata of the returned block is correct
START_TEST(test_malloc_nosplit_block1_metadata) {
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    metadata_t *meta = (metadata_t*) ret - 1;
    ck_assert_int_eq(meta -> size, A_START_SIZE);
}
END_TEST

//checking to see if the lists are correct after the operation
START_TEST(test_malloc_nosplit_block1_lists) {
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    ck_ptr(B,  address_list);
    ck_ptr(B -> next,  C);
    ck_assert(!C -> next);
}
END_TEST

                                /***** Malloc_No_Split_Block2 Test Case Tests *****/

//checking to see if you returned the correct block
START_TEST(test_malloc_nosplit_block2_retvalue) {
    //call_freelist("actual/Malloc_Split_Block2.txt");
    ck_assert(ret);
    //metadata_t* meta = (metadata)ret;
    uint8_t *expected = my_sbrk_fake_heap + B_START_OFF + TOTAL_METADATA_SIZE;
    ck_ptr(ret, expected);
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
}
END_TEST

//checking to see if the metadata of the returned block is correct
START_TEST(test_malloc_nosplit_block2_metadata) {
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    metadata_t *meta = (metadata_t*) ret - 1;
    ck_assert_int_eq(meta -> size, B_START_SIZE);
}
END_TEST

//checking to see if the lists are correct after the operation
START_TEST(test_malloc_nosplit_block2_lists) {
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    ck_ptr(A,  address_list);
    ck_ptr(A -> next,  C);
    ck_assert(!C -> next);
}
END_TEST

                                /***** Malloc_No_Split_Block3 Test Case Tests *****/

//checking to see if you returned the correct block
START_TEST(test_malloc_nosplit_block3_retvalue) {
    //call_freelist("actual/Malloc_Split_Block3.txt");
    ck_assert(ret);
    //metadata_t* meta = (metadata)ret;
    uint8_t *expected = my_sbrk_fake_heap + C_START_OFF + TOTAL_METADATA_SIZE;
    ck_ptr(ret, expected);
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
}
END_TEST

//checking to see if the metadata of the returned block is correct
START_TEST(test_malloc_nosplit_block3_metadata) {
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    metadata_t *meta = (metadata_t*) ret - 1;
    ck_assert_int_eq(meta -> size, C_START_SIZE);
}
END_TEST

//checking to see if the lists are correct after the operation
START_TEST(test_malloc_nosplit_block3_lists) {
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    ck_ptr(A,  address_list);
    ck_ptr(A -> next,  B);
    ck_assert(!B -> next);
}
END_TEST

                                /***** Malloc_Split_Block_SBRKmerge Test Case Tests *****/

//check if sbrk got called correctly
START_TEST(test_malloc_split_block_sbrk_merge_sbrkcalled) {
    //call_freelist("actual/Malloc_Split_Block_SBRKmerge.txt");
    ck_assert(my_sbrk_called);
}
END_TEST

//check to make sure you returned the correct block
START_TEST(test_malloc_split_block_sbrk_merge_retvalue) {
    //set errno correctly
     ck_assert_int_eq(my_malloc_errno, NO_ERROR);
     ck_assert(ret1);
     uint8_t* pointer = my_sbrk_fake_heap + 2* SBRK_SIZE - 256 - TOTAL_METADATA_SIZE + sizeof(metadata_t);
     ck_ptr(ret1,  pointer);
}
END_TEST

//check to make sure the metadata of the returned block is correct
START_TEST(test_malloc_split_block_sbrk_merge_metadata) {
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    metadata_t *meta = (metadata_t *)ret1 - 1;
    ck_assert_int_eq(meta->size, 256);
    // ck_assert(!meta -> next);
}
END_TEST

//check for a correct remainder of the split block in the freelist
START_TEST(test_malloc_split_block_sbrk_merge_splitblock) {
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    uint8_t *split_left = (uint8_t*)my_sbrk_fake_heap + SBRK_SIZE - 64 - TOTAL_METADATA_SIZE;
    ck_ptr(E,  split_left);
    //check to see if the remainder is correct in the list
    unsigned long e_size = SBRK_SIZE - 192 - TOTAL_METADATA_SIZE;
    ck_assert_int_eq(E -> size, e_size);
    //the pointers of this block will be checked in the freelist section so no need to duplicate that check here
}
END_TEST

//check for correct freelists after operation
START_TEST(test_malloc_split_block_sbrk_merge_lists) {
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    //check size list front to back
    ck_ptr(address_list,  D);
    ck_ptr(D -> next,  E);
    ck_assert(!E -> next);
}
END_TEST






                                /***** Malloc_Zero Test Case Test *****/

//check return value
START_TEST(test_malloc_zero_retvalue) {
    //call_freelist("actual/Malloc_Zero.txt");
    ck_assert(!ret1);
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
}
END_TEST





                                /***** Malloc_TooBig Test Case Test *****/
//check return value
START_TEST(test_malloc_toobig_retvalue) {
    //call_freelist("actual/Malloc_TooBig.txt");
    ck_assert(!ret1);
    ck_assert_int_eq(my_malloc_errno, SINGLE_REQUEST_TOO_LARGE);
}
END_TEST




                                /***** Malloc_OOM Test Case Test *****/

//check return value
START_TEST(test_malloc_oom_retvalue) {
    //call_freelist("actual/Malloc_OOM.txt");
    ck_assert(!ret1);
    ck_assert_int_eq(my_malloc_errno, OUT_OF_MEMORY);
}
END_TEST






// *************************************************************************************** //
// *********************** Create malloc testing suite *********************************** //
// *************************************************************************************** //


Suite *malloc_suite(void) {

    /*** Test Case Layout ***/

    /* Setup    - setup_malloc_malloc */
    /* Teardown - teardown_malloc_malloc */

    /* Each test case has an extra setup function specific to the test */

    /* Test Case1 */
    /* Test Case2 */
    /*    ...     */
    /* Test CaseN */

    /* Add the test case to the suite */

    Suite *s = suite_create("malloc");


    TCase *malloc1 = tcase_create("Malloc_Init");
    tcase_add_checked_fixture(malloc1, setup_malloc_malloc, teardown_malloc_malloc);
    tcase_add_checked_fixture(malloc1, malloc1_setup, NULL);
    tcase_add_test(malloc1, test_malloc1_sbrkcall);
    tcase_add_test(malloc1, test_malloc1_lists);
    tcase_add_test(malloc1, test_malloc1_metadata);
    tcase_add_test(malloc1, test_malloc1_retvalue);
    suite_add_tcase(s, malloc1);

    TCase *malloc12 = tcase_create("Malloc_Init_SBRK");
    tcase_add_checked_fixture(malloc12, setup_malloc_malloc, teardown_malloc_malloc);
    tcase_add_checked_fixture(malloc12, malloc_initial_sbrk_setup, NULL);
    tcase_add_test(malloc12, test_malloc_initial_sbrk_lists);
    tcase_add_test(malloc12, test_malloc_initial_sbrk_metadata);
    tcase_add_test(malloc12, test_malloc_initial_sbrk_retval);
    suite_add_tcase(s, malloc12);

    TCase *malloc2 = tcase_create("Malloc_Perf_Block1");
    tcase_add_checked_fixture(malloc2, setup_malloc_malloc, teardown_malloc_malloc);
    tcase_add_checked_fixture(malloc2, malloc_perf_block1_setup, NULL);
    tcase_add_test(malloc2, test_malloc_perf_block1_lists);
    tcase_add_test(malloc2, test_malloc_perf_block1_metadata);
    tcase_add_test(malloc2, test_malloc_perf_block1_retvalue);
    suite_add_tcase(s, malloc2);

    TCase *malloc3 = tcase_create("Malloc_Perf_Block2");
    tcase_add_checked_fixture(malloc3, setup_malloc_malloc, teardown_malloc_malloc);
    tcase_add_checked_fixture(malloc3, malloc_perf_block2_setup, NULL);
    tcase_add_test(malloc3, test_malloc_perf_block2_lists);
    tcase_add_test(malloc3, test_malloc_perf_block2_metadata);
    tcase_add_test(malloc3, test_malloc_perf_block2_retvalue);
    suite_add_tcase(s, malloc3);

    TCase *malloc4 = tcase_create("Malloc_Perf_Block3");
    tcase_add_checked_fixture(malloc4, setup_malloc_malloc, teardown_malloc_malloc);
    tcase_add_checked_fixture(malloc4, malloc_perf_block3_setup, NULL);
    tcase_add_test(malloc4, test_malloc_perf_block3_lists);
    tcase_add_test(malloc4, test_malloc_perf_block3_metadata);
    tcase_add_test(malloc4, test_malloc_perf_block3_retvalue);
    suite_add_tcase(s, malloc4);

    TCase *malloc5 = tcase_create("Malloc_Split_Block1");
    tcase_add_checked_fixture(malloc5, setup_malloc_malloc, teardown_malloc_malloc);
    tcase_add_checked_fixture(malloc5, malloc_split_block1_setup, NULL);
    tcase_add_test(malloc5, test_malloc_split_block1_lists);
    tcase_add_test(malloc5, test_malloc_split_block1_metadata);
    tcase_add_test(malloc5, test_malloc_split_block1_retvalue);
    tcase_add_test(malloc5, test_malloc_split_block1_splitblock);
    suite_add_tcase(s, malloc5);

    TCase *malloc6 = tcase_create("Malloc_Split_Block2");
    tcase_add_checked_fixture(malloc6, setup_malloc_malloc, teardown_malloc_malloc);
    tcase_add_checked_fixture(malloc6, malloc_split_block2_setup, NULL);
    tcase_add_test(malloc6, test_malloc_split_block2_lists);
    tcase_add_test(malloc6, test_malloc_split_block2_metadata);
    tcase_add_test(malloc6, test_malloc_split_block2_retvalue);
    tcase_add_test(malloc6, test_malloc_split_block2_splitblock);
    suite_add_tcase(s, malloc6);

    TCase *malloc7 = tcase_create("Malloc_Split_Block3");
    tcase_add_checked_fixture(malloc7, setup_malloc_malloc, teardown_malloc_malloc);
    tcase_add_checked_fixture(malloc7, malloc_split_block3_setup, NULL);
    tcase_add_test(malloc7, test_malloc_split_block3_lists);
    tcase_add_test(malloc7, test_malloc_split_block3_metadata);
    tcase_add_test(malloc7, test_malloc_split_block3_retvalue);
    tcase_add_test(malloc7, test_malloc_split_block3_splitblock);
    suite_add_tcase(s, malloc7);

    TCase *malloc13 = tcase_create("Malloc_Split_Min_Block_Size");
    tcase_add_checked_fixture(malloc13, setup_malloc_malloc, teardown_malloc_malloc);
    tcase_add_checked_fixture(malloc13, malloc_split_min_block_size_setup, NULL);
    tcase_add_test(malloc13, test_malloc_split_min_block_size_lists);
    tcase_add_test(malloc13, test_malloc_split_min_block_size_metadata);
    tcase_add_test(malloc13, test_malloc_split_min_block_size_retvalue);
    tcase_add_test(malloc13, test_malloc_split_min_block_size_splitblock);
    suite_add_tcase(s, malloc13);

    TCase *malloc14 = tcase_create("Malloc_No_Split_Block1");
    tcase_add_checked_fixture(malloc14, setup_malloc_malloc, teardown_malloc_malloc);
    tcase_add_checked_fixture(malloc14, malloc_nosplit_block1_setup, NULL);
    tcase_add_test(malloc14, test_malloc_nosplit_block1_lists);
    tcase_add_test(malloc14, test_malloc_nosplit_block1_metadata);
    tcase_add_test(malloc14, test_malloc_nosplit_block1_retvalue);
    suite_add_tcase(s, malloc14);

    TCase *malloc15 = tcase_create("Malloc_No_Split_Block2");
    tcase_add_checked_fixture(malloc15, setup_malloc_malloc, teardown_malloc_malloc);
    tcase_add_checked_fixture(malloc15, malloc_nosplit_block2_setup, NULL);
    tcase_add_test(malloc15, test_malloc_nosplit_block2_lists);
    tcase_add_test(malloc15, test_malloc_nosplit_block2_metadata);
    tcase_add_test(malloc15, test_malloc_nosplit_block2_retvalue);
    suite_add_tcase(s, malloc15);

    TCase *malloc16 = tcase_create("Malloc_No_Split_Block3");
    tcase_add_checked_fixture(malloc16, setup_malloc_malloc, teardown_malloc_malloc);
    tcase_add_checked_fixture(malloc16, malloc_nosplit_block3_setup, NULL);
    tcase_add_test(malloc16, test_malloc_nosplit_block3_lists);
    tcase_add_test(malloc16, test_malloc_nosplit_block3_metadata);
    tcase_add_test(malloc16, test_malloc_nosplit_block3_retvalue);
    suite_add_tcase(s, malloc16);

    TCase *malloc11 = tcase_create("Malloc_Split_Block_SBRKmerge");
    tcase_add_checked_fixture(malloc11, setup_malloc_malloc, teardown_malloc_malloc);
    tcase_add_checked_fixture(malloc11, malloc_split_block_sbrk_merge, NULL);
    tcase_add_test(malloc11, test_malloc_split_block_sbrk_merge_lists);
    tcase_add_test(malloc11, test_malloc_split_block_sbrk_merge_splitblock);
    tcase_add_test(malloc11, test_malloc_split_block_sbrk_merge_metadata);
    tcase_add_test(malloc11, test_malloc_split_block_sbrk_merge_retvalue);
    tcase_add_test(malloc11, test_malloc_split_block_sbrk_merge_sbrkcalled);
    suite_add_tcase(s, malloc11);

    TCase *malloc8 = tcase_create("Malloc_Zero");
    tcase_add_checked_fixture(malloc8, setup_malloc_malloc, teardown_malloc_malloc);
    tcase_add_checked_fixture(malloc8, malloc_zero_setup, NULL);
    tcase_add_test(malloc8, test_malloc_zero_retvalue);
    suite_add_tcase(s, malloc8);

    TCase *malloc9 = tcase_create("Malloc_TooBig");
    tcase_add_checked_fixture(malloc9, setup_malloc_malloc, teardown_malloc_malloc);
    tcase_add_checked_fixture(malloc9, malloc_toobig_setup, NULL);
    tcase_add_test(malloc9, test_malloc_toobig_retvalue);
    suite_add_tcase(s, malloc9);

    TCase *malloc10 = tcase_create("Malloc_OOM");
    tcase_add_checked_fixture(malloc10, setup_malloc_malloc, teardown_malloc_malloc);
    tcase_add_checked_fixture(malloc10, malloc_oom_setup, NULL);
    tcase_add_test(malloc10, test_malloc_oom_retvalue);
    suite_add_tcase(s, malloc10);

    return s;

}

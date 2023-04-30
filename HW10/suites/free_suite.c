// Homework 11 Tests, Fall 2018
// Tests based off of ones written by Austin Adams. Formatting and tests and randomness by Maddie Brickell
//
// Warning: much the structure of this file is shamelessly copypasted from
// https://libcheck.github.io/check/doc/check_html/check_3.html

#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "suites.h"

#define SUBPTR(a) ((uint8_t*)((((uint8_t*)(a)) - ((uint8_t*)(my_sbrk_fake_heap))) + 1))

#define ck_ptr(A, B) do { \
    uint8_t* _a = SUBPTR((A)); \
    uint8_t* _b = SUBPTR((B)); \
    ck_assert_msg(_a == _b, "Assertion '%s' failed: %s == %p, %s == %p", #A" == "#B, #A, _a, #B, _b); \
} while(0)


// *************************************************************************************** //
// ************************ test case setup/teardown functions *************************** //
// *************************************************************************************** //

/*Special setup for free*/
void setup_malloc_free(void) {
    setup_malloc_malloc();
    init_malloc_test(1, false);
}

void teardown_malloc_free(void) {
    teardown_malloc_malloc();
}


// *************************************************************************************** //
// ************************ setup functions for specific tests *************************** //
// *************************************************************************************** //

/*Free Null*/
void setup_free_null(void) {
    my_malloc_errno = OUT_OF_MEMORY;
    my_free(NULL);
}

/*Free with an Empty Freelist*/
static metadata_t* meta;
void setup_free_empty_freelist(void) {
    my_malloc_errno = OUT_OF_MEMORY;
    meta = (metadata_t *)(my_sbrk_fake_heap + 64);
    meta->size = 128 + TOTAL_METADATA_SIZE;
    my_free(meta + 1);
}

/*No Merge 1*/
void setup_free_no_merge1(void) {
    // Set errno to something else to check if student is setting it
    my_malloc_errno = OUT_OF_MEMORY;

    // Setup the freelist and fake heap to represent situation 1

    create_situation_1(&A, &B, &C);

    meta = (metadata_t *)((uint8_t *)B + B->size + 8 + TOTAL_METADATA_SIZE);
    meta->size = 32;

    my_free(meta + 1);
}

/*No Merge 2*/
void setup_free_no_merge2(void) {
    // Set errno to something else to check if student is setting it
    my_malloc_errno = OUT_OF_MEMORY;

    // Setup the freelist and fake heap to represent situation 1

    create_situation_1(&A, &B, &C);

    meta = (metadata_t *)my_sbrk_fake_heap;
    meta->size = 400;

    my_free(meta + 1);
}

/*Left Merge 1*/
static unsigned long a_size;
metadata_t* prev_a;
void setup_free_left_merge1(void) {
    // Set errno to something else to check if student is setting it
    my_malloc_errno = OUT_OF_MEMORY;

    // Setup the freelist and fake heap to represent situation 1

    create_situation_1(&A, &B, &C);
    prev_a = A;
    a_size = A -> size;
    meta = (metadata_t *)((uint8_t*)A + A -> size + TOTAL_METADATA_SIZE);
    meta->size = 64;

    my_free(meta + 1);
}

/*Left Merge 2 *The* GIANT*/
static unsigned long b_size;
metadata_t* prev_b;
void setup_free_left_merge2(void) {
    // Set errno to something else to check if student is setting it
    my_malloc_errno = OUT_OF_MEMORY;

    // Setup the freelist and fake heap to represent situation 1
    create_situation_1(&A, &B, &C);
    prev_b = B;
    b_size = B -> size;
    meta = (metadata_t *)((uint8_t*)B + B -> size + TOTAL_METADATA_SIZE);
    //to make the ...GIANT BLOCK... we gotta get rid of C. Adios Amigos!
    B -> next = NULL;

    meta->size = SBRK_SIZE - 1472 - 2*TOTAL_METADATA_SIZE;

    my_free(meta + 1);
}

/*Right Merge 1 the *right* GIANT*/
metadata_t * prev_meta;
void setup_free_right_merge1(void) {
    // Set errno to something else to check if student is setting it
    my_malloc_errno = OUT_OF_MEMORY;

    // Setup the freelist and fake heap to represent situation 1
    create_situation_1(&A, &B, &C);
    b_size = B -> size;
    meta = (metadata_t *)((uint8_t*)B - 64 - TOTAL_METADATA_SIZE);
    prev_meta = meta;
    meta->size = 64;

    my_free(meta + 1);
}

/*Right Merge 2 hehehe*/
static unsigned long c_size;
void setup_free_right_merge2(void) {
    // Set errno to something else to check if student is setting it
    my_malloc_errno = OUT_OF_MEMORY;

    // Setup the freelist and fake heap to represent situation 1
    create_situation_1(&A, &B, &C);
    c_size = C -> size;
    meta = (metadata_t *)((uint8_t*)C - 255);

    meta->size = 255 - TOTAL_METADATA_SIZE;

    my_free(meta + 1);
}

/*Double Merge 1*/
void setup_free_double_merge1(void) {
    // Set errno to something else to check if student is setting it
    my_malloc_errno = OUT_OF_MEMORY;

    // Setup the freelist and fake heap to represent situation 1
    create_situation_1(&A, &B, &C);
    b_size = B -> size;
    a_size = A -> size;
    prev_a = A;
    meta = (metadata_t *)((uint8_t*)A + A -> size + TOTAL_METADATA_SIZE);

    meta->size = B_START_OFF - A_START_OFF - TOTAL_METADATA_SIZE * 2 - A_START_SIZE;

    my_free(meta + 1);
}

/*Double Merge 2*/
void setup_free_double_merge2(void) {
    // Set errno to something else to check if student is setting it
    my_malloc_errno = OUT_OF_MEMORY;

    // Setup the freelist and fake heap to represent situation 1
    create_situation_1(&A, &B, &C);
    prev_b = B;
    b_size = B -> size;
    c_size = C -> size;
    meta = (metadata_t *)((uint8_t*)B + B -> size + TOTAL_METADATA_SIZE);

    meta->size = C_START_OFF - B_START_OFF - TOTAL_METADATA_SIZE * 2 - B_START_SIZE;

    my_free(meta + 1);
}

/*DOUBLE DECKER FOR DANIEL BECKER, AKA THE BIGGEST BLOCK YOU'VE EVA SEEEEEEN*/
void setup_free_double_decker(void) {
    // Set errno to something else to check if student is setting it
    my_malloc_errno = OUT_OF_MEMORY;

    // Setup the freelist and fake heap to represent situation 1
    create_situation_1(&A, &B, &C);
    prev_a = A;
    b_size = B -> size;
    c_size = C -> size;
    a_size = A -> size;
    meta = (metadata_t *)((uint8_t*)B + B -> size + TOTAL_METADATA_SIZE);

    meta->size = C_START_OFF - B_START_OFF - TOTAL_METADATA_SIZE * 2 - B_START_SIZE;

    my_free(meta + 1);

    meta = (metadata_t *)((uint8_t*)A + A -> size + TOTAL_METADATA_SIZE);

    meta->size = B_START_OFF - A_START_OFF - TOTAL_METADATA_SIZE * 2 - A_START_SIZE;

    my_free(meta + 1);
}





// ********************************************************************************************************************* //
// *********************************************** free() TESTS ******************************************************** //
// ********************************************************************************************************************* //

//tests to make sure you checked error code correctly
START_TEST(test_free_null_gen) {
    //call_freelist("actual/Free_Null.txt");
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
}
END_TEST


//check the lists
START_TEST(test_free_empty_freelist_lists) {
    //call_freelist("actual/Free_Empty_Freelist.txt");
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    //check address_list
    ck_assert(address_list);
    ck_ptr(address_list, meta);
    ck_assert(!address_list -> next);
}
END_TEST

//check if remainder in free list is correct correct
START_TEST(test_free_empty_freelist_insert) {
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    ck_ptr(meta, my_sbrk_fake_heap + 64);
    ck_assert_int_eq(meta -> size, 128 + TOTAL_METADATA_SIZE);
}
END_TEST

//check the freelists
START_TEST(test_free_no_merge1_lists) {
    //call_freelist("actual/Free_NoMerge1.txt");
    // printf("A = %p\n", (void*)A);
    // printf("B = %p\n", (void*)B);
    // printf("C = %p\n", (void*)C);
    // printf("meta = %p\n", (void*)meta);
    // printf("address_list = %p\n", (void*)address_list);
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);

    //check size list front to back
    ck_ptr(meta, address_list->next->next);
    ck_ptr(meta -> next, C);
    ck_ptr(A, address_list);
    ck_ptr(A -> next, B);
    ck_ptr(B -> next, meta);
    ck_assert(!C -> next);
}
END_TEST

//check the block in the freelist for correct metadata
START_TEST(test_free_no_merge1_insert) {
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    ck_assert_int_eq(meta -> size, 32);
    ck_ptr(meta, (uint8_t*)B + B -> size + 8 + TOTAL_METADATA_SIZE);
}
END_TEST

//check for correctness in the freelist
START_TEST(test_free_no_merge2_lists) {
    //call_freelist("actual/Free_NoMerge2.txt");
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    //check size list front to back
    ck_ptr(meta, address_list);
    ck_ptr(B -> next, C);
    ck_ptr(A -> next, B);
    ck_ptr(meta -> next, A);
    ck_assert(!C -> next);
}
END_TEST

//check if the metadata of the inserted block is correct
START_TEST(test_free_no_merge2_insert) {
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    ck_assert_int_eq(meta -> size, 400);
    ck_ptr(meta, my_sbrk_fake_heap);
}
END_TEST


//Check to make sure the freelists are correct
START_TEST(test_free_left_merge1_lists) {
    //call_freelist("actual/Free_LeftMerge1.txt");
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);

    //check size list front to back
    ck_ptr(A, address_list);
    ck_ptr(A -> next, B);
    ck_ptr(B -> next, C);
    ck_assert(!C -> next);
}
END_TEST

//check to make sure the inserted block is correct
START_TEST(test_free_left_merge1_insert) {
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    ck_assert_int_eq(A -> size, a_size + 64 + TOTAL_METADATA_SIZE);
    ck_ptr(A, prev_a);
}
END_TEST


//check for correct freelists
START_TEST(test_free_left_merge2_lists) {
    //call_freelist("actual/Free_LeftMerge2.txt");
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);

    ck_assert(address_list);
    //check size list front to back
    ck_ptr(A, address_list);
    ck_ptr(A -> next, B);
    ck_assert(!B -> next);
}
END_TEST

//check to make sure the inserted block is correct
START_TEST(test_free_left_merge2_insert) {
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    ck_assert_int_eq(B -> size, b_size + SBRK_SIZE - 1472 - TOTAL_METADATA_SIZE);
    ck_ptr(B, prev_b);
}
END_TEST

//check to see if the freelist is correct
START_TEST(test_free_right_merge1_lists) {
    //call_freelist("actual/Free_RightMerge1.txt");
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    //check size list front to back
    ck_ptr(A, address_list);
    ck_ptr(A -> next, meta);
    ck_ptr(meta -> next, C);
    ck_assert(!C -> next);
}
END_TEST

//Check to see if the inserted block is correct
START_TEST(test_free_right_merge1_insert) {
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    ck_assert_int_eq(meta -> size, b_size + 64 + TOTAL_METADATA_SIZE);
    ck_ptr(meta, prev_meta);
}
END_TEST


//check to make sure the freelist is correct
START_TEST(test_free_right_merge2_lists) {
    //call_freelist("actual/Free_RightMerge2.txt");
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    //check size list front to back
    ck_ptr(A, address_list);
    ck_ptr(A -> next, B);
    ck_ptr(B -> next, meta);
    ck_assert(!meta -> next);
}
END_TEST

//check to make sure the block is inserted correctly
START_TEST(test_free_right_merge2_insert) {
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    ck_assert_int_eq(meta -> size, c_size + 255);
    ck_ptr(meta, (uint8_t*)C - 255);
}
END_TEST

//check to make sure the freelist is correct
START_TEST(test_free_double_merge1_lists) {
    //call_freelist("actual/Free_DoubleMerge1.txt");
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    //check size list front to back
    ck_ptr(A, address_list);
    ck_ptr(A -> next, C);
    ck_assert(!C -> next);
}
END_TEST

//check to make sure the block is inserted correctly
START_TEST(test_free_double_merge1_insert) {
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    size_t meta_size = B_START_OFF - A_START_OFF - TOTAL_METADATA_SIZE * 2 - A_START_SIZE;
    ck_assert_int_eq(A -> size, b_size + meta_size + a_size + 2 * TOTAL_METADATA_SIZE);
    ck_ptr(A, prev_a);
}
END_TEST


//check to make sure the lists are completely correct
START_TEST(test_free_double_merge2_lists) {
    //call_freelist("actual/Free_DoubleMerge2.txt");
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    //check size list front to back
    ck_ptr(A, address_list);
    ck_ptr(A -> next, B);
    ck_assert(!B -> next);
}
END_TEST

//check to make sure the student inserted the block correctly
START_TEST(test_free_double_merge2_insert) {
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    size_t meta_size = C_START_OFF - B_START_OFF - TOTAL_METADATA_SIZE * 2 - B_START_SIZE;
    ck_assert_int_eq(B -> size, b_size + meta_size + c_size + 2*TOTAL_METADATA_SIZE);
    ck_ptr(B, prev_b);
}
END_TEST


//check the lists for correctness
START_TEST(test_free_double_decker_lists) {
    //call_freelist("actual/Free_DoubleDecker.txt");
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    //check size list
    ck_ptr(A, address_list);
    ck_assert(!A -> next);

}
END_TEST

//check that the inserted block is correct
START_TEST(test_free_double_decker_insert) {
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    size_t meta_1_size = C_START_OFF - B_START_OFF - TOTAL_METADATA_SIZE * 2 - B_START_SIZE;
    size_t meta_2_size = B_START_OFF - A_START_OFF - TOTAL_METADATA_SIZE * 2 - A_START_SIZE;
    ck_assert_int_eq(A -> size, a_size + meta_1_size + b_size + meta_2_size + c_size + 4 * TOTAL_METADATA_SIZE);
    ck_ptr(A, prev_a);

    //make sure you don't have pointers flying everywhere
    ck_assert(!B -> next);

    ck_assert(!C -> next);

}
END_TEST







// *************************************************************************************** //
// ************************* Create free testing suite *********************************** //
// *************************************************************************************** //

Suite *free_suite(void) {

        /*** Test Case Layout ***/

    /* Setup    - setup_malloc_malloc */
    /* Teardown - teardown_malloc_malloc */

    /* Each test case has an extra setup function specific to the test */

    /* Test Case1 */
    /* Test Case2 */
    /*    ...     */
    /* Test CaseN */

    /* Add the test case to the suite */

    Suite *s = suite_create("free");


    TCase *free1 = tcase_create("Free_Null");
    tcase_add_checked_fixture(free1, setup_malloc_free, teardown_malloc_free);
    tcase_add_checked_fixture(free1, setup_free_null, NULL);
    tcase_add_test(free1, test_free_null_gen);
    suite_add_tcase(s, free1);

    TCase *free4 = tcase_create("Free_Empty_Freelist");
    tcase_add_checked_fixture(free4, setup_malloc_free, teardown_malloc_free);
    tcase_add_checked_fixture(free4, setup_free_empty_freelist, NULL);
    tcase_add_test(free4, test_free_empty_freelist_insert);
    tcase_add_test(free4, test_free_empty_freelist_lists);
    suite_add_tcase(s, free4);

    TCase *free5 = tcase_create("Free_NoMerge1");
    tcase_add_checked_fixture(free5, setup_malloc_free, teardown_malloc_free);
    tcase_add_checked_fixture(free5, setup_free_no_merge1, NULL);
    tcase_add_test(free5, test_free_no_merge1_insert);
    tcase_add_test(free5, test_free_no_merge1_lists);
    suite_add_tcase(s, free5);

    TCase *free6 = tcase_create("Free_NoMerge2");
    tcase_add_checked_fixture(free6, setup_malloc_free, teardown_malloc_free);
    tcase_add_checked_fixture(free6, setup_free_no_merge2, NULL);
    tcase_add_test(free6, test_free_no_merge2_insert);
    tcase_add_test(free6, test_free_no_merge2_lists);
    suite_add_tcase(s, free6);

    TCase *free7 = tcase_create("Free_LeftMerge1");
    tcase_add_checked_fixture(free7, setup_malloc_free, teardown_malloc_free);
    tcase_add_checked_fixture(free7, setup_free_left_merge1, NULL);
    tcase_add_test(free7, test_free_left_merge1_insert);
    tcase_add_test(free7, test_free_left_merge1_lists);
    suite_add_tcase(s, free7);

    TCase *free8 = tcase_create("Free_LeftMerge2");
    tcase_add_checked_fixture(free8, setup_malloc_free, teardown_malloc_free);
    tcase_add_checked_fixture(free8, setup_free_left_merge2, NULL);
    tcase_add_test(free8, test_free_left_merge2_insert);
    tcase_add_test(free8, test_free_left_merge2_lists);
    suite_add_tcase(s, free8);

    TCase *free9 = tcase_create("Free_RightMerge1");
    tcase_add_checked_fixture(free9, setup_malloc_free, teardown_malloc_free);
    tcase_add_checked_fixture(free9, setup_free_right_merge1, NULL);
    tcase_add_test(free9, test_free_right_merge1_insert);
    tcase_add_test(free9, test_free_right_merge1_lists);
    suite_add_tcase(s, free9);

    TCase *free10 = tcase_create("Free_RightMerge2");
    tcase_add_checked_fixture(free10, setup_malloc_free, teardown_malloc_free);
    tcase_add_checked_fixture(free10, setup_free_right_merge2, NULL);
    tcase_add_test(free10, test_free_right_merge2_insert);
    tcase_add_test(free10, test_free_right_merge2_lists);
    suite_add_tcase(s, free10);

    TCase *free11 = tcase_create("Free_DoubleMerge1");
    tcase_add_checked_fixture(free11, setup_malloc_free, teardown_malloc_free);
    tcase_add_checked_fixture(free11, setup_free_double_merge1, NULL);
    tcase_add_test(free11, test_free_double_merge1_insert);
    tcase_add_test(free11, test_free_double_merge1_lists);
    suite_add_tcase(s, free11);

    TCase *free12 = tcase_create("Free_DoubleMerge2");
    tcase_add_checked_fixture(free12, setup_malloc_free, teardown_malloc_free);
    tcase_add_checked_fixture(free12, setup_free_double_merge2, NULL);
    tcase_add_test(free12, test_free_double_merge2_insert);
    tcase_add_test(free12, test_free_double_merge2_lists);
    suite_add_tcase(s, free12);

    TCase *free13 = tcase_create("Free_DoubleDecker");
    tcase_add_checked_fixture(free13, setup_malloc_free, teardown_malloc_free);
    tcase_add_checked_fixture(free13, setup_free_double_decker, NULL);
    tcase_add_test(free13, test_free_double_decker_insert);
    tcase_add_test(free13, test_free_double_decker_lists);
    suite_add_tcase(s, free13);

    return s;

}

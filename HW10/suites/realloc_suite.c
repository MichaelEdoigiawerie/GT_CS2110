// Homework 11 Tests, Fall 2017
// Tests (and quotes) by the one, the only Austin Adams. Formatting done by Maddie Brickell
//
// Warning: much the structure of this file is shamelessly copypasted from
// https://libcheck.github.io/check/doc/check_html/check_3.html

#include <check.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "suites.h"

//
// realloc() tests
//

#define SUBPTR(a) ((uint8_t*)((((uint8_t*)(a)) - ((uint8_t*)(my_sbrk_fake_heap))) + 1))

#define ck_ptr(A, B) do { \
    uint8_t* _a = SUBPTR((A)); \
    uint8_t* _b = SUBPTR((B)); \
    ck_assert_msg(_a == _b, "Assertion '%s' failed: %s == %p, %s == %p", #A" == "#B, #A, _a, #B, _b); \
} while(0)


// *************************************************************************************** //
// ************************ test case setup/teardown functions *************************** //
// *************************************************************************************** //

                            /* Defined in malloc_suite.c */




// *************************************************************************************** //
// ************************ setup functions for specific tests *************************** //
// *************************************************************************************** //

/*Initial Realloc*/
void setup_realloc_initial(void) {
    init_malloc_test(0, true);
    my_malloc_errno = OUT_OF_MEMORY;
    ret = my_realloc(NULL, 1024);
}

static metadata_t* meta;
/*Realloc Zero*/
void setup_realloc_zero(void) {
    init_malloc_test(1, false);

    // Put something in the freelist to check they don't touch it
    meta = (metadata_t *)my_sbrk_fake_heap;
    meta->size = SBRK_SIZE;
    meta->next = NULL;
    address_list = meta;

    // Set errno to something else to check if student is setting it
    my_malloc_errno = OUT_OF_MEMORY;
    ret = my_realloc(NULL, 0);
}

/*Copy Realloc*/
char *important_data;
static unsigned long a_size;
static unsigned long b_size;
static unsigned long c_size;
void setup_realloc_copy(void) {
    init_malloc_test(1, false);

    // Counting the null terminator, this is 55 bytes
    important_data = "Watch your fingers 'cause the cactus dangerous (yeah)\n";

    // Setup the freelist and fake heap to represent situation 1
    create_situation_1(&A, &B, &C);
    a_size = A->size;
    b_size = B->size;
    c_size = C->size;

    // Put this important data in memory somewhere
    meta = (metadata_t *)((uint8_t *)B + B->size + 128);
    meta->size = 55;
    // Copy in our important data
    strcpy((char *)(meta + 1), important_data);

    // Set errno to something else to check if student is setting it
    my_malloc_errno = OUT_OF_MEMORY;
    ret = my_realloc(meta + 1, 128);
}

/*Realloc a smaller block*/
uint8_t *byte_after_B;
void setup_realloc_copy_smaller(void) {
    init_malloc_test(1, false);

    // Counting the null terminator, this is 242 bytes
    important_data = "Uh, yeah, mansion it sit on the hill, (woo) after my last arrest (woo, woo)\n"
                           "[Individuals] out here in the field, don't need a mask or vest (brrt, brrt)\n"
                           "Uh, back to back Lambos, repeat it, you like to run when it's heated (run, back to back)\n";

    // Setup the freelist and fake heap to represent situation 1
    create_situation_1(&A, &B, &C);
    // I'm lazy, so shrink A a little bit so that it'll be first in the new
    // freelist instead of the block passed in
    A->size -= 64;
    a_size = A->size;
    b_size = B->size;
    c_size = C->size;

    // Put this important data in memory somewhere
    meta = (metadata_t *)my_sbrk_fake_heap;
    meta->size = 242;
    // Copy in our important data
    strcpy((char *)(meta + 1), important_data);
    // B will be chosen, so to make sure they're not writing past the end of B,
    // zero out the byte directly following B and check that it stays zero.
    byte_after_B = (uint8_t *)B + b_size + TOTAL_METADATA_SIZE;
    *byte_after_B = 0;

    // Set errno to something else to check if student is setting it
    my_malloc_errno = OUT_OF_MEMORY;
    ret = my_realloc(meta + 1, 64);
}

/*Double Merge 2 - Realloc*/
static metadata_t* prev_b;
void setup_realloc_free(void) {
    // Set errno to something else to check if student is setting it
    my_malloc_errno = OUT_OF_MEMORY;

    // Setup the freelist and fake heap to represent situation 1
    create_situation_1(&A, &B, &C);
    prev_b = B;
    b_size = B -> size;
    c_size = C -> size;
    meta = (metadata_t *)((uint8_t*)B + B -> size + TOTAL_METADATA_SIZE);

    meta->size = C_START_OFF - B_START_OFF - TOTAL_METADATA_SIZE * 2 - B_START_SIZE;

    ret = my_realloc(meta + 1, 0);
}

/*TOOBIG*/
void setup_realloc_toobig(void) {
    init_malloc_test(0, false);

    // Set errno to something else to check if student is setting it
    my_malloc_errno = OUT_OF_MEMORY;
    ret = my_realloc(NULL, SBRK_SIZE);
}

/*OOM*/
void setup_realloc_oom(void) {
   init_malloc_test(HEAP_SIZE/SBRK_SIZE, true);

    // Set errno to something else to check if student is setting it
    my_malloc_errno = NO_ERROR;
    ret = my_realloc(NULL, 8);
}

// ********************************************************************************************************************* //
// *********************************************** realloc() TESTS ***************************************************** //
// ********************************************************************************************************************* //


//check the return value
START_TEST(test_realloc_initial_retvalue) {
    //call_freelist("actual/Realloc_Init.txt");
    ck_assert(ret);
    uint8_t *split_right_choice =  my_sbrk_fake_heap + SBRK_SIZE - 1024;
    ck_ptr(ret, split_right_choice);
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
}
END_TEST

//check the metadata of the returned value
START_TEST(test_realloc_initial_metadata) {
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    metadata_t *meta = (metadata_t *)ret - 1;
    ck_assert_int_eq(meta->size, 1024);
}
END_TEST

//check the sbrk called correctly
START_TEST(test_realloc_initial_sbrkcall) {
    ck_assert(my_sbrk_called);
}
END_TEST

//make sure lists are correct
START_TEST(test_realloc_initial_lists) {
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    ck_assert(address_list);
    ck_assert(!address_list -> next);
    ck_assert_int_eq(address_list -> size, SBRK_SIZE - 1024 - (TOTAL_METADATA_SIZE * 2));
    ck_ptr((uint8_t*)address_list, my_sbrk_fake_heap);

}
END_TEST



//check return value
START_TEST(test_realloc_zero_retvalue) {
   //call_freelist("actual/Realloc_Zero.txt");
    ck_assert(!ret);
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
}
END_TEST

//check lists
START_TEST(test_realloc_zero_lists) {
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    ck_assert(address_list);
    ck_assert(!address_list -> next);
    ck_ptr(address_list, meta);
    ck_assert_int_eq(address_list -> size, SBRK_SIZE);

}
END_TEST



//test return value
START_TEST(test_realloc_copy_retvalue) {
    //call_freelist("actual/Realloc_Copy.txt");
    ck_assert(ret);
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    uint8_t *split_right_choice = (uint8_t *)A + a_size - 128  + sizeof (metadata_t);
    ck_ptr(ret, split_right_choice);
    ck_assert(!strncmp((char *)ret, important_data, 55));
}
END_TEST

//test the freelists
START_TEST(test_realloc_copy_lists) {
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    //size list front to back
    ck_assert(address_list);
    ck_ptr(address_list, A);
    ck_ptr(A -> next, B);
    ck_ptr(B -> next, meta);
    ck_ptr(meta -> next, C);
    ck_assert(!C -> next);

    //probably should just go and check the sizes of things to make sure you kiddies are doing things right
    ck_assert_int_eq(A -> size, a_size - 128 - TOTAL_METADATA_SIZE);
    ck_assert_int_eq(B -> size, b_size);
    ck_assert_int_eq(C -> size, c_size);

}
END_TEST



// Check return value
START_TEST(test_realloc_copy_smaller_retvalue) {
    //call_freelist("actual/Realloc_Copy_Baby.txt");
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    ck_assert(ret);
    metadata_t *ret_meta = (metadata_t *)ret - 1;
    ck_ptr(ret_meta, B);
    ck_assert_int_eq(ret_meta->size, b_size);

    // Check our important data is intact.
    // Use strncmp() so this test doesn't hang/segfault by walking way off into
    // memory looking for a null terminator
    ck_assert(!strncmp((char *)ret, important_data, 64));
    // Make sure they didn't write past B.
    ck_assert(*byte_after_B != important_data[64 + sizeof (unsigned long)]);
}
END_TEST

//test lists
START_TEST(test_realloc_copy_smaller_lists) {
    //size list front to back
    ck_assert(address_list);
    ck_ptr(address_list, meta);
    ck_ptr(meta -> next, A);
    ck_ptr(A -> next, C);
    ck_assert(!C -> next);

    //probably should just go and check the sizes of things to make sure you kiddies are doing things right
    ck_assert_int_eq(A -> size, a_size);
    ck_assert_int_eq(meta -> size, 242);
    ck_assert_int_eq(C -> size, c_size);

}
END_TEST



//test return value
START_TEST(test_realloc_free_retvalue) {
    //call_freelist("actual/Realloc_Free.txt");
    ck_assert(!ret);
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
}
END_TEST

//test free lists here
START_TEST(test_realloc_free_lists) {
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);

    ck_assert(address_list);
    //check size list front to back
    ck_ptr(A, address_list);
    ck_ptr(A -> next, B);
    ck_assert(!B -> next);
}
END_TEST

//make sure the block inserted into the freelist is correct
START_TEST(test_realloc_free_insert) {
    ck_assert_int_eq(my_malloc_errno, NO_ERROR);
    size_t meta_size = C_START_OFF - B_START_OFF - TOTAL_METADATA_SIZE * 2 - B_START_SIZE;
    ck_assert_int_eq(B -> size,  b_size + meta_size + c_size + 2 * TOTAL_METADATA_SIZE);
    ck_ptr(B, prev_b);
}
END_TEST


//check ret value
START_TEST(test_realloc_toobig_retvalue) {
    //call_freelist("actual/Realloc_TooBig.txt");
    ck_assert(!ret);
    ck_assert_int_eq(my_malloc_errno, SINGLE_REQUEST_TOO_LARGE);
}
END_TEST




//check retvalue
START_TEST(test_realloc_oom_retvalue) {
    //call_freelist("actual/Realloc_OOM.txt");
    ck_assert(!ret);
    ck_assert_int_eq(my_malloc_errno, OUT_OF_MEMORY);
}
END_TEST


// *************************************************************************************** //
// ************************ Create realloc testing suite ********************************* //
// *************************************************************************************** //

Suite *realloc_suite(void) {

            /*** Test Case Layout ***/

    /* Setup    - setup_malloc_malloc */
    /* Teardown - teardown_malloc_malloc */

    /* Each test case has an extra setup function specific to the test */

    /* Test Case1 */
    /* Test Case2 */
    /*    ...     */
    /* Test CaseN */

    /* Add the test case to the suite */

    Suite *s = suite_create("Realloc");


    TCase *realloc1 = tcase_create("Realloc_Init");
    tcase_add_checked_fixture(realloc1, setup_malloc_malloc, teardown_malloc_malloc);
    tcase_add_checked_fixture(realloc1, setup_realloc_initial, NULL);
    tcase_add_test(realloc1, test_realloc_initial_lists);
    tcase_add_test(realloc1, test_realloc_initial_sbrkcall);
    tcase_add_test(realloc1, test_realloc_initial_retvalue);
    tcase_add_test(realloc1, test_realloc_initial_metadata);
    suite_add_tcase(s, realloc1);

    TCase *realloc2 = tcase_create("Realloc_Zero");
    tcase_add_checked_fixture(realloc2, setup_malloc_malloc, teardown_malloc_malloc);
    tcase_add_checked_fixture(realloc2, setup_realloc_zero, NULL);
    tcase_add_test(realloc2, test_realloc_zero_lists);
    tcase_add_test(realloc2, test_realloc_zero_retvalue);
    suite_add_tcase(s, realloc2);

    TCase *realloc3 = tcase_create("Realloc_Copy");
    tcase_add_checked_fixture(realloc3, setup_malloc_malloc, teardown_malloc_malloc);
    tcase_add_checked_fixture(realloc3, setup_realloc_copy, NULL);
    tcase_add_test(realloc3, test_realloc_copy_retvalue);
    tcase_add_test(realloc3, test_realloc_copy_lists);
    suite_add_tcase(s, realloc3);

    TCase *realloc4 = tcase_create("Realloc_Copy_Baby");
    tcase_add_checked_fixture(realloc4, setup_malloc_malloc, teardown_malloc_malloc);
    tcase_add_checked_fixture(realloc4, setup_realloc_copy_smaller, NULL);
    tcase_add_test(realloc4, test_realloc_copy_smaller_retvalue);
    tcase_add_test(realloc4, test_realloc_copy_smaller_lists);
    suite_add_tcase(s, realloc4);

    TCase *realloc5 = tcase_create("Realloc_Free");
    tcase_add_checked_fixture(realloc5, setup_malloc_malloc, teardown_malloc_malloc);
    tcase_add_checked_fixture(realloc5, setup_realloc_free, NULL);
    tcase_add_test(realloc5, test_realloc_free_retvalue);
    tcase_add_test(realloc5, test_realloc_free_lists);
    tcase_add_test(realloc5, test_realloc_free_insert);
    suite_add_tcase(s, realloc5);

    TCase *realloc6 = tcase_create("Realloc_TooBig");
    tcase_add_checked_fixture(realloc6, setup_malloc_malloc, teardown_malloc_malloc);
    tcase_add_checked_fixture(realloc6, setup_realloc_toobig, NULL);
    tcase_add_test(realloc6, test_realloc_toobig_retvalue);
    suite_add_tcase(s, realloc6);

    TCase *realloc7 = tcase_create("Realloc_OOM");
    tcase_add_checked_fixture(realloc7, setup_malloc_malloc, teardown_malloc_malloc);
    tcase_add_checked_fixture(realloc7, setup_realloc_oom, NULL);
    tcase_add_test(realloc7, test_realloc_oom_retvalue);
    suite_add_tcase(s, realloc7);

    return s;

}

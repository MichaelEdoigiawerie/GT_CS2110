// Homework 10 Helper Tests, Spring 2023
// Tests based off of ones written by Austin Adams.
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
void setup_malloc_helpers(void) {
    setup_malloc_malloc();
}

void teardown_malloc_helpers(void) {
    teardown_malloc_malloc();
}

// *************************************************************************************** //
// ************************ setup functions for specific tests *************************** //
// *************************************************************************************** //


// ********************************************************************************************************************* //
// *********************************************** add_to_addr_list() TESTS ******************************************** //
// ********************************************************************************************************************* //
START_TEST(test_add_to_addr_list_empty) {
    metadata_t *new_block = (metadata_t *)my_sbrk_fake_heap;
    new_block->size = SBRK_SIZE;
    new_block->next = (metadata_t *) 0x69; // useful for checking that the next value is set properly

    add_to_addr_list(new_block);

    ck_ptr(new_block, address_list);
    ck_ptr(new_block->next, NULL);
}
END_TEST

START_TEST(test_add_to_addr_list_front) {
    create_situation_1(&A, &B, &C);

    // create new block that is before A, B, or C
    metadata_t *new_block = (metadata_t *) (my_sbrk_fake_heap + D_START_OFF);
    new_block->size = D_START_SIZE;
    new_block->next = (metadata_t *) 0x69; // useful for checking that the next value is set properly

    add_to_addr_list(new_block);

    // new block is inserted at the front
    ck_ptr(new_block, address_list);
    ck_ptr(new_block->next, A);

    // rest of the list is in tact
    ck_ptr(A->next, B);
    ck_ptr(B->next, C);
    ck_ptr(C->next, NULL);
}
END_TEST

START_TEST(test_add_to_addr_list_middle) {
    create_situation_1(&A, &B, &C);

    // create new block that is between B and C
    metadata_t *new_block = (metadata_t *) (my_sbrk_fake_heap + 1100); // 1100 is in between B's offset plus size (928 + 64) and C's offset (1216)
    new_block->size = 64;
    new_block->next = (metadata_t *) 0x69; // useful for checking that the next value is set properly

    add_to_addr_list(new_block);

    // new block is inserted in between B and C
    ck_ptr(new_block, B->next);
    ck_ptr(new_block->next, C);

    // rest of the list is in tact
    ck_ptr(address_list, A);
    ck_ptr(A->next, B);
    ck_ptr(C->next, NULL);
}
END_TEST

START_TEST(test_add_to_addr_list_end) {
    create_situation_1(&A, &B, &C);

    // create new block that is before A, B, or C
    metadata_t *new_block = (metadata_t *) (my_sbrk_fake_heap + C_START_OFF + C_START_SIZE + TOTAL_METADATA_SIZE + 100); 
    // new block should be after the C block
    new_block->size = 128;
    new_block->next = (metadata_t *) 0x69; // useful for checking that the next value is set properly

    add_to_addr_list(new_block);

    // new block is inserted in between B and C
    ck_ptr(new_block, C->next);
    ck_ptr(new_block->next, NULL);

    // rest of the list is intact
    ck_ptr(address_list, A);
    ck_ptr(A->next, B);
    ck_ptr(B->next, C);
}
END_TEST

START_TEST(test_add_to_addr_list_single) {
    B = (metadata_t *)(my_sbrk_fake_heap + B_START_OFF);
    B->size = B_START_SIZE;
    B->next = NULL;
    address_list = B;

    C = (metadata_t *)(my_sbrk_fake_heap + C_START_OFF);
    C->size = C_START_SIZE;
    C->next = (metadata_t *) 0x69; // useful for checking that the next value is set properly

    add_to_addr_list(C);

    // C block is inserted after B
    ck_ptr(address_list, B);
    ck_ptr(B->next, C);
    ck_ptr(C->next, NULL);

    A = (metadata_t *)(my_sbrk_fake_heap + A_START_OFF);
    A->size = A_START_SIZE;
    A->next = (metadata_t *) 0x69; // useful for checking that the next value is set properly

    // "remove" C
    B->next = NULL;

    add_to_addr_list(A);

    // A block is inserted before B
    ck_ptr(address_list, A);
    ck_ptr(A->next, B);
    ck_ptr(B->next, NULL);
}
END_TEST

// ********************************************************************************************************************* //
// *********************************************** split_block() TESTS ************************************************* //
// ********************************************************************************************************************* //
START_TEST(test_split_block_1) {
    create_situation_1(&A, &B, &C);
    metadata_t * new_block = split_block(A, 30);
    // check that A and the new_block are "contiguous" based on A's size
    ck_ptr(((uint8_t *)(A + 1) + A->size), new_block);
    // check that new_block's size is set properly
    ck_assert_int_eq(new_block->size, 30);
    // shortened A block is still in the freelist
    ck_ptr(address_list, A);
    ck_ptr(A->next, B);
}
END_TEST

START_TEST(test_split_block_2) {
    create_situation_1(&A, &B, &C);
    metadata_t *new_block = split_block(B, 6);
    // check that B and the new_block are "contiguous" based on B's size
    ck_ptr(((uint8_t *)(B + 1) + B->size), new_block);
    // check that new_block's size is set properly
    ck_assert_int_eq(new_block->size, 6);
    // shortened B block is still in the freelist
    ck_ptr(address_list, A);
    ck_ptr(A->next, B);
    ck_ptr(B->next, C);
}
END_TEST

// ********************************************************************************************************************* //
// *********************************************** merge() TESTS ******************************************************* //
// ********************************************************************************************************************* //

START_TEST(test_merge_1) {
    metadata_t *left_block = (metadata_t *) (my_sbrk_fake_heap);
    left_block->size = 120;
    metadata_t *right_block = (metadata_t *) (my_sbrk_fake_heap + TOTAL_METADATA_SIZE + 120);
    right_block->size = 400;
    right_block->next = NULL;
    left_block->next = right_block;

    address_list = left_block;

    merge(left_block, right_block);

    // left_block still in list (with right_block removed)
    ck_ptr(left_block->next, NULL);
    ck_ptr(left_block, address_list);

    // left_block's size is set properly
    ck_assert_int_eq(left_block->size, 520 + TOTAL_METADATA_SIZE);
}
END_TEST

START_TEST(test_merge_2) {
    metadata_t *block_one = (metadata_t *) (my_sbrk_fake_heap);
    block_one->size = 45;
    metadata_t *block_two = (metadata_t *) (my_sbrk_fake_heap + TOTAL_METADATA_SIZE + 45);
    block_two->size = 400;
    metadata_t *block_three = (metadata_t *) (my_sbrk_fake_heap + 2*TOTAL_METADATA_SIZE + 45 + 400);
    block_three->size = 100;
    block_one->next = block_two;
    block_two->next = block_three;
    block_three->next = NULL;
    
    address_list = block_one;

    merge(block_one, block_two);

    // block_one still in list (with block_removed)
    ck_ptr(block_one->next, block_three);
    ck_ptr(block_one, address_list);

    // block_one's size is set properly
    ck_assert_int_eq(block_one->size, 445 + TOTAL_METADATA_SIZE);
}
END_TEST

// ********************************************************************************************************************* //
// *********************************************** find_best_fit() TESTS *********************************************** //
// ********************************************************************************************************************* //
START_TEST(test_find_best_fit_perfect_fit) {
    // create free list from the right node to the left
    C = (metadata_t *) (my_sbrk_fake_heap + C_START_OFF);
    C->next = NULL;
    C->size = C_START_SIZE;

    B = (metadata_t *) (my_sbrk_fake_heap + B_START_OFF);
    B->next = C;
    B->size = B_START_SIZE;

    A = (metadata_t *) (my_sbrk_fake_heap + A_START_OFF);
    A->next = B;
    A->size = A_START_SIZE;

    // Set 'A' to be the head of the free list
    address_list = A;

    // run the method
    metadata_t *foundBlock = find_best_fit((size_t)(B_START_SIZE));

    // run the tests
    ck_ptr(foundBlock, B);
    ck_assert_int_eq(foundBlock->size, B->size);
    ck_ptr(foundBlock->next, B->next);
} END_TEST

START_TEST(test_find_best_fit_imperfect_fit) {
    // create free list from the right node to the left
    C = (metadata_t *) (my_sbrk_fake_heap + C_START_OFF);
    C->next = NULL;
    C->size = C_START_SIZE;

    B = (metadata_t *) (my_sbrk_fake_heap + B_START_OFF);
    B->next = C;
    B->size = B_START_SIZE;

    A = (metadata_t *) (my_sbrk_fake_heap + A_START_OFF);
    A->next = B;
    A->size = A_START_SIZE; 

    // Set 'A' to be the head of the free list
    address_list = A;

    // run the method
    metadata_t *foundBlock = find_best_fit((size_t)94);

    // run the tests
    ck_ptr(foundBlock, A);
    ck_assert_int_eq(foundBlock->size, A->size);
    ck_ptr(foundBlock->next, A->next);
} END_TEST

START_TEST(test_find_best_fit_no_fit) {
    // create free list from the right node to the left
    C = (metadata_t *) (my_sbrk_fake_heap + C_START_OFF);
    C->next = NULL;
    C->size = C_START_SIZE;

    B = (metadata_t *) (my_sbrk_fake_heap + B_START_OFF);
    B->next = C;
    B->size = B_START_SIZE;

    A = (metadata_t *) (my_sbrk_fake_heap + A_START_OFF);
    A->next = B;
    A->size = A_START_SIZE; 

    // Set 'A' to be the head of the free list
    address_list = A;

    // run the method
    metadata_t *foundBlock = find_best_fit((size_t)10000);

    // run the test  
    ck_ptr(foundBlock, NULL);
} END_TEST

// ********************************************************************************************************************* //
// *********************************************** find_right() TESTS ************************************************** //
// ********************************************************************************************************************* //

START_TEST(test_find_right_end_of_list) {
    // create free list from the right node to the left
    C = (metadata_t *) (my_sbrk_fake_heap + C_START_OFF);
    C->next = NULL;
    C->size = C_START_SIZE - TOTAL_METADATA_SIZE;

    B = (metadata_t *) (my_sbrk_fake_heap + B_START_OFF);
    B->next = C;
    B->size = B_START_SIZE - TOTAL_METADATA_SIZE;

    A = (metadata_t *) (my_sbrk_fake_heap + A_START_OFF);
    A->next = B;
    A->size = A_START_SIZE - TOTAL_METADATA_SIZE; 

    // Set 'B' to be the head of the free list
    address_list = B;

    // run the method
    metadata_t *foundFit = find_right(C);

    // run the test
    ck_ptr(foundFit, NULL);
} END_TEST

START_TEST(test_find_right_immediate) {
    // create free list from the right node to the left
    C = (metadata_t * )(my_sbrk_fake_heap + C_START_OFF);
    C->size = (SBRK_SIZE - A_START_SIZE - C_START_SIZE);
    C->next = NULL;

    B = (metadata_t *)(my_sbrk_fake_heap + A_START_OFF + A_START_SIZE);
    B->size = B_START_SIZE - TOTAL_METADATA_SIZE;
    B->next = C;

    A = (metadata_t *) (my_sbrk_fake_heap + A_START_OFF);
    A->next = B;
    A->size = A_START_SIZE - TOTAL_METADATA_SIZE;

    // Set 'A' to be the head of the free list
    address_list = A;

    // run the method
    metadata_t *foundBlock = find_right(A);

    // run the tests
    ck_ptr(foundBlock, B);
    ck_assert_int_eq(foundBlock->size, B->size);
    ck_ptr(foundBlock->next, B->next);
} END_TEST

START_TEST(test_find_right_none) {
    // create free list from the right node to the left
    C = (metadata_t *) (my_sbrk_fake_heap + C_START_OFF);
    C->next = NULL;
    C->size = C_START_SIZE - TOTAL_METADATA_SIZE;

    B = (metadata_t *) (my_sbrk_fake_heap + B_START_OFF);
    B->next = C;
    B->size = B_START_SIZE - TOTAL_METADATA_SIZE;

    A = (metadata_t *) (my_sbrk_fake_heap + A_START_OFF);
    A->next = B;
    A->size = A_START_SIZE - TOTAL_METADATA_SIZE; 

    // Set 'A' to be the head of the free list
    address_list = A;

    // run the method
    metadata_t *foundFit = find_right(A);

    // run the tests
    ck_ptr(foundFit, NULL);
} END_TEST

Suite *helpers_suite(void) {

    Suite *s = suite_create("helpers");

    TCase *add_to_addr_list_tests = tcase_create("add_to_addr_list");
    tcase_add_checked_fixture(add_to_addr_list_tests, setup_malloc_helpers, teardown_malloc_helpers);
    tcase_add_test(add_to_addr_list_tests, test_add_to_addr_list_empty);
    tcase_add_test(add_to_addr_list_tests, test_add_to_addr_list_front);
    tcase_add_test(add_to_addr_list_tests, test_add_to_addr_list_middle);
    tcase_add_test(add_to_addr_list_tests, test_add_to_addr_list_end);
    tcase_add_test(add_to_addr_list_tests, test_add_to_addr_list_single);
    suite_add_tcase(s, add_to_addr_list_tests);

    TCase *split_block_tests = tcase_create("split_block");
    tcase_add_checked_fixture(split_block_tests, setup_malloc_helpers, teardown_malloc_helpers);
    tcase_add_test(split_block_tests, test_split_block_1);
    tcase_add_test(split_block_tests, test_split_block_2);
    suite_add_tcase(s, split_block_tests);

    TCase *merge_tests = tcase_create("merge");
    tcase_add_checked_fixture(merge_tests, setup_malloc_helpers, teardown_malloc_helpers);
    tcase_add_test(merge_tests, test_merge_1);
    tcase_add_test(merge_tests, test_merge_2);
    suite_add_tcase(s, merge_tests);

    TCase *find_best_fit_tests = tcase_create("find_best_fit");
    tcase_add_checked_fixture(find_best_fit_tests, setup_malloc_helpers, teardown_malloc_helpers);
    tcase_add_test(find_best_fit_tests, test_find_best_fit_perfect_fit);
    tcase_add_test(find_best_fit_tests, test_find_best_fit_imperfect_fit);
    tcase_add_test(find_best_fit_tests, test_find_best_fit_no_fit);
    suite_add_tcase(s, find_best_fit_tests);

    TCase *find_right_tests = tcase_create("find_right");
    tcase_add_checked_fixture(find_right_tests, setup_malloc_helpers, teardown_malloc_helpers);
    tcase_add_test(find_right_tests, test_find_right_end_of_list);
    tcase_add_test(find_right_tests, test_find_right_immediate);
    tcase_add_test(find_right_tests, test_find_right_none);
    suite_add_tcase(s, find_right_tests);

    return s;
}
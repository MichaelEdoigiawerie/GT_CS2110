/**
 * @file tl4_suite.c
 * @author  Nandha, Michael, Saif
 * @brief Testing Suites to TL4 - Spring 2023
 * @date 2023-04-06
 */

// Check Docs
// Tutorial : https://libcheck.github.io/check/doc/check_html/check_3.html
// Check API: https://libcheck.github.io/check/doc/doxygen/html/check_8h.html

// System Headers
#include <string.h>
#include <check.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys/wait.h> // For grabbing return value of system call

// TA Headers
#include "test_utils.h"

int malloc_use_pattern = 0;
int malloc_pattern_ind = 0;
int malloc_pattern[10] = {0};

// allow infinite mallocs by default
static void reset_malloc_pattern(void) {
    malloc_use_pattern = 0;
    malloc_pattern_ind = 0;
    memset(malloc_pattern, 0, sizeof(malloc_pattern));
}

static void reset(void) {
    reset_malloc_pattern();
    stack_arr = NULL;
}



const char *questions[7] = {"When is Final Exam?", 
"What to study for TL4?", 
"Will the Final Exam be cumulitive?", 
"Difference between pointer and array variable?", 
"How are the 2110 TAs so amazing?", // I know you guys were thinking this ;) 
"How can I schedule a meeting with a professor?", 
"How do I sign up to be a 2110 TA?"};

static void _create_normal_stack(void) {
    _initialize_stack(5);
    _stack_push(questions[0], GENERAL);
}

static void assert_post_data_equals(struct post_t *post, const char *postLabel, const char *origQuestion, enum category_t category) {
    ck_assert_msg(post != NULL, "expected %s to be non-null, instead was NULL", postLabel);
    ck_assert_msg(strcmp(post->question, origQuestion) == 0, "expected %s->question to be %s, instead was %s", postLabel, origQuestion, post->question);
    ck_assert_msg(post->question != origQuestion, "expected %s->question to be deepcopied, instead was same as passed in question", postLabel);
    ck_assert_msg(post->category == category, "expected %s->category to be %d, instead was %d", postLabel, category, post->category);
}

// static void assert_node_data_equals(struct queue_node_t *node, const char *nodeLabel, const char *origquestion, enum assignment_t assignment) {
//     ck_assert_msg(node != NULL, "expected %s to be non-null, instead was NULL", nodeLabel);
//     ck_assert_msg(strcmp(node->data.question, origquestion) == 0, "expected %s->data.question to be %s, instead was %s", nodeLabel, origquestion, queue_tail->data.question);
//     ck_assert_msg(node->data.question != origquestion, "expected %s->data.question to be deepcopied, instead was same as passed in question", nodeLabel);
//     ck_assert_msg(node->data.assignment == assignment, "expected %s->data.assignment to be %d, instead was %d", nodeLabel, assignment, queue_tail->data.assignment);
// }



/* stack_push tests */

START_TEST(test_stack_push_normal) {
    _create_normal_stack();
    const char *question = questions[4];
    enum category_t category = GENERAL;
    int success = stack_push(question, category);

    ck_assert_msg(success == 0, "expected stack_push to return zero on success, instead was %d", success);
    ck_assert_msg(stack_arr != NULL, "expected stack_arr to be non-null, instead was NULL");
    assert_post_data_equals(stack_arr[1], "top post", questions[4], category);
    assert_post_data_equals(stack_arr[0], "bottom post", questions[0], category);
    ck_assert_msg(numPosts == 2, "expected numPosts == %d but instead was %d", 2, numPosts);

    _destroy_stack();
}
END_TEST

START_TEST(test_stack_push_null_question) {
    _create_normal_stack();
    enum category_t category = HOMEWORK_9;
    int success = stack_push(NULL, category);

    ck_assert_msg(success == 1, "expected stack_push to return zero on failure, instead was %d", success);
    ck_assert_msg(stack_arr != NULL, "expected stack_arr to be non-null, instead was NULL");
    ck_assert_msg(numPosts == 1, "expected numPosts == %d but instead was %d", 1, numPosts);
    assert_post_data_equals(stack_arr[0], "post", questions[0], GENERAL);

    _destroy_stack();
}
END_TEST

START_TEST(test_stack_push_empty_string_question) {
    _create_normal_stack();
    const char *question = "";
    enum category_t category = HOMEWORK_10;
    int success = stack_push(question, category);

    ck_assert_msg(success == 1, "expected queue_add to return 1 with empty string question, instead was %d", success);
    ck_assert_msg(stack_arr != NULL, "expected stack_arr to be non-null, instead was NULL");
    ck_assert_msg(numPosts == 1, "expected numPosts == %d but instead was %d", 1, numPosts);
    assert_post_data_equals(stack_arr[0], "post", questions[0], GENERAL);

    _destroy_stack();
}
END_TEST

START_TEST(test_stack_push_over_capacity) {
    _create_normal_stack();
    for(int i = 1; i < 5; i++) {
        stack_push(questions[i], TIMED_LAB_1);
    }
    int success = stack_push(questions[6], LOGISTICS);
    ck_assert_msg(success == 0, "expected stack_push to return zero on success, instead was %d", success);
    ck_assert_msg(stack_arr != NULL, "expected stack_arr to be non-null, instead was NULL");
    ck_assert_msg(numPosts == 6, "expected numPosts == %d but instead was %d", 6, numPosts);
    ck_assert_msg(capacity == 10, "expected capacity == %d but instead was %d", 10, capacity);
    for(int i = 1; i < 5; i++) {
        assert_post_data_equals(stack_arr[i], "post", questions[i], TIMED_LAB_1);
    }
    assert_post_data_equals(stack_arr[5], "top post", questions[6], LOGISTICS);

    _destroy_stack();
}
END_TEST

START_TEST(test_stack_push_0_mallocs_should_fail) {
    _create_normal_stack();
    const char *question = questions[1];
    enum category_t category = TIMED_LAB_4;
    malloc_use_pattern = 1;
    int success = stack_push(question, category);

    ck_assert_msg(success == 1, "expected queue_add to return 1 when malloc fails, instead was %d", success);
    ck_assert_msg(stack_arr != NULL, "expected stack_arr to be non-null, instead was NULL");
    ck_assert_msg(numPosts == 1, "expected numPosts == %d but instead was %d", 1, numPosts);
    assert_post_data_equals(stack_arr[0], "post", questions[0], GENERAL);

    _destroy_stack();
}
// END_TEST

START_TEST(test_stack_push_1_mallocs_no_leaks) {
    _create_normal_stack();
    const char *question = questions[6];
    enum category_t category = GENERAL;
    malloc_use_pattern = 1;
    malloc_pattern[0] = 1;
    stack_push(question, category);

    malloc_use_pattern = 1;
    malloc_pattern_ind = 0;
    malloc_pattern[0] = 0;
    malloc_pattern[1] = 1;
    stack_push(question, category);

    _destroy_stack();
}
END_TEST

/* stack_pop tests */

START_TEST(test_stack_pop_null_data) {
    _create_normal_stack();
    int success = stack_pop(NULL);

    ck_assert_msg(success == 1, "expected stack_pop to return 1 on failure (NULL data), instead was %d", success);
    ck_assert_msg(numPosts == 1, "expected numPosts == %d but instead was %d", 1, numPosts);
    assert_post_data_equals(stack_arr[0], "post", questions[0], GENERAL);

    _destroy_stack();
}
END_TEST

START_TEST(test_stack_pop_empty_stack) {
    _initialize_stack(5);
    struct post_t removed_post;
    int result = stack_pop(&removed_post);
    ck_assert_msg(result == FAILURE, "expected stack_pop to return FAILURE, instead was %d", result);
    ck_assert_msg(stack_arr != NULL, "expected stack_arr to be non-null, instead was NULL");
    ck_assert_msg(numPosts == 0, "expected numPosts == 0 but instead was %d", numPosts);

    _destroy_stack();
}
END_TEST


START_TEST(test_stack_pop_normal) {
    _create_normal_stack();
    stack_push(questions[1], LAB);

    struct post_t removed_post;
    int success = stack_pop(&removed_post);

    ck_assert_msg(success == 0, "expected stack_pop to return zero on success, instead was %d", success);
    ck_assert_msg(numPosts == 1, "expected numPosts == %d but instead was %d", 1, numPosts);
    assert_post_data_equals(&removed_post, "removed post", questions[1], LAB);
    assert_post_data_equals(stack_arr[0], "remaining post", questions[0], GENERAL);
    free(removed_post.question);

    _destroy_stack();
}
END_TEST

START_TEST(test_stack_pop_until_empty) {

    _create_normal_stack();
    _stack_push(questions[1], LAB);
    _stack_push(questions[2], LECTURE);
    int n = numPosts;
    int success;

    for (int i = 0; i < n; i++) {
        struct post_t popped_post;
        success = stack_pop(&popped_post);
        ck_assert_msg(success == SUCCESS, "expected stack_pop call %d to return SUCCESS, instead got %d", i + 1, success);
        free(popped_post.question);
    }

    ck_assert_msg(numPosts == 0, "expected numPosts to be 0 after popping all elements, instead was %d", numPosts);

    _destroy_stack();
}
END_TEST


START_TEST(test_stack_pop_single_element) {
    _create_normal_stack();
    struct post_t popped_post;
    int success = stack_pop(&popped_post);

    ck_assert_msg(success == SUCCESS, "expected stack_pop to return SUCCESS, instead was %d", success);
    ck_assert_msg(stack_arr != NULL, "expected stack_arr to be non-null, instead was NULL");
    ck_assert_msg(numPosts == 0, "expected numPosts == 0 but instead was %d", numPosts);
    assert_post_data_equals(&popped_post, "popped post", questions[0], GENERAL);

    free(popped_post.question);
    _destroy_stack();
}
END_TEST

START_TEST(test_stack_pop_multiple_elements) {
    _create_normal_stack();
    _stack_push(questions[1], LAB);
    _stack_push(questions[2], LECTURE);
    struct post_t popped_post;
    int success = stack_pop(&popped_post);

    ck_assert_msg(success == SUCCESS, "expected stack_pop to return SUCCESS, instead was %d", success);
    ck_assert_msg(stack_arr != NULL, "expected stack_arr to be non-null, instead was NULL");
    ck_assert_msg(numPosts == 2, "expected numPosts == 2 but instead was %d", numPosts);
    assert_post_data_equals(&popped_post, "popped post", questions[2], LECTURE);
    assert_post_data_equals(stack_arr[0], "bottom post", questions[0], GENERAL);
    assert_post_data_equals(stack_arr[1], "middle post", questions[1], LAB);

    free(popped_post.question);
    _destroy_stack();
}
END_TEST

START_TEST(test_destroy_stack_normal) {
    _create_normal_stack();
    const char *question = questions[3];
    enum category_t category = GENERAL;
    
    _stack_push(question, category);

    const char *question2 = questions[0];
    _stack_push(question2, category);
    
    int success = destroy_stack();
    ck_assert_msg(success == 0, "expected destroy_stack to return zero on success, instead was %d", success);
    ck_assert_msg(numPosts == 0, "expected destroy_stack to set numPosts to equal 0, instead found %d", numPosts);
}
END_TEST

START_TEST(test_destroy_stack_single_element) {
    _create_normal_stack();
    const char *question = questions[3];
    enum category_t category = GENERAL;
    
    _stack_push(question, category);
    
    int success = destroy_stack();
    ck_assert_msg(success == 0, "expected destroy_stack to return zero on success, instead was %d", success);
    ck_assert_msg(numPosts == 0, "expected destroy_stack to set numPosts to equal 0, instead found %d", numPosts);
}
END_TEST

START_TEST(test_destroy_stack_return_value) {
    _create_normal_stack();
    const char *question = questions[3];
    enum category_t category = GENERAL;
    
    _stack_push(question, category);
    
    int success = destroy_stack();
    ck_assert_msg(success == 0, "expected destroy_stack to return zero on success, instead was %d", success);
}
END_TEST

START_TEST(test_destroy_stack_empty) {
    _initialize_stack(5);
    
    int success = destroy_stack();
    ck_assert_msg(success == 0, "expected destroy_stack to return zero on success, instead was %d", success);
    ck_assert_msg(numPosts == 0, "expected destroy_stack to set numPosts to equal 0, instead found %d", numPosts);
}
END_TEST


Suite *tl4_suite(void)
{
    Suite *s = suite_create("tl4_suite");

    // stack_push tests
    tcase_singleton(s, reset, NULL, test_stack_push_normal);
    tcase_singleton(s, reset, NULL, test_stack_push_null_question);
    tcase_singleton(s, reset, NULL, test_stack_push_empty_string_question);
    tcase_singleton(s, reset, NULL, test_stack_push_over_capacity);
    tcase_singleton(s, reset, NULL, test_stack_push_0_mallocs_should_fail);
    tcase_singleton(s, reset, NULL, test_stack_push_1_mallocs_no_leaks);

    // stack_pop tests
    tcase_singleton(s, reset, NULL, test_stack_pop_null_data);
    tcase_singleton(s, reset, NULL, test_stack_pop_empty_stack);
    tcase_singleton(s, reset, NULL, test_stack_pop_normal);
    tcase_singleton(s, reset, NULL, test_stack_pop_until_empty);
    tcase_singleton(s, reset, NULL, test_stack_pop_single_element);
    tcase_singleton(s, reset, NULL, test_stack_pop_multiple_elements);

    //destroy_stack_tests
    tcase_singleton(s, reset, NULL, test_destroy_stack_normal);
    tcase_singleton(s, reset, NULL, test_destroy_stack_single_element);
    tcase_singleton(s, reset, NULL, test_destroy_stack_return_value);
    tcase_singleton(s, reset, NULL, test_destroy_stack_empty);




    return s;
}

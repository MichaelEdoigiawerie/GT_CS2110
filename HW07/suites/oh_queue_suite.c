/**
 * @file hw7_suite.c
 * @author Richard Zhang
 * @brief Testing Suites to HW7 - Spring 2023
 * @date 2023-03-13
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

#define INCORRECT_RESULT_MSG "array at index %d not populated correctly. Expected: %s, %d, %d (in order).\nargv[1]='%s'\n"

// Helper function prototypes
static void setup(void);
static void _push(const char *studentName, const enum subject topicName, const float questionNumber, struct public_key pub_key);
static void _hash(int *ciphertext, char *plaintext, struct public_key pub_key);

///////////////////////////////////////////////////////////////////////////////////////////////////
/******************************************* TESTS ***********************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////

/* push() tests */
// Test that push works when called once (this is the "basic" intro test, easy test case)
START_TEST(test_push_single)
{
    // Step 1: the setup() function is called automagically
    // We don't really need to change it, all it does is clear the array, set size=0 and detect illegal #includes

    // Step 2: per-test setup
    // If we want to do anything particular for this test (maybe have some data already in the array, we should add it here)
    // It's fine if we start from an empty array for this test, so nothing needed here either
    // Frequently, we have helper methods to help with setup. Check out the _push function; this is
    //   entirely separate from the student's push function and is essentially a "working" implementation
    //   (without error handling) that you can use to add test data to the array.

    // Step 3: make the function call we're testing (in this case, addUser)
    const char *student_name = "George Burdell";
    const enum subject subject = Assembly;
    const float questionNumber = 6;
    struct public_key pub_key;
    pub_key.n = 3;
    pub_key.e = 4;
    
    int retVal = push(student_name, subject, questionNumber, pub_key);

    // Step 4: Test that the return value is correct
    // If the first parameter is false, then the autograder test case will fail and print the message
    // If the first parameter is true, then it'll just keep going
    ck_assert_msg(retVal == SUCCESS, "Expected return value to be SUCCESS (%d) when inserting into empty users, got %d", SUCCESS, retVal);

    // Step 5: Make sure the internal state is correct
    // This function updates the pokedex, so we should make sure that the correct values were inserted
    // You should also check that the other entries already in the pokedex aren't deleted (there aren't an y in this case)
    // For other functions that don't update the pokedex, you would still want to check that didn't accidentally get changed
    // We don't need to test that the values at nickname and speciesName haven't changed, since they are pointers to const chars
    //printf("\n%s %d %f %s\n", species, id, hungerScale, habitat);
    //printf("%s %d %f %s\n\n", animals[0].species, animals[0].id, animals[0].hungerScale, animals[0].habitat);
    
    ck_assert_msg(oh_queue.stats.no_of_people_in_queue == 1, "Expected no_of_people_in_queue to be 1 after inserting into empty users, was %d", oh_queue.stats.no_of_people_in_queue);
    ck_assert_msg(strncmp(oh_queue.students[0].studentData.name, student_name, MAX_NAME_LENGTH) == 0, "Expected name to be %s, was %s", student_name, oh_queue.students[0].studentData.name);
    ck_assert_msg(oh_queue.students[0].studentData.topic.topicName == subject, "Expected topicName to be %d, was %d", subject, oh_queue.students[0].studentData.topic.topicName);
    int expected_id[30];
    _hash(expected_id, (char *) student_name, pub_key);
    for (int i = 0; i < (int) strlen(student_name); i++) {
        ck_assert_int_eq(oh_queue.students[0].customID[i], expected_id[i]);
    }
    ck_assert_msg(oh_queue.students[0].studentData.topic.questionNumber == questionNumber, "Expected questionNumber to be %f, was %f", questionNumber, oh_queue.students[0].studentData.topic.questionNumber);
    ck_assert_msg(oh_queue.students[0].queue_number == 0, "Expected queue_number to be 0, was %d", oh_queue.students[0].queue_number);
}
END_TEST // if you get weird compiler error messages, you probably forgot the END_TEST

// Now, repeat the process above until the function is fully tested.
// Make sure you test all "regular" cases (if the pokedex is empty, if there are already entries, etc.) as well as
//  also testing the edge cases (if the name is longer than the max length, if the company name is longer than the
//  max length, if both of those are true, etc.)
START_TEST(test_push_two)
{
    const char *student_name1 = "Sophie Imhof";
    const enum subject subject1 = LC3;
    const float questionNumber1 = 7;
    struct public_key pub_key1;
    pub_key1.n = 1;
    pub_key1.e = 2;
    int retVal1 = push(student_name1, subject1, questionNumber1, pub_key1);

    const char *student_name2 = "Shawn Wahi";
    const enum subject subject2 = C;
    const float questionNumber2 = 9;
    struct public_key pub_key2;
    pub_key2.n = 7;
    pub_key2.e = 3;
    int retVal2 = push(student_name2, subject2, questionNumber2, pub_key2);

    ck_assert_int_eq(retVal1, SUCCESS);
    ck_assert_int_eq(retVal2, SUCCESS);

    ck_assert_msg(oh_queue.stats.no_of_people_in_queue == 2, "Expected no_of_people_in_queue to be 2 after inserting into users, was %d", oh_queue.stats.no_of_people_in_queue);

    ck_assert_msg(oh_queue.students[0].studentData.topic.topicName == subject1, "Expected topicName to be %d, was %d", subject1, oh_queue.students[0].studentData.topic.topicName);
    ck_assert_msg(strncmp(oh_queue.students[0].studentData.name, student_name1, MAX_NAME_LENGTH) == 0, "Expected name to be %s, was %s", student_name1, oh_queue.students[0].studentData.name);

    int expected_id[30];
    _hash(expected_id, (char *) student_name1, pub_key1);
    for (int i = 0; i < (int) strlen(student_name1); i++) {
        ck_assert_int_eq(oh_queue.students[0].customID[i], expected_id[i]);
    }
    ck_assert_msg(oh_queue.students[0].studentData.topic.questionNumber == questionNumber1, "Expected questionNumber to be %f, was %f", questionNumber1, oh_queue.students[0].studentData.topic.questionNumber);
    ck_assert_msg(oh_queue.students[0].queue_number == 0, "Expected queue_number to be 0 was %d", oh_queue.students[0].queue_number);

    ck_assert_msg(oh_queue.students[1].studentData.topic.topicName == subject2, "Expected topicName to be %d, was %d", subject2, oh_queue.students[0].studentData.topic.topicName);
    ck_assert_msg(strncmp(oh_queue.students[1].studentData.name, student_name2, MAX_NAME_LENGTH) == 0, "Expected name to be %s, was %s", student_name2, oh_queue.students[0].studentData.name);

    _hash(expected_id, (char *) student_name2, pub_key2);
    for (int i = 0; i < (int) strlen(student_name2); i++) {
        ck_assert_int_eq(oh_queue.students[1].customID[i], expected_id[i]);
    }
    ck_assert_msg(oh_queue.students[1].studentData.topic.questionNumber == questionNumber2, "Expected questionNumber to be %f, was %f", questionNumber2, oh_queue.students[0].studentData.topic.questionNumber);
    ck_assert_msg(oh_queue.students[1].queue_number == 1, "Expected queue_number to be 1 was %d", oh_queue.students[0].queue_number);
}
END_TEST

START_TEST(test_push_multiple)
{
    for (int i = 0; i < 25; i++) {
        char student_name[10];
        sprintf(student_name, "student%d", i);
        enum subject subject = LC3;
        float questionNumber = 3 * i * i;
        struct public_key pub_key;
        pub_key.n = i + 2;
        pub_key.e = i + 4;
        
        int retVal = push(student_name, subject, questionNumber, pub_key);

        ck_assert_int_eq(retVal, SUCCESS);
    }

    ck_assert_msg(oh_queue.stats.no_of_people_in_queue == 25, "Expected no_of_people_in_queue to be 25 after pushing 25 users, was %d", oh_queue.stats.no_of_people_in_queue);
    
    for (int i = 0; i < 25; i++) {
        char student_name[10];
        sprintf(student_name, "student%d", i);
        enum subject subject = LC3;
        float questionNumber = 3 * i * i;
        struct public_key pub_key;
        pub_key.n = i + 2;
        pub_key.e = i + 4;

        ck_assert_msg(oh_queue.students[i].studentData.topic.topicName == subject, "Expected topicName to be %d, was %d", subject, oh_queue.students[i].studentData.topic.topicName);
        ck_assert_msg(strncmp(oh_queue.students[i].studentData.name, student_name, MAX_NAME_LENGTH) == 0, "Expected name to be %s, was %s", student_name, oh_queue.students[i].studentData.name);
        int expected_id[30];
        _hash(expected_id, (char *) student_name, pub_key);
        for (int j = 0; j < (int) strlen(student_name); j++) {
            ck_assert_int_eq(oh_queue.students[i].customID[j], expected_id[j]);
        }
        ck_assert_msg(oh_queue.students[i].studentData.topic.questionNumber == questionNumber, "Expected questionNumber to be %f, was %f", questionNumber, oh_queue.students[i].studentData.topic.questionNumber);
        ck_assert_msg(oh_queue.students[i].queue_number == i, "Expected queue_number to be %d, was %d", i, oh_queue.students[i].queue_number);
    }
}
END_TEST

START_TEST(test_push_null_terminator)
{
    // ensures there will be some data around if they don't properly null terminate
    struct public_key pub_key_temp;
    pub_key_temp.n = 3;
    pub_key_temp.e = 9;
    _push("Busanerhwnaskjdt", C, 4.0, pub_key_temp);
    oh_queue.stats.no_of_people_in_queue--;

    const char *student_name = "So Zhang";
    const enum subject subject = Other;
    const float questionNumber = 8;
    struct public_key pub_key;
    pub_key.n = 11;
    pub_key.e = 5;
    
    int retVal = push(student_name, subject, questionNumber, pub_key);

    ck_assert_msg(retVal == SUCCESS, "Expected return value to be SUCCESS (%d) when inserting into empty users, got %d", SUCCESS, retVal);

    ck_assert_msg(oh_queue.stats.no_of_people_in_queue == 1, "Expected no_of_people_in_queue to be 1 after inserting into empty users, was %d", oh_queue.stats.no_of_people_in_queue);
    ck_assert_msg(strncmp(oh_queue.students[0].studentData.name, student_name, MAX_NAME_LENGTH) == 0, "Expected name to be %s, was %s", student_name, oh_queue.students[0].studentData.name);
    ck_assert_msg(oh_queue.students[0].studentData.topic.topicName == subject, "Expected topicName to be %d, was %d", subject, oh_queue.students[0].studentData.topic.topicName);
    int expected_id[30];
    _hash(expected_id, (char *) student_name, pub_key);
    for (int i = 0; i < (int) strlen(student_name); i++) {
        ck_assert_int_eq(oh_queue.students[0].customID[i], expected_id[i]);
    }
    ck_assert_msg(oh_queue.students[0].studentData.topic.questionNumber == questionNumber, "Expected questionNumber to be %f, was %f", questionNumber, oh_queue.students[0].studentData.topic.questionNumber);
    ck_assert_msg(oh_queue.students[0].queue_number == 0, "Expected queue_number to be 0, was %d", oh_queue.students[0].queue_number);

}
END_TEST

START_TEST(test_push_long_name)
{
    const char *student_name = "Prabhav Prabhav Prabhav PrabhavD";
    const char *truncated_name = "Prabhav Prabhav Prabhav Prabh";
    const enum subject subject = C;
    const float questionNumber = 76;
    struct public_key pub_key;
    pub_key.n = 11;
    pub_key.e = 6;
    
    int retVal = push(student_name, subject, questionNumber, pub_key);

    ck_assert_msg(retVal == SUCCESS, "Expected return value to be SUCCESS (%d) when inserting into empty users, got %d", SUCCESS, retVal);

    ck_assert_msg(oh_queue.stats.no_of_people_in_queue == 1, "Expected no_of_people_in_queue to be 1 after inserting into empty users, was %d", oh_queue.stats.no_of_people_in_queue);
    ck_assert_msg(strncmp(oh_queue.students[0].studentData.name, truncated_name, MAX_NAME_LENGTH) == 0, "Expected name to be %s, was %s", truncated_name, oh_queue.students[0].studentData.name);
    ck_assert_msg(oh_queue.students[0].studentData.topic.topicName == subject, "Expected topicName to be %d, was %d", subject, oh_queue.students[0].studentData.topic.topicName);
    int expected_id[30];
    _hash(expected_id, (char *) truncated_name, pub_key);
    for (int i = 0; i < (int) strlen(truncated_name); i++) {
        ck_assert_int_eq(oh_queue.students[0].customID[i], expected_id[i]);
    }
    ck_assert_msg(oh_queue.students[0].studentData.topic.questionNumber == questionNumber, "Expected questionNumber to be %f, was %f", questionNumber, oh_queue.students[0].studentData.topic.questionNumber);
    ck_assert_msg(oh_queue.students[0].queue_number == 0, "Expected queue_number to be 0, was %d", oh_queue.students[0].queue_number);

}
END_TEST

START_TEST(test_push_too_many_students)
{
    for (int i = 0; i < MAX_QUEUE_LENGTH; i++) {
        char student_name[10];
        sprintf(student_name, "st%d", i);
        enum subject subject = LC3;
        float questionNumber = 4 * i * i;
        struct public_key pub_key;
        pub_key.n = i + 2;
        pub_key.e = i + 4;
        
        _push(student_name, subject, questionNumber, pub_key);
    }

    struct public_key pub_key_temp;
    pub_key_temp.n = 7;
    pub_key_temp.e = 3;
    // should exceed capacity
    int retVal = push("One Mohr", LC3, 2.0, pub_key_temp);
    ck_assert_int_eq(retVal, FAILURE);

    ck_assert_msg(oh_queue.stats.no_of_people_in_queue == MAX_QUEUE_LENGTH, "Expected no_of_people_in_queue to be %d after pushing %d users, was %d", MAX_QUEUE_LENGTH, MAX_QUEUE_LENGTH, oh_queue.stats.no_of_people_in_queue);

    // Students in queue should be the same even after failure
    for (int i = 0; i < MAX_QUEUE_LENGTH; i++) {
        char student_name[10];
        sprintf(student_name, "st%d", i);
        enum subject subject = LC3;
        float questionNumber = 4 * i * i;
        struct public_key pub_key;
        pub_key.n = i + 2;
        pub_key.e = i + 4;

        ck_assert_msg(oh_queue.students[i].studentData.topic.topicName == subject, "Expected topicName to be %d, was %d", subject, oh_queue.students[i].studentData.topic.topicName);
        ck_assert_msg(strncmp(oh_queue.students[i].studentData.name, student_name, MAX_NAME_LENGTH) == 0, "Expected name to be %s, was %s", student_name, oh_queue.students[i].studentData.name);
        int expected_id[30];
        _hash(expected_id, (char *) student_name, pub_key);
        for (int j = 0; j < (int) strlen(student_name); j++) {
            ck_assert_int_eq(oh_queue.students[i].customID[j], expected_id[j]);
        }
        ck_assert_msg(oh_queue.students[i].studentData.topic.questionNumber == questionNumber, "Expected questionNumber to be %f, was %f", questionNumber, oh_queue.students[i].studentData.topic.questionNumber);
        ck_assert_msg(oh_queue.students[i].queue_number == i, "Expected queue_number to be %d, was %d", i, oh_queue.students[i].queue_number);
    }
}
END_TEST

/* pop() tests */
START_TEST(test_pop_basic)
{
    for (int i = 0; i < 5; i++) {
        char student_name[10];
        sprintf(student_name, "sai%d", i);
        enum subject subject = Assembly;
        float questionNumber = 3 + i;
        struct public_key pub_key;
        pub_key.n = i + 2;
        pub_key.e = i + 3;
        
        _push(student_name, subject, questionNumber, pub_key);
    }

    int retVal = pop();

    ck_assert_int_eq(retVal, SUCCESS);
    
    ck_assert_msg(oh_queue.stats.no_of_people_visited == 1, "Expected no_of_people_visited to be 1 after pushing 5 users and removing 1, was %d", oh_queue.stats.no_of_people_visited);
    ck_assert_msg(oh_queue.stats.no_of_people_in_queue == 4, "Expected no_of_people_in_queue to be 4 after pushing 5 users and removing 1, was %d", oh_queue.stats.no_of_people_in_queue);
    for (int i = 1; i < 5; i++) {
        char student_name[10];
        sprintf(student_name, "sai%d", i);
        enum subject subject = Assembly;
        float questionNumber = 3 + i;
        struct public_key pub_key;
        pub_key.n = i + 2;
        pub_key.e = i + 3;

        i -= 1;
        ck_assert_msg(oh_queue.students[i].studentData.topic.topicName == subject, "Expected topicName to be %d, was %d", subject, oh_queue.students[i].studentData.topic.topicName);
        ck_assert_msg(strncmp(oh_queue.students[i].studentData.name, student_name, MAX_NAME_LENGTH) == 0, "Expected name to be %s, was %s", student_name, oh_queue.students[i].studentData.name);
        int expected_id[30];
        _hash(expected_id, (char *) student_name, pub_key);
        for (int j = 0; j < (int) strlen(student_name); j++) {
            ck_assert_int_eq(oh_queue.students[i].customID[j], expected_id[j]);
        }
        ck_assert_msg(oh_queue.students[i].studentData.topic.questionNumber == questionNumber, "Expected questionNumber to be %f, was %f", questionNumber, oh_queue.students[i].studentData.topic.questionNumber);
        ck_assert_msg(oh_queue.students[i].queue_number == i + 1, "Expected queue_number to be %d, was %d", i + 1, oh_queue.students[i].queue_number);
        i++;
    }}
END_TEST

START_TEST(test_pop_single_student)
{
    char student_name[10] = "richard";
    enum subject subject = Other;
    float questionNumber = 3;
    struct public_key pub_key;
    pub_key.n = 2;
    pub_key.e = 5;
    
    _push(student_name, subject, questionNumber, pub_key);

    int retVal = pop();

    ck_assert_int_eq(retVal, SUCCESS);
    
    ck_assert_msg(oh_queue.stats.no_of_people_visited == 1, "Expected no_of_people_visited to be 1 after pushing 1 user and removing 1, was %d", oh_queue.stats.no_of_people_visited);
    ck_assert_msg(oh_queue.stats.no_of_people_in_queue == 0, "Expected no_of_people_in_queue to be 0 after pushing 1 user and removing 1, was %d", oh_queue.stats.no_of_people_in_queue);
}
END_TEST

START_TEST(test_pop_no_students)
{
    int retVal = pop();

    ck_assert_int_eq(retVal, FAILURE);
    
    ck_assert_int_eq(oh_queue.stats.no_of_people_visited, 0);
    ck_assert_int_eq(oh_queue.stats.no_of_people_in_queue, 0);
}
END_TEST

START_TEST(test_pop_multiple)
{
    for (int i = 0; i < 10; i++) {
        char student_name[20];
        sprintf(student_name, "2110 student%d", i);
        enum subject subject = Assembly;
        float questionNumber = 3 + i;
        struct public_key pub_key;
        pub_key.n = i + 2;
        pub_key.e = i + 3;
        
        _push(student_name, subject, questionNumber, pub_key);
    }

    int retVal1 = pop();
    int retVal2 = pop();
    int retVal3 = pop();
    int retVal4 = pop();
    ck_assert_int_eq(retVal1, SUCCESS);
    ck_assert_int_eq(retVal2, SUCCESS);
    ck_assert_int_eq(retVal3, SUCCESS);
    ck_assert_int_eq(retVal4, SUCCESS);
    
    ck_assert_msg(oh_queue.stats.no_of_people_visited == 4, "Expected no_of_people_visited to be 4 after pushing 10 users and removing 4, was %d", oh_queue.stats.no_of_people_visited);
    ck_assert_msg(oh_queue.stats.no_of_people_in_queue == 6, "Expected no_of_people_in_queue to be 6 after pushing 10 users and removing 4, was %d", oh_queue.stats.no_of_people_in_queue);
    for (int i = 4; i < 10; i++) {
        char student_name[20];
        sprintf(student_name, "2110 student%d", i);
        enum subject subject = Assembly;
        float questionNumber = 3 + i;
        struct public_key pub_key;
        pub_key.n = i + 2;
        pub_key.e = i + 3;

        i -= 4;
        ck_assert_msg(oh_queue.students[i].studentData.topic.topicName == subject, "Expected topicName to be %d, was %d", subject, oh_queue.students[i].studentData.topic.topicName);
        ck_assert_msg(strncmp(oh_queue.students[i].studentData.name, student_name, MAX_NAME_LENGTH) == 0, "Expected name to be %s, was %s", student_name, oh_queue.students[i].studentData.name);
        int expected_id[30];
        _hash(expected_id, (char *) student_name, pub_key);
        for (int j = 0; j < (int) strlen(student_name); j++) {
            ck_assert_int_eq(oh_queue.students[i].customID[j], expected_id[j]);
        }
        ck_assert_msg(oh_queue.students[i].studentData.topic.questionNumber == questionNumber, "Expected questionNumber to be %f, was %f", questionNumber, oh_queue.students[i].studentData.topic.questionNumber);
        ck_assert_msg(oh_queue.students[i].queue_number == i + 4, "Expected queue_number to be %d, was %d", i, oh_queue.students[i].queue_number);
        i += 4;
    }}
END_TEST

START_TEST(test_pop_everything)
{
    for (int i = 0; i < 10; i++) {
        char student_name[20];
        sprintf(student_name, "2110 student%d", i);
        enum subject subject = Assembly;
        float questionNumber = 3 + i;
        struct public_key pub_key;
        pub_key.n = i + 2;
        pub_key.e = i + 3;
        
        _push(student_name, subject, questionNumber, pub_key);
    }
    for (int i = 0; i < 10; i++) {
        int retVal = pop();
        ck_assert_int_eq(retVal, SUCCESS);
    }
    
    ck_assert_msg(oh_queue.stats.no_of_people_visited == 10, "Expected no_of_people_visited to be 10 after pushing 10 users and popping 10, was %d", oh_queue.stats.no_of_people_visited);
    ck_assert_msg(oh_queue.stats.no_of_people_in_queue == 0, "Expected no_of_people_in_queue to be 0 after pushing 10 users and popping 10, was %d", oh_queue.stats.no_of_people_in_queue);
}
END_TEST

/* group_by_topic() tests */
START_TEST(test_group_by_topic_basic)
{
    const char *student_name1 = "Jason";
    const enum subject subject1 = Other;
    const float questionNumber1 = 45;
    struct public_key pub_key1;
    pub_key1.n = 5;
    pub_key1.e = 6;
    _push(student_name1, subject1, questionNumber1, pub_key1);

    const char *student_name2 = "Jason2";
    const enum subject subject2 = LC3;
    const float questionNumber2 = 27;
    struct public_key pub_key2;
    pub_key2.n = 5;
    pub_key2.e = 7;
    _push(student_name2, subject2, questionNumber2, pub_key2);


    struct Student *group[10];

    struct Topic other_topic = {Other, 45};

    int retVal = group_by_topic(other_topic, group);

    ck_assert_msg(retVal == 1, "Expected the number of students with topic {Other, 45} to be 1, was %d", retVal);

    ck_assert_msg(group[0]->studentData.topic.topicName == subject1, "Expected topicName to be %d, was %d", subject1, group[0]->studentData.topic.topicName);
    ck_assert_msg(strncmp(group[0]->studentData.name, student_name1, MAX_NAME_LENGTH) == 0, "Expected name to be %s, was %s", student_name1, group[0]->studentData.name);

    int expected_id[30];
    _hash(expected_id, (char *) student_name1, pub_key1);
    for (int i = 0; i < (int) strlen(student_name1); i++) {
        ck_assert_int_eq(group[0]->customID[i], expected_id[i]);
    }
    ck_assert_msg(group[0]->studentData.topic.questionNumber == questionNumber1, "Expected questionNumber to be %f, was %f", questionNumber1, group[0]->studentData.topic.questionNumber);
    ck_assert_msg(group[0]->queue_number == 0, "Expected queue_number to be 0 was %d", group[0]->queue_number);

    // Queue should not change
    ck_assert_msg(oh_queue.stats.no_of_people_in_queue == 2, "Expected no_of_people_in_queue to be 2 after inserting into users, was %d", oh_queue.stats.no_of_people_in_queue);

    ck_assert_msg(oh_queue.students[0].studentData.topic.topicName == subject1, "Expected topicName to be %d, was %d", subject1, oh_queue.students[0].studentData.topic.topicName);
    ck_assert_msg(strncmp(oh_queue.students[0].studentData.name, student_name1, MAX_NAME_LENGTH) == 0, "Expected name to be %s, was %s", student_name1, oh_queue.students[0].studentData.name);

    _hash(expected_id, (char *) student_name1, pub_key1);
    for (int i = 0; i < (int) strlen(student_name1); i++) {
        ck_assert_int_eq(oh_queue.students[0].customID[i], expected_id[i]);
    }
    ck_assert_msg(oh_queue.students[0].studentData.topic.questionNumber == questionNumber1, "Expected questionNumber to be %f, was %f", questionNumber1, oh_queue.students[0].studentData.topic.questionNumber);
    ck_assert_msg(oh_queue.students[0].queue_number == 0, "Expected queue_number to be 0 was %d", oh_queue.students[0].queue_number);

    ck_assert_msg(oh_queue.students[1].studentData.topic.topicName == subject2, "Expected topicName to be %d, was %d", subject2, oh_queue.students[0].studentData.topic.topicName);
    ck_assert_msg(strncmp(oh_queue.students[1].studentData.name, student_name2, MAX_NAME_LENGTH) == 0, "Expected name to be %s, was %s", student_name2, oh_queue.students[0].studentData.name);

    _hash(expected_id, (char *) student_name2, pub_key2);
    for (int i = 0; i < (int) strlen(student_name2); i++) {
        ck_assert_int_eq(oh_queue.students[1].customID[i], expected_id[i]);
    }
    ck_assert_msg(oh_queue.students[1].studentData.topic.questionNumber == questionNumber2, "Expected questionNumber to be %f, was %f", questionNumber2, oh_queue.students[0].studentData.topic.questionNumber);
    ck_assert_msg(oh_queue.students[1].queue_number == 1, "Expected queue_number to be 1 was %d", oh_queue.students[0].queue_number);
}
END_TEST

START_TEST(test_group_by_topic_different_topic_name_same_number)
{
    for (int i = 0; i < 5; i++) {
        char student_name[10];
        sprintf(student_name, "saif%d", i);
        enum subject subject;
        float questionNumber = 2;
        if (i % 2 == 0) {
            subject = Other;
        } else {
            subject = C;
        }
        
        struct public_key pub_key;
        pub_key.n = i + 2;
        pub_key.e = i + 3;
        
        _push(student_name, subject, questionNumber, pub_key);
    }

    struct Student *group[10];

    struct Topic other_topic = {Other, 2};

    int retVal = group_by_topic(other_topic, group);

    ck_assert_msg(retVal == 3, "Expected the number of students with topic {Other, 2} to be 3, was %d", retVal);
    
    for (int i = 0; i < retVal; i ++) {
        ck_assert_msg(group[i]->studentData.topic.topicName == other_topic.topicName, "Expected the topicName to be %d after grouping, was %d", other_topic.topicName, group[i]->studentData.topic.topicName);
        ck_assert_msg(group[i]->studentData.topic.questionNumber == other_topic.questionNumber, "Expected the questionNumber to be %f after grouping, was %f", other_topic.questionNumber, group[i]->studentData.topic.questionNumber);
    }

    // queue should be unchanged
    ck_assert_msg(oh_queue.stats.no_of_people_in_queue == 5, "Expected no_of_people_in_queue to be 5, was %d", oh_queue.stats.no_of_people_in_queue);
    for (int i = 0; i < 5; i++) {
        char student_name[10];
        sprintf(student_name, "saif%d", i);
        enum subject subject;
        float questionNumber = 2;
        if (i % 2 == 0) {
            subject = Other;
        } else {
            subject = C;
        }
        struct public_key pub_key;
        pub_key.n = i + 2;
        pub_key.e = i + 3;

        ck_assert_msg(oh_queue.students[i].studentData.topic.topicName == subject, "Expected topicName to be %d, was %d", subject, oh_queue.students[i].studentData.topic.topicName);
        ck_assert_msg(strncmp(oh_queue.students[i].studentData.name, student_name, MAX_NAME_LENGTH) == 0, "Expected name to be %s, was %s", student_name, oh_queue.students[i].studentData.name);
        int expected_id[30];
        _hash(expected_id, (char *) student_name, pub_key);
        for (int j = 0; j < (int) strlen(student_name); j++) {
            ck_assert_int_eq(oh_queue.students[i].customID[j], expected_id[j]);
        }
        ck_assert_msg(oh_queue.students[i].studentData.topic.questionNumber == questionNumber, "Expected questionNumber to be %f, was %f", questionNumber, oh_queue.students[i].studentData.topic.questionNumber);
        ck_assert_msg(oh_queue.students[i].queue_number == i, "Expected queue_number to be %d, was %d", i, oh_queue.students[i].queue_number);
    }}
END_TEST

START_TEST(test_group_by_topic_same_topic_different_number)
{
    for (int i = 0; i < 8; i++) {
        char student_name[10];
        sprintf(student_name, "saul%d", i);
        enum subject subject = Assembly;
        float questionNumber;
        if (i % 2 == 0) {
            questionNumber = 5;
        } else {
            questionNumber = 7;
        }
        
        struct public_key pub_key;
        pub_key.n = i + 7;
        pub_key.e = i + 10;
        
        _push(student_name, subject, questionNumber, pub_key);
    }

    struct Student *group[10];

    struct Topic other_topic = {Assembly, 7};

    int retVal = group_by_topic(other_topic, group);

    ck_assert_msg(retVal == 4, "Expected the number of students with topic {Assembly, 7} to be 4, was %d", retVal);
    
    for (int i = 0; i < retVal; i ++) {
        ck_assert_msg(group[i]->studentData.topic.topicName == other_topic.topicName, "Expected the topicName to be %d after grouping, was %d", other_topic.topicName, group[i]->studentData.topic.topicName);
        ck_assert_msg(group[i]->studentData.topic.questionNumber == other_topic.questionNumber, "Expected the questionNumber to be %f after grouping, was %f", other_topic.questionNumber, group[i]->studentData.topic.questionNumber);
    }

    // queue should be unchanged
    ck_assert_msg(oh_queue.stats.no_of_people_in_queue == 8, "Expected no_of_people_in_queue to be 8, was %d", oh_queue.stats.no_of_people_in_queue);
    for (int i = 0; i < 8; i++) {
        char student_name[10];
        sprintf(student_name, "saul%d", i);
        enum subject subject = Assembly;
        float questionNumber;
        if (i % 2 == 0) {
            questionNumber = 5;
        } else {
            questionNumber = 7;
        }
        
        struct public_key pub_key;
        pub_key.n = i + 7;
        pub_key.e = i + 10;

        ck_assert_msg(oh_queue.students[i].studentData.topic.topicName == subject, "Expected topicName to be %d, was %d", subject, oh_queue.students[i].studentData.topic.topicName);
        ck_assert_msg(strncmp(oh_queue.students[i].studentData.name, student_name, MAX_NAME_LENGTH) == 0, "Expected name to be %s, was %s", student_name, oh_queue.students[i].studentData.name);
        int expected_id[30];
        _hash(expected_id, (char *) student_name, pub_key);
        for (int j = 0; j < (int) strlen(student_name); j++) {
            ck_assert_int_eq(oh_queue.students[i].customID[j], expected_id[j]);
        }
        ck_assert_msg(oh_queue.students[i].studentData.topic.questionNumber == questionNumber, "Expected questionNumber to be %f, was %f", questionNumber, oh_queue.students[i].studentData.topic.questionNumber);
        ck_assert_msg(oh_queue.students[i].queue_number == i, "Expected queue_number to be %d, was %d", i, oh_queue.students[i].queue_number);
    }}
END_TEST

START_TEST(test_group_by_topic_no_student_with_topic)
{
    for (int i = 0; i < 15; i++) {
        char student_name[10];
        sprintf(student_name, "stud%d", i);
        enum subject subject;
        float questionNumber;
        if (i % 2 == 0) {
            questionNumber = 5;
            subject = LC3;
        } else {
            questionNumber = 9;
            subject = C;
        }
        
        struct public_key pub_key;
        pub_key.n = i + 9;
        pub_key.e = i + 10;
        
        _push(student_name, subject, questionNumber, pub_key);
    }

    struct Student *group[10];

    struct Topic other_topic = {Assembly, 2};

    int retVal = group_by_topic(other_topic, group);

    ck_assert_msg(retVal == 0, "Expected the number of students with topic {Assembly, 2} to be 0, was %d", retVal);
    
    // queue should be unchanged
    ck_assert_msg(oh_queue.stats.no_of_people_in_queue == 15, "Expected no_of_people_in_queue to be 15, was %d", oh_queue.stats.no_of_people_in_queue);
    for (int i = 0; i < 15; i++) {
        char student_name[10];
        sprintf(student_name, "stud%d", i);
        enum subject subject;
        float questionNumber;
        if (i % 2 == 0) {
            questionNumber = 5;
            subject = LC3;
        } else {
            questionNumber = 9;
            subject = C;
        }
        
        struct public_key pub_key;
        pub_key.n = i + 9;
        pub_key.e = i + 10;

        ck_assert_msg(oh_queue.students[i].studentData.topic.topicName == subject, "Expected topicName to be %d, was %d", subject, oh_queue.students[i].studentData.topic.topicName);
        ck_assert_msg(strncmp(oh_queue.students[i].studentData.name, student_name, MAX_NAME_LENGTH) == 0, "Expected name to be %s, was %s", student_name, oh_queue.students[i].studentData.name);
        int expected_id[30];
        _hash(expected_id, (char *) student_name, pub_key);
        for (int j = 0; j < (int) strlen(student_name); j++) {
            ck_assert_int_eq(oh_queue.students[i].customID[j], expected_id[j]);
        }
        ck_assert_msg(oh_queue.students[i].studentData.topic.questionNumber == questionNumber, "Expected questionNumber to be %f, was %f", questionNumber, oh_queue.students[i].studentData.topic.questionNumber);
        ck_assert_msg(oh_queue.students[i].queue_number == i, "Expected queue_number to be %d, was %d", i, oh_queue.students[i].queue_number);
    }}
END_TEST

START_TEST(test_group_by_topic_empty_array) {
    char *student_name = "George Burdell 15";
    enum subject subject = LC3;
    float questionNumber = 9;
    struct public_key pub_key;
    pub_key.n = 9;
    pub_key.e = 4;
    
    _push(student_name, subject, questionNumber, pub_key);
    oh_queue.stats.no_of_people_in_queue -= 1;
    // The pushed entry is no longer in the array
    
    struct Student *group[10];
    struct Topic target_topic = {LC3, 9};
    int retVal = group_by_topic(target_topic, group);

    ck_assert_msg(retVal == 0, "Expected the number of students with topic {Assembly, 2} to be 0, was %d", retVal);
    ck_assert_int_eq(oh_queue.stats.no_of_people_in_queue, 0);
}
END_TEST

/* encrypt() tests */
START_TEST(test_hash_1) {
    char *string = "Mint chocolate chip is a bad flavor.";
    struct public_key pub_key = {1333, 97};
    const int length = 36;
    int ciphertext[36];
    int actual[36];

    hash(ciphertext, string, pub_key);
    _hash(actual, string, pub_key);

    for (int i = 0; i < length; i++) {
        ck_assert_int_eq(ciphertext[i], actual[i]);
    }
}
END_TEST

START_TEST(test_hash_2) {
    char *string = "Cowboys don't belong anywhere near a Super Bowl.";
    const struct public_key pub_key = {526567, 65537};
    const int length = 48;
    int ciphertext[48];
    int expected[48];

    hash(ciphertext, string, pub_key);
    _hash(expected, string, pub_key);

  for (int i = 0; i < length; i++) {
    ck_assert_int_eq(ciphertext[i], expected[i]);
  }
}
END_TEST

/* update_student() tests */
START_TEST(test_update_student_basic) {
    char studentNames[][20] = {
        "David", "David"
    };
    enum subject topicNames[] = {
        LC3, Assembly
    };
    float questionNumbers[] = {
        56, 39
    };
    struct public_key pub_keys[] = {
        {143, 23},
        {8, 3}
    };

    for (int i = 0; i < 2; i ++) {
        _push(studentNames[i], topicNames[i], questionNumbers[i], pub_keys[i]);
    }

    struct Topic new_topic = {C, 3};
    // regular update test
    int targetCustomID[10];
    _hash(targetCustomID, studentNames[0], pub_keys[0]);
    int retVal1 = update_student(new_topic, targetCustomID);

    // The id is the only part of the struct that actually matters when we're searching
    // Make sure the students aren't checking using the whole struct

    // mess up user[1] besides the ID
    struct Student *student = &oh_queue.students[1];
    student->queue_number -= 2;
    _hash(targetCustomID, studentNames[1], pub_keys[1]);
    new_topic.topicName = LC3;
    new_topic.questionNumber = 7;
    int retVal2 = update_student(new_topic, targetCustomID);
    student->queue_number += 2;
    ck_assert_int_eq(retVal1, SUCCESS);
    ck_assert_int_eq(retVal2, SUCCESS);

    // queue should not change when updating (except topic)
    ck_assert_int_eq(oh_queue.stats.no_of_people_in_queue, 2);
    struct Topic new_topics[] = {
        {C, 3},
        {LC3, 7}
    };
    for (int i = 0; i < 2; i++) {
        ck_assert_msg(oh_queue.students[i].studentData.topic.topicName == new_topics[i].topicName, "Expected topicName to be %d, was %d", new_topics[i].topicName, oh_queue.students[i].studentData.topic.topicName);
        ck_assert_msg(strncmp(oh_queue.students[i].studentData.name, studentNames[i], MAX_NAME_LENGTH) == 0, "Expected name to be %s, was %s", studentNames[i], oh_queue.students[i].studentData.name);
        int expected_id[30];
        _hash(expected_id, (char *) studentNames[i], pub_keys[i]);
        for (int j = 0; j < (int) strlen(studentNames[i]); j++) {
            ck_assert_int_eq(oh_queue.students[i].customID[j], expected_id[j]);
        }
        ck_assert_msg(oh_queue.students[i].studentData.topic.questionNumber == new_topics[i].questionNumber, "Expected questionNumber to be %f, was %f", new_topics[i].questionNumber, oh_queue.students[i].studentData.topic.questionNumber);
        ck_assert_msg(oh_queue.students[i].queue_number == i, "Expected queue_number to be %d, was %d", i, oh_queue.students[i].queue_number);
    }
}
END_TEST

/* update_student() tests */
START_TEST(test_update_student_no_student_with_custom_id) {
    char studentNames[][20] = {
        "David", "Bob", "Julie", "Dan", "Caleb"
    };
    enum subject topicNames[] = {
        LC3, Assembly, LC3, C, LC3
    };
    float questionNumbers[] = {
        56, 39, 21, 34, 3
    };
    struct public_key pub_keys[] = {
        {143, 23},
        {8, 3},
        {9, 3},
        {9, 2},
        {6, 3},
    };

    for (int i = 0; i < 5; i ++) {
        _push(studentNames[i], topicNames[i], questionNumbers[i], pub_keys[i]);
    }

    struct Topic new_topic = {C, 7};
    // update test
    int targetCustomID[10];
    struct public_key unused_pub_key = {132, 4};
    _hash(targetCustomID, "Non existent", unused_pub_key);
    int retVal1 = update_student(new_topic, targetCustomID);

    ck_assert_int_eq(retVal1, FAILURE);
    // queue should not change 
    ck_assert_int_eq(oh_queue.stats.no_of_people_in_queue, 5);

    for (int i = 0; i < 5; i++) {
        ck_assert_msg(oh_queue.students[i].studentData.topic.topicName == topicNames[i], "Expected topicName to be %d, was %d", new_topic.topicName, oh_queue.students[i].studentData.topic.topicName);
        ck_assert_msg(strncmp(oh_queue.students[i].studentData.name, studentNames[i], MAX_NAME_LENGTH) == 0, "Expected name to be %s, was %s", studentNames[i], oh_queue.students[i].studentData.name);
        int expected_id[30];
        _hash(expected_id, (char *) studentNames[i], pub_keys[i]);
        for (int j = 0; j < (int) strlen(studentNames[i]); j++) {
            ck_assert_int_eq(oh_queue.students[i].customID[j], expected_id[j]);
        }
        ck_assert_msg(oh_queue.students[i].studentData.topic.questionNumber == questionNumbers[i], "Expected questionNumber to be %f, was %f", new_topic.questionNumber, oh_queue.students[i].studentData.topic.questionNumber);
        ck_assert_msg(oh_queue.students[i].queue_number == i, "Expected queue_number to be %d, was %d", i, oh_queue.students[i].queue_number);
    }
}
END_TEST

START_TEST(test_update_student_empty_array) {
    char *student_name = "George Burdell 2";
    enum subject subject = Assembly;
    float questionNumber = 6;
    struct public_key pub_key;
    pub_key.n = 9;
    pub_key.e = 4;
    
    _push(student_name, subject, questionNumber, pub_key);
    oh_queue.stats.no_of_people_in_queue -= 1;
    // The pushed entry is no longer in the array
    struct Topic new_topic = {Assembly, 6};
    int targetCustomID[20];
    _hash(targetCustomID, student_name, pub_key);
    int retVal1 = update_student(new_topic, targetCustomID);

    ck_assert_int_eq(retVal1, FAILURE);
    ck_assert_int_eq(oh_queue.stats.no_of_people_in_queue, 0);
}
END_TEST

/* remove_student_by_name() tests */
START_TEST(test_remove_student_by_name_basic) {
    char studentNames[][20] = {
        "David", "Charlie", "James"
    };
    enum subject topicNames[] = {
        LC3, C, C
    };
    float questionNumbers[] = {
        47, 39, 47
    };
    struct public_key pub_keys[] = {
        {143, 23},
        {8, 3},
        {78, 12}
    };

    for (int i = 0; i < 3; i ++) {
        _push(studentNames[i], topicNames[i], questionNumbers[i], pub_keys[i]);
    }

   int retVal = remove_student_by_name("Charlie");

   ck_assert_int_eq(retVal, SUCCESS);

    // charlie should be removed
    ck_assert_int_eq(oh_queue.stats.no_of_people_in_queue, 2);
    ck_assert_int_eq(oh_queue.stats.no_of_people_visited, 1);
    
    // new student names, topicNames, etc.
    char expectedStudentNames[][20] = {
        "David", "James"
    };
    enum subject expectedTopicNames[] = {
        LC3, C
    };
    float expectedQuestionNumbers[] = {
        47, 47
    };
    struct public_key expectedPubKeys[] = {
        {143, 23},
        {78, 12}
    };
    int expectedQueueNumbers[] = {
        0, 2
    };

    for (int i = 0; i < 2; i++) {
        ck_assert_msg(oh_queue.students[i].studentData.topic.topicName == expectedTopicNames[i], "Expected topicName to be %d, was %d", expectedTopicNames[i], oh_queue.students[i].studentData.topic.topicName);
        ck_assert_msg(strncmp(oh_queue.students[i].studentData.name, expectedStudentNames[i], MAX_NAME_LENGTH) == 0, "Expected name to be %s, was %s", expectedStudentNames[i], oh_queue.students[i].studentData.name);
        int expected_id[30];
        _hash(expected_id, (char *) expectedStudentNames[i], expectedPubKeys[i]);
        for (int j = 0; j < (int) strlen(expectedStudentNames[i]); j++) {
            ck_assert_int_eq(oh_queue.students[i].customID[j], expected_id[j]);
        }
        ck_assert_msg(oh_queue.students[i].studentData.topic.questionNumber == expectedQuestionNumbers[i], "Expected questionNumber to be %f, was %f", expectedQuestionNumbers[i], oh_queue.students[i].studentData.topic.questionNumber);
        ck_assert_msg(oh_queue.students[i].queue_number == expectedQueueNumbers[i], "Expected queue_number to be %d, was %d", expectedQueueNumbers[i], oh_queue.students[i].queue_number);
    }
}
END_TEST

START_TEST(test_remove_student_by_name_no_students_with_name) {
    char studentNames[][20] = {
        "David", "Eddy", "James", "Eddy"
    };
    enum subject topicNames[] = {
        LC3, Assembly, Assembly, Other
    };
    float questionNumbers[] = {
        47, 2, 47, 20
    };
    struct public_key pub_keys[] = {
        {143, 23},
        {8, 3},
        {78, 12},
        {86, 12}
    };

    for (int i = 0; i < 4; i ++) {
        _push(studentNames[i], topicNames[i], questionNumbers[i], pub_keys[i]);
    }

    int retVal = remove_student_by_name("Eddy");

    ck_assert_int_eq(retVal, SUCCESS);

    // only one eddy should be removed
    ck_assert_int_eq(oh_queue.stats.no_of_people_in_queue, 3);
    ck_assert_int_eq(oh_queue.stats.no_of_people_visited, 1);
    
    // new student names, topicNames, etc.
    char expectedStudentNames[][20] = {
        "David", "James", "Eddy"
    };
    enum subject expectedTopicNames[] = {
        LC3, Assembly, Other
    };
    float expectedQuestionNumbers[] = {
        47, 47, 20
    };
    struct public_key expectedPubKeys[] = {
        {143, 23},
        {78, 12},
        {86, 12}
    };
    int expectedQueueNumbers[] = {
        0, 2, 3
    };

    for (int i = 0; i < 3; i++) {
        ck_assert_msg(oh_queue.students[i].studentData.topic.topicName == expectedTopicNames[i], "Expected topicName to be %d, was %d", expectedTopicNames[i], oh_queue.students[i].studentData.topic.topicName);
        ck_assert_msg(strncmp(oh_queue.students[i].studentData.name, expectedStudentNames[i], MAX_NAME_LENGTH) == 0, "Expected name to be %s, was %s", expectedStudentNames[i], oh_queue.students[i].studentData.name);
        int expected_id[30];
        _hash(expected_id, (char *) expectedStudentNames[i], expectedPubKeys[i]);
        for (int j = 0; j < (int) strlen(expectedStudentNames[i]); j++) {
            ck_assert_int_eq(oh_queue.students[i].customID[j], expected_id[j]);
        }
        ck_assert_msg(oh_queue.students[i].studentData.topic.questionNumber == expectedQuestionNumbers[i], "Expected questionNumber to be %f, was %f", expectedQuestionNumbers[i], oh_queue.students[i].studentData.topic.questionNumber);
        ck_assert_msg(oh_queue.students[i].queue_number == expectedQueueNumbers[i], "Expected queue_number to be %d, was %d", expectedQueueNumbers[i], oh_queue.students[i].queue_number);
    }
}
END_TEST

START_TEST(test_remove_student_by_name_empty_array) {
    char *student_name = "George Burdell 3";
    enum subject subject = Assembly;
    float questionNumber = 6;
    struct public_key pub_key;
    pub_key.n = 9;
    pub_key.e = 4;
    
    _push(student_name, subject, questionNumber, pub_key);
    oh_queue.stats.no_of_people_in_queue -= 1;
    // The pushed entry is no longer in the array
    int retVal1 = remove_student_by_name("George Burdell 3");

    ck_assert_int_eq(retVal1, FAILURE);
    ck_assert_int_eq(oh_queue.stats.no_of_people_in_queue, 0);
}
END_TEST

START_TEST(test_remove_student_by_name_multiple) {
    char studentNames[][20] = {
        "David", "Eddy", "James", "Eddy"
    };
    enum subject topicNames[] = {
        LC3, Assembly, Assembly, Other
    };
    float questionNumbers[] = {
        47, 2, 47, 20
    };
    struct public_key pub_keys[] = {
        {143, 23},
        {8, 3},
        {78, 12},
        {86, 12}
    };

    for (int i = 0; i < 4; i ++) {
        _push(studentNames[i], topicNames[i], questionNumbers[i], pub_keys[i]);
    }

    int retVal = remove_student_by_name("Eddy");

    ck_assert_int_eq(retVal, SUCCESS);

    // only one eddy should be removed
    ck_assert_int_eq(oh_queue.stats.no_of_people_in_queue, 3);
    ck_assert_int_eq(oh_queue.stats.no_of_people_visited, 1);
    
    // new student names, topicNames, etc.
    char expectedStudentNames[][20] = {
        "David", "James", "Eddy"
    };
    enum subject expectedTopicNames[] = {
        LC3, Assembly, Other
    };
    float expectedQuestionNumbers[] = {
        47, 47, 20
    };
    struct public_key expectedPubKeys[] = {
        {143, 23},
        {78, 12},
        {86, 12}
    };
    int expectedQueueNumbers[] = {
        0, 2, 3
    };

    for (int i = 0; i < 3; i++) {
        ck_assert_msg(oh_queue.students[i].studentData.topic.topicName == expectedTopicNames[i], "Expected topicName to be %d, was %d", expectedTopicNames[i], oh_queue.students[i].studentData.topic.topicName);
        ck_assert_msg(strncmp(oh_queue.students[i].studentData.name, expectedStudentNames[i], MAX_NAME_LENGTH) == 0, "Expected name to be %s, was %s", expectedStudentNames[i], oh_queue.students[i].studentData.name);
        int expected_id[30];
        _hash(expected_id, (char *) expectedStudentNames[i], expectedPubKeys[i]);
        for (int j = 0; j < (int) strlen(expectedStudentNames[i]); j++) {
            ck_assert_int_eq(oh_queue.students[i].customID[j], expected_id[j]);
        }
        ck_assert_msg(oh_queue.students[i].studentData.topic.questionNumber == expectedQuestionNumbers[i], "Expected questionNumber to be %f, was %f", expectedQuestionNumbers[i], oh_queue.students[i].studentData.topic.questionNumber);
        ck_assert_msg(oh_queue.students[i].queue_number == expectedQueueNumbers[i], "Expected queue_number to be %d, was %d", expectedQueueNumbers[i], oh_queue.students[i].queue_number);
    }
}
END_TEST

/* remove_student_by_topic() tests */
START_TEST(test_remove_student_by_topic_basic) {
    char studentNames[][20] = {
        "John", "Sebastian", "James", "Eddy"
    };
    enum subject topicNames[] = {
        C, Assembly, Assembly, Other
    };
    float questionNumbers[] = {
        12, 47, 47, 20
    };
    struct public_key pub_keys[] = {
        {8, 3},
        {143, 23},
        {78, 12},
        {86, 12}
    };

    for (int i = 0; i < 4; i ++) {
        _push(studentNames[i], topicNames[i], questionNumbers[i], pub_keys[i]);
    }

    struct Topic target_topic = { Assembly, 47 };
    int retVal = remove_student_by_topic(target_topic);

    ck_assert_int_eq(retVal, SUCCESS);

    ck_assert_int_eq(oh_queue.stats.no_of_people_in_queue, 2);
    ck_assert_int_eq(oh_queue.stats.no_of_people_visited, 2);
    
    // new student names, topicNames, etc.
    char expectedStudentNames[][20] = {
        "John", "Eddy"
    };
    enum subject expectedTopicNames[] = {
        C, Other
    };
    float expectedQuestionNumbers[] = {
        12, 20
    };
    struct public_key expectedPubKeys[] = {
        {8, 3},
        {86, 12}
    };
    int expectedQueueNumbers[] = {
        0, 3
    };

    for (int i = 0; i < 2; i++) {
        ck_assert_msg(oh_queue.students[i].studentData.topic.topicName == expectedTopicNames[i], "Expected topicName to be %d, was %d", expectedTopicNames[i], oh_queue.students[i].studentData.topic.topicName);
        ck_assert_msg(strncmp(oh_queue.students[i].studentData.name, expectedStudentNames[i], MAX_NAME_LENGTH) == 0, "Expected name to be %s, was %s", expectedStudentNames[i], oh_queue.students[i].studentData.name);
        int expected_id[30];
        _hash(expected_id, (char *) expectedStudentNames[i], expectedPubKeys[i]);
        for (int j = 0; j < (int) strlen(expectedStudentNames[i]); j++) {
            ck_assert_int_eq(oh_queue.students[i].customID[j], expected_id[j]);
        }
        ck_assert_msg(oh_queue.students[i].studentData.topic.questionNumber == expectedQuestionNumbers[i], "Expected questionNumber to be %f, was %f", expectedQuestionNumbers[i], oh_queue.students[i].studentData.topic.questionNumber);
        ck_assert_msg(oh_queue.students[i].queue_number == expectedQueueNumbers[i], "Expected queue_number to be %d, was %d", expectedQueueNumbers[i], oh_queue.students[i].queue_number);
    }
}
END_TEST

START_TEST(test_remove_student_by_topic_no_student_with_topic) {
    char studentNames[][20] = {
        "John", "Sebastian", "James", "Eddy"
    };
    enum subject topicNames[] = {
        C, LC3, LC3, Other
    };
    float questionNumbers[] = {
        12, 47, 47, 20
    };
    struct public_key pub_keys[] = {
        {8, 3},
        {143, 23},
        {78, 12},
        {86, 12}
    };

    for (int i = 0; i < 4; i ++) {
        _push(studentNames[i], topicNames[i], questionNumbers[i], pub_keys[i]);
    }

    struct Topic target_topic = { Assembly, 47 };
    int retVal = remove_student_by_topic(target_topic);

    ck_assert_int_eq(retVal, FAILURE);

    // Queue should be unchanged
    ck_assert_int_eq(oh_queue.stats.no_of_people_in_queue, 4);
    ck_assert_int_eq(oh_queue.stats.no_of_people_visited, 0);
    
    
    char expectedStudentNames[][20] = {
        "John", "Sebastian", "James", "Eddy"
    };
    enum subject expectedTopicNames[] = {
        C, LC3, LC3, Other
    };
    float expectedQuestionNumbers[] = {
        12, 47, 47, 20
    };
    struct public_key expectedPubKeys[] = {
        {8, 3},
        {143, 23},
        {78, 12},
        {86, 12}
    };
    int expectedQueueNumbers[] = {
        0, 1, 2, 3
    };

    for (int i = 0; i < 2; i++) {
        ck_assert_msg(oh_queue.students[i].studentData.topic.topicName == expectedTopicNames[i], "Expected topicName to be %d, was %d", expectedTopicNames[i], oh_queue.students[i].studentData.topic.topicName);
        ck_assert_msg(strncmp(oh_queue.students[i].studentData.name, expectedStudentNames[i], MAX_NAME_LENGTH) == 0, "Expected name to be %s, was %s", expectedStudentNames[i], oh_queue.students[i].studentData.name);
        int expected_id[30];
        _hash(expected_id, (char *) expectedStudentNames[i], expectedPubKeys[i]);
        for (int j = 0; j < (int) strlen(expectedStudentNames[i]); j++) {
            ck_assert_int_eq(oh_queue.students[i].customID[j], expected_id[j]);
        }
        ck_assert_msg(oh_queue.students[i].studentData.topic.questionNumber == expectedQuestionNumbers[i], "Expected questionNumber to be %f, was %f", expectedQuestionNumbers[i], oh_queue.students[i].studentData.topic.questionNumber);
        ck_assert_msg(oh_queue.students[i].queue_number == expectedQueueNumbers[i], "Expected queue_number to be %d, was %d", expectedQueueNumbers[i], oh_queue.students[i].queue_number);
    }
}
END_TEST

START_TEST(test_remove_student_by_topic_empty_array) {
    char *student_name = "George Burdell 4";
    enum subject subject = C;
    float questionNumber = 8;
    struct public_key pub_key;
    pub_key.n = 9;
    pub_key.e = 4;
    
    _push(student_name, subject, questionNumber, pub_key);
    oh_queue.stats.no_of_people_in_queue -= 1;
    // The pushed entry is no longer in the array
    
    struct Topic target_topic = { C, 8 };
    int retVal1 = remove_student_by_topic(target_topic);

    ck_assert_int_eq(retVal1, FAILURE);
    ck_assert_int_eq(oh_queue.stats.no_of_people_in_queue, 0);
    ck_assert_int_eq(oh_queue.stats.no_of_people_visited, 0);

}
END_TEST

/* queue status comprehensive tests */
START_TEST(test_office_hours_status_students_pushed) {
    for (int i = 0; i < 5; i++) {
        char student_name[30];
        sprintf(student_name, "queue_student_%d", i);
        enum subject subject = Assembly;
        float questionNumber = 3 + i;
        struct public_key pub_key;
        pub_key.n = i + 2;
        pub_key.e = i + 3;
        
        int retVal = push(student_name, subject, questionNumber, pub_key);
        ck_assert_int_eq(retVal, SUCCESS);
    }

    ck_assert_msg(strcmp(oh_queue.stats.currentStatus, "InProgress") == 0, "Expected the queue status to be InProgress, was %s", oh_queue.stats.currentStatus);
}
END_TEST

START_TEST(test_office_hours_status_all_students_popped) {
    for (int i = 0; i < 8; i++) {
        char student_name[30];
        sprintf(student_name, "queue_student_%d", i);
        enum subject subject = Assembly;
        float questionNumber = 3 + i;
        struct public_key pub_key;
        pub_key.n = i + 2;
        pub_key.e = i + 3;
        
        _push(student_name, subject, questionNumber, pub_key);
    }

    for (int i = 0; i < 8; i++) {
        int retVal = pop();
        ck_assert_int_eq(retVal, SUCCESS);
    }

    ck_assert_msg(strcmp(oh_queue.stats.currentStatus, "Completed") == 0, "Expected the queue status to be Completed, was %s", oh_queue.stats.currentStatus);
}
END_TEST

START_TEST(test_office_hours_status_some_students_popped) {
    for (int i = 0; i < 8; i++) {
        char student_name[30];
        sprintf(student_name, "queue_student_%d", i);
        enum subject subject = Assembly;
        float questionNumber = 3 + i;
        struct public_key pub_key;
        pub_key.n = i + 2;
        pub_key.e = i + 3;
        
        _push(student_name, subject, questionNumber, pub_key);
    }

    for (int i = 0; i < 3; i++) {
        int retVal = pop();
        ck_assert_int_eq(retVal, SUCCESS);
    }

    ck_assert_msg(strcmp(oh_queue.stats.currentStatus, "InProgress") == 0, "Expected the queue status to be InProgress, was %s", oh_queue.stats.currentStatus);
}
END_TEST

START_TEST(test_office_hours_status_remove_some_students_by_topic) {
    char studentNames[][20] = {
        "David", "Eddy", "James", "Eddy"
    };
    enum subject topicNames[] = {
        LC3, Assembly, Assembly, Other
    };
    float questionNumbers[] = {
        2, 47, 47, 20
    };
    struct public_key pub_keys[] = {
        {143, 23},
        {8, 3},
        {78, 12},
        {86, 12}
    };

    for (int i = 0; i < 4; i ++) {
        _push(studentNames[i], topicNames[i], questionNumbers[i], pub_keys[i]);
    }

    struct Topic target_topic = { Assembly, 47 };
    int retVal = remove_student_by_topic(target_topic);

    ck_assert_int_eq(retVal, SUCCESS);

    ck_assert_msg(strcmp(oh_queue.stats.currentStatus, "InProgress") == 0, "Expected the queue status to be InProgress, was %s", oh_queue.stats.currentStatus);
}
END_TEST

START_TEST(test_office_hours_status_remove_all_students_by_topic) {
    char studentNames[][20] = {
        "David", "Eddy", "James", "Eddy"
    };
    enum subject topicNames[] = {
        C, C, C, C
    };
    float questionNumbers[] = {
        82.5, 82.5, 82.5, 82.5
    };
    struct public_key pub_keys[] = {
        {143, 23},
        {8, 3},
        {78, 12},
        {86, 12}
    };

    for (int i = 0; i < 4; i ++) {
        _push(studentNames[i], topicNames[i], questionNumbers[i], pub_keys[i]);
    }

    struct Topic target_topic = { C, 82.5 };
    int retVal = remove_student_by_topic(target_topic);

    ck_assert_int_eq(retVal, SUCCESS);

    ck_assert_msg(strcmp(oh_queue.stats.currentStatus, "Completed") == 0, "Expected the queue status to be Completed, was %s", oh_queue.stats.currentStatus);
}
END_TEST

START_TEST(test_office_hours_status_remove_all_students_by_name) {
    char studentNames[][20] = {
        "Only Person"
    };
    enum subject topicNames[] = {
        C
    };
    float questionNumbers[] = {
        82.5
    };
    struct public_key pub_keys[] = {
        {143, 23}
    };

    _push(studentNames[0], topicNames[0], questionNumbers[0], pub_keys[0]);
    
    int retVal = remove_student_by_name("Only Person");

    ck_assert_int_eq(retVal, SUCCESS);

    ck_assert_msg(strcmp(oh_queue.stats.currentStatus, "Completed") == 0, "Expected the queue status to be Completed, was %s", oh_queue.stats.currentStatus);
}
END_TEST

START_TEST(test_office_hours_status_remove_one_student_by_name) {
    char studentNames[][20] = {
        "David", "Eddy", "James", "Eddy"
    };
    enum subject topicNames[] = {
        LC3, Assembly, Assembly, Other
    };
    float questionNumbers[] = {
        47, 2, 47, 20
    };
    struct public_key pub_keys[] = {
        {143, 23},
        {8, 3},
        {78, 12},
        {86, 12}
    };

    for (int i = 0; i < 4; i ++) {
        _push(studentNames[i], topicNames[i], questionNumbers[i], pub_keys[i]);
    }

    int retVal = remove_student_by_name("Eddy");

    ck_assert_int_eq(retVal, SUCCESS);

    ck_assert_msg(strcmp(oh_queue.stats.currentStatus, "InProgress") == 0, "Expected the queue status to be InProgress, was %s", oh_queue.stats.currentStatus);
}
END_TEST
///////////////////////////////////////////////////////////////////////////////////////////////////
/************************************** HELPER FUNCTIONS *****************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Wipe out the array - since they are globals, it is reasonable for students to expect the array to be zeroed and size to be 0 before each test
 * Also try to detect any funky stuff going on with including <string(s).h>
 */
static void setup(void)
{
    memset(oh_queue.students, 0, sizeof(struct Student) * MAX_QUEUE_LENGTH);
    oh_queue.stats.no_of_people_in_queue = 0;
    oh_queue.stats.no_of_people_visited = 0;
    oh_queue.stats.currentStatus = "Completed";
}


static void _push(const char *studentName, const enum subject topicName, const float questionNumber, struct public_key pub_key) {
    struct Topic newTopic;
    newTopic.topicName = topicName;
    newTopic.questionNumber = questionNumber;
    
    struct StudentData studentD;
    strncpy(studentD.name, studentName, strlen(studentName)+1);
    studentD.topic = newTopic;
    
    struct Student student;
    student.studentData = studentD;
    _hash(student.customID, studentD.name, pub_key);
    student.queue_number = oh_queue.stats.no_of_people_in_queue + oh_queue.stats.no_of_people_visited;
    oh_queue.students[oh_queue.stats.no_of_people_in_queue++] = student;
}

static void _hash(int *ciphertext, char *plaintext, struct public_key pub_key) {
    int plaintext_len = strlen(plaintext);

    for (int i = 0; i < plaintext_len; i++) {
        int val = (int) plaintext[i];

        int cipher_number = power_and_mod(val, pub_key.e, pub_key.n);

        ciphertext[i] = cipher_number;
    }
}

Suite *oh_queue_suite(void)
{
    Suite *s = suite_create("oh_queue_suite");

    // PUSHES
    tcase_hack(s, setup, NULL, test_push_single);
    tcase_hack(s, setup, NULL, test_push_two);
    tcase_hack(s, setup, NULL, test_push_multiple);
    tcase_hack(s, setup, NULL, test_push_null_terminator);
    tcase_hack(s, setup, NULL, test_push_long_name);
    tcase_hack(s, setup, NULL, test_push_too_many_students);

    // POPS
    tcase_hack(s, setup, NULL, test_pop_basic);
    tcase_hack(s, setup, NULL, test_pop_single_student);
    tcase_hack(s, setup, NULL, test_pop_no_students);
    tcase_hack(s, setup, NULL, test_pop_multiple);
    tcase_hack(s, setup, NULL, test_pop_everything);

    // GROUPS
    tcase_hack(s, setup, NULL, test_group_by_topic_basic);
    tcase_hack(s, setup, NULL, test_group_by_topic_different_topic_name_same_number);
    tcase_hack(s, setup, NULL, test_group_by_topic_same_topic_different_number);
    tcase_hack(s, setup, NULL, test_group_by_topic_no_student_with_topic);
    tcase_hack(s, setup, NULL, test_group_by_topic_empty_array);

    // HASHES
    tcase_hack(s, setup, NULL, test_hash_1);
    tcase_hack(s, setup, NULL, test_hash_2);

    // UPDATES
    tcase_hack(s, setup, NULL, test_update_student_basic);
    tcase_hack(s, setup, NULL, test_update_student_no_student_with_custom_id)
    tcase_hack(s, setup, NULL, test_update_student_empty_array);
    
    // REMOVE BY NAME
    tcase_hack(s, setup, NULL, test_remove_student_by_name_basic);
    tcase_hack(s, setup, NULL, test_remove_student_by_name_no_students_with_name);
    tcase_hack(s, setup, NULL, test_remove_student_by_name_empty_array);
    tcase_hack(s, setup, NULL, test_remove_student_by_name_multiple);

    // REMOVE BY TOPIC
    tcase_hack(s, setup, NULL, test_remove_student_by_topic_basic);
    tcase_hack(s, setup, NULL, test_remove_student_by_topic_no_student_with_topic);
    tcase_hack(s, setup, NULL, test_remove_student_by_topic_empty_array);
    
    // QUEUE STATUS
    tcase_hack(s, setup, NULL, test_office_hours_status_students_pushed);
    tcase_hack(s, setup, NULL, test_office_hours_status_all_students_popped);
    tcase_hack(s, setup, NULL, test_office_hours_status_some_students_popped);
    tcase_hack(s, setup, NULL, test_office_hours_status_remove_some_students_by_topic);
    tcase_hack(s, setup, NULL, test_office_hours_status_remove_all_students_by_topic);
    tcase_hack(s, setup, NULL, test_office_hours_status_remove_all_students_by_name);
    tcase_hack(s, setup, NULL, test_office_hours_status_remove_one_student_by_name);
    // Return Testing Suite
    return s;
}

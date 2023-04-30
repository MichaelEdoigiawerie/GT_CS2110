/**
 * @file my_string_suite.c
 * @author Richard Zhang
 * @brief Testing Suites to HW7 - Spring 2023
 * @date 2023-03-13
 */

// Check Docs
// Tutorial : https://libcheck.github.io/check/doc/check_html/check_3.html
// Check API: https://libcheck.github.io/check/doc/doxygen/html/check_8h.html

// System Headers
#include <stdio.h>
#include <string.h>
#include <check.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include "test_utils.h"

static void randStringGenerator(char *dest, int length);

/******************************************************************************/
/**************************** my_strlen tests *********************************/
/******************************************************************************/

START_TEST(test_my_strlen_1)
{
    const char *str = "12345";
    const char *backup = str; // pointers should not be changed
    size_t retVal = my_strlen(str);
    ck_assert_ptr_eq(str, backup);
    ck_assert_int_eq(retVal, strlen(str));
}
END_TEST

START_TEST(test_my_strlen_2)
{
    const char *str = "1\0";
    const char *backup = str; // pointers should not be changed

    size_t retVal = my_strlen(str);
    ck_assert_ptr_eq(str, backup);
    ck_assert_int_eq(retVal, strlen(str));
}
END_TEST

START_TEST(test_my_strlen_3)
{
    const char str[] = {'1', '2', '3', '\0', 'Z', 'f'};
    const char *backup = str; // pointers should not be changed
    size_t retVal = my_strlen(str);
    ck_assert_ptr_eq(str, backup);
    ck_assert_int_eq(retVal, strlen(str));
}
END_TEST

START_TEST(test_my_strlen_4)
{
    srand(time(0));
    unsigned long randBoi = rand() % 512;
    if (randBoi < 100)
    {
        // Everybody gotta do at least 155 characters
        randBoi += 155;
    }

    // The only function that doesn't use the random string generator
    char str[512];
    const char *backup = str; // pointers should not be changed

    memset(str, 7, randBoi - 1);
    str[randBoi] = '\0';

    size_t retVal = my_strlen(str);
    ck_assert_ptr_eq(str, backup);
    ck_assert_int_eq(retVal, strlen(str));
}
END_TEST

START_TEST(test_my_strlen_5)
{
    const char *str = "";
    const char *backup = str; // pointers should not be changed
    size_t retVal = my_strlen(str);
    ck_assert_ptr_eq(str, backup);
    ck_assert_int_eq(retVal, strlen(str));
}
END_TEST

/******************************************************************************/
/**************************** my_strncmp tests ********************************/
/******************************************************************************/

/**
 * All the strncmp test cases just check for negative, positive, or zero value
 * and the actual value doesn't matter. This is done to adhere to strncmp man page
 * and resources online.
 */

START_TEST(test_my_strncmp_equal)
{
    const char *str1 = "1234/#?\\21tenMo";
    const char *str2 = "1234/#?\\21tenMo";

    const char *backup1 = str1; // pointers should not be changed
    const char *backup2 = str2; // pointers should not be changed

    size_t n = strlen(str1);

    int retVal = my_strncmp(str1, str2, n);
    int correctValSign = strncmp(str1, str2, n);

    if (correctValSign < 0)
    {
        ck_assert_int_lt(retVal, 0);
    }
    else if (correctValSign > 0)
    {
        ck_assert_int_gt(retVal, 0);
    }
    else
    {
        ck_assert_int_eq(retVal, correctValSign);
    }

    ck_assert_ptr_eq(str1, backup1);
    ck_assert_ptr_eq(str2, backup2);
}
END_TEST

START_TEST(test_my_strncmp_null_terminator_in_middle_almost_same)
{
    const char str1_buf[] = {'Z', '\0',       'y', '0', 'h', 'a', '\0',      'Z', 'f'};
    const char *str1 = str1_buf + 2;
    const char str2_buf[] = {'A', 'a',        'y', '0', 'h', 'a', '\0',      'A', 'a'};
    const char *str2 = str2_buf + 2;

    const char *backup1 = str1; // pointers should not be changed
    const char *backup2 = str2;

    size_t n = strlen(str1) + 2;

    int retVal = my_strncmp(str1, str2, n);
    int correctValSign = strncmp(str1, str2, n);

    if (correctValSign < 0)
    {
        ck_assert_int_lt(retVal, 0);
    }
    else if (correctValSign > 0)
    {
        ck_assert_int_gt(retVal, 0);
    }
    else
    {
        ck_assert_int_eq(retVal, correctValSign);
    }

    ck_assert_ptr_eq(str1, backup1);
    ck_assert_ptr_eq(str2, backup2);
}
END_TEST

START_TEST(test_my_strncmp_null_terminator_in_middle_different)
{
    // Should return negative
    const char str1_buf[] = {'Z', 'f',   'y', 'e', 'e', 'h', 'a', '\0',                   'Z', 'f'};
    const char *str1 = str1_buf + 2;
    const char str2_buf[] = {'A', '\0',  'y', 'e', 'e', 'h', 'a', 'B', 'r', 'o', '\0',    'A', 'a'};
    const char *str2 = str2_buf + 2;

    const char *backup1 = str1; // pointers should not be changed
    const char *backup2 = str2;
    size_t n = strlen(str1) + 1;

    int retVal = my_strncmp(str1, str2, n);
    int correctValSign = strncmp(str1, str2, n);

    if (correctValSign < 0)
    {
        ck_assert_int_lt(retVal, 0);
    }
    else if (correctValSign > 0)
    {
        ck_assert_int_gt(retVal, 0);
    }
    else
    {
        ck_assert_int_eq(retVal, correctValSign);
    }

    ck_assert_ptr_eq(str1, backup1);
    ck_assert_ptr_eq(str2, backup2);
}
END_TEST

START_TEST(test_my_strncmp_random)
{
    // Let's see if student's can hack this test case (without zerodays tho..)

    srand(time(0));
    unsigned int randBoi = rand() % 128;
    if (randBoi < 69) // "nice" - the person shall remain nameless
    {
        randBoi += 100; // Everybody gonna do 100 minimum
    }
    char str1[169];

    randStringGenerator(str1, sizeof(str1));

    char str2[169];
    strncpy(str2, str1, strlen(str1)); // Creating a copy
    str2[randBoi] = '\0';

    str2[5] = 'F'; // Yeah boiii

    const char *backup1 = str1; // pointers should not be changed
    const char *backup2 = str2;

    size_t n = (strlen(str1) / 2) + 1;

    int retVal = my_strncmp(str1, str2, n);
    int correctValSign = strncmp(str1, str2, n);

    if (correctValSign < 0)
    {
        ck_assert_int_lt(retVal, 0);
    }
    else if (correctValSign > 0)
    {
        ck_assert_int_gt(retVal, 0);
    }
    else
    {
        ck_assert_int_eq(retVal, correctValSign);
    }

    ck_assert_ptr_eq(str1, backup1);
    ck_assert_ptr_eq(str2, backup2);
}
END_TEST


/*
Test for shuffled letters in a string
Eg. s1 = "ABC", s2 = "CBA"
*/
START_TEST(test_my_strncmp_letters_shuffled)
{
    const char *str1 = "ABCD";
    const char *str2 = "BCDA";

    const char *backup1 = str1; // pointers should not be changed
    const char *backup2 = str2; // pointers should not be changed

    size_t n = strlen(str1) + 1;

    int retVal = my_strncmp(str1, str2, n);
    int correctValSign = strncmp(str1, str2, n);

    if (correctValSign < 0)
    {
        ck_assert_int_lt(retVal, 0);
    }
    else if (correctValSign > 0)
    {
        ck_assert_int_gt(retVal, 0);
    }
    else
    {
        ck_assert_int_eq(retVal, correctValSign);
    }

    ck_assert_ptr_eq(str1, backup1);
    ck_assert_ptr_eq(str2, backup2);

}
END_TEST

START_TEST(test_my_strncmp_first_half_same_end_different_large_n)
{
    const char *str1 = "chocolatecake";
    const char *str2 = "chocolatechipcookie";

    const char *backup1 = str1; // pointers should not be changed
    const char *backup2 = str2; // pointers should not be changed

    size_t n = strlen(str2) + 1;

    int retVal = my_strncmp(str1, str2, n);
    int correctValSign = strncmp(str1, str2, n);

    if (correctValSign < 0)
    {
        ck_assert_int_lt(retVal, 0);
    }
    else if (correctValSign > 0)
    {
        ck_assert_int_gt(retVal, 0);
    }
    else
    {
        ck_assert_int_eq(retVal, correctValSign);
    }

    ck_assert_ptr_eq(str1, backup1);
    ck_assert_ptr_eq(str2, backup2);

}
END_TEST

START_TEST(test_my_strncmp_first_half_same_end_different_small_n)
{
    const char *str1 = "chocolatecake";
    const char *str2 = "chocolatechipcookie";

    const char *backup1 = str1; // pointers should not be changed
    const char *backup2 = str2; // pointers should not be changed

    size_t n = 7;

    int retVal = my_strncmp(str1, str2, n);
    int correctValSign = strncmp(str1, str2, n);

    if (correctValSign < 0)
    {
        ck_assert_int_lt(retVal, 0);
    }
    else if (correctValSign > 0)
    {
        ck_assert_int_gt(retVal, 0);
    }
    else
    {
        ck_assert_int_eq(retVal, correctValSign);
    }

    ck_assert_ptr_eq(str1, backup1);
    ck_assert_ptr_eq(str2, backup2);

}
END_TEST

START_TEST(test_my_strncmp_almost_equal)
{
    const char *str1 = "spooky scary skeleton";
    const char *str2 = "spooky scary skeletons";

    const char *backup1 = str1; // pointers should not be changed
    const char *backup2 = str2; // pointers should not be changed

    size_t n = strlen(str2);

    int retVal = my_strncmp(str1, str2, n);
    int correctValSign = strncmp(str1, str2, n);

    if (correctValSign < 0)
    {
        ck_assert_int_lt(retVal, 0);
    }
    else if (correctValSign > 0)
    {
        ck_assert_int_gt(retVal, 0);
    }
    else
    {
        ck_assert_int_eq(retVal, correctValSign);
    }

    ck_assert_ptr_eq(str1, backup1);
    ck_assert_ptr_eq(str2, backup2);

}
END_TEST

/******************************************************************************/
/*************************** my_strncpy tests *********************************/
/******************************************************************************/
START_TEST(test_my_strncpy_basic)
{
    char src[] = {'\0'};
    const char *backup = src; // pointers should not be changed

    char dest[] = {'a', '\0'};
    size_t n = 1;

    char *retVal = my_strncpy(dest, src, n);

    // Checking if dest didn't change
    ck_assert_ptr_eq(retVal, dest);

    ck_assert_ptr_eq(src, backup);

    // Checking if strncmp returns 0
    ck_assert_msg(!strncmp(dest, src, n), "did not overwrite first byte of dest\n");
}
END_TEST

START_TEST(test_my_strncpy_destination_larger)
{
    char *src = "His palms are sweaty, knees weak, arms are heavy ... mom's spaghetti";
    const char *backup = src; // pointers should not be changed

    char dest[100];
    memset(dest, 0, sizeof(dest)); // just to be safe
    size_t n = strlen(src);

    char *retVal = my_strncpy(dest, src, n);

    // Checking if dest didn't change
    ck_assert_ptr_eq(retVal, dest);

    ck_assert_ptr_eq(src, backup);

    // Checking if strncmp returns 0
    ck_assert_msg(!strncmp(dest, src, n), "did not overwrite first byte of dest\n");
}
END_TEST

START_TEST(test_my_strncpy_random_basic_256)
{
    srand(time(0) - 5000); // 5000 means something Sean

    // Generate random source
    char src[256 + 1];
    const char *backup = src; // pointers should not be changed

    randStringGenerator(src, sizeof(src));

    // No null-termination on dest on purpose
    char dest[256];
    size_t n = strlen(src);
    char *retVal = my_strncpy(dest, src, n);

    // Checking if dest didn't change
    ck_assert_ptr_eq(retVal, dest);

    ck_assert_ptr_eq(src, backup);

    // Checking if strncmp returns 0
    ck_assert_msg(!strncmp(dest, src, n), "did not copy the first 256 characters of src to dest correctly\n");
}
END_TEST

START_TEST(test_my_strncpy_null_terminator_in_middle)
{
    srand(time(0) - 5000); // 5000 means something Sean

    // Generate random source
    char src[256];
    randStringGenerator(src, sizeof(src));
    const char *backup = src; // pointers should not be changed

    // Null terminating in middle to make sure they only copy all the way to null terminator
    150 [src] = 0; // gangsta move

    // No null-termination on dest on purpose
    char dest[256];
    memset(dest, 7, sizeof(256)); // Filling destination with arbitrary val
    size_t n = 256;
    char *retVal = my_strncpy(dest, src, n);

    // Checking if dest didn't change
    ck_assert_ptr_eq(retVal, dest);

    ck_assert_ptr_eq(src, backup);

    // Size should be the same because of n and null terminator position
    ck_assert_int_eq(strlen(dest), strlen(src));

    // Checking if strncmp returns 0
    ck_assert_msg(!strncmp(dest, src, strlen(src) + 1), "did not copy src to dest correctly when null terminator is in middle\n");
}
END_TEST

START_TEST(test_my_strncpy_overwrite)
{
    char src[100];
    randStringGenerator(src, sizeof(src));
    const char *backup = src; // pointers should not be changed

    char dest[100];
    randStringGenerator(dest, sizeof(dest));
    size_t n = strlen(dest);

    char *retVal = my_strncpy(dest, src, n);

    // Checking if dest didn't change
    ck_assert_ptr_eq(retVal, dest);

    ck_assert_ptr_eq(src, backup);

    // Checking if strncmp returns 0
    ck_assert_msg(!strncmp(dest, src, n), "did not overwrite destination completely\n");
}
END_TEST

START_TEST(test_my_strncpy_adv)
{
    char src[77];
    randStringGenerator(src, sizeof(src));
    const char *backup = src; // pointers should not be changed

    // Making sure we overwrite null terminator at the end so strncpy doesn't halt early
    src[strlen(src) - 1] = 'F';

    char dest[100];
    randStringGenerator(dest, sizeof(dest));
    size_t n = strlen(dest) - 1;

    // so everything after src length should be nulled out

    char *retVal = my_strncpy(dest, src, n);

    // Checking if dest didn't change
    ck_assert_ptr_eq(retVal, dest);
    ck_assert_ptr_eq(src, backup);

    // Checking if strncmp returns 0
    ck_assert_msg(!strncmp(dest, src, strlen(src)), "did not overwrite destination completely\n");

    // Making sure students actually read the man page
    // "If the length of src is less than n, strncpy() writes additional null bytes
    // to dest to ensure that a total of n bytes are written."
    char zeroedOut[100];
    memset(zeroedOut, '\0', sizeof(zeroedOut));
    int test = !strncmp(dest + strlen(src), zeroedOut, n - strlen(src));
    ck_assert_msg(test, "Make sure you read the man page carefully\n");
}
END_TEST

/******************************************************************************/
/**************************** my_strncat tests ********************************/
/******************************************************************************/
START_TEST(test_my_strncat_basic)
{
    char src[] = {'b','\0'};

    char dest[] = {'a', '\0'};
    size_t n = 1;

    char *retVal = my_strncat(dest, src, n);

    // Checking if dest didn't change
    ck_assert_ptr_eq(retVal, dest);

    // Checking if strncmp returns 0
    char dest1[] = {'a', '\0'};
    char *correctVal = strncat(dest1, src, n);
    ck_assert_int_eq(strncmp(retVal, correctVal, strlen(correctVal)),0);
}
END_TEST

START_TEST(test_my_strncat_random)
{
    char src[100];
    randStringGenerator(src, sizeof(src));

    char dest[200] = "123";
    char dest1[200] = "123";
    size_t n = strlen(src);

    char *retVal = my_strncat(dest, src, n);
    char *correctVal = strncat(dest1, src, n);

    // Checking if dest pointer didn't change
    ck_assert_ptr_eq(retVal, dest);

    // Checking if strncmp returns 0
    ck_assert_int_eq(strncmp(retVal, correctVal, strlen(dest1)),0);
}
END_TEST

START_TEST(test_my_strncat_null_terminator)
{
    char src[100];
    randStringGenerator(src, sizeof(src));

    char dest[200] = "This is the first half of the string";
    char dest1[200] = "This is the first half of the string";
    size_t n = strlen(src) - 10; //don't concat all of n (add a null terintator)

    char *retVal = my_strncat(dest, src, n);
    char *correctVal = strncat(dest1, src, n);

    // Checking if dest pointer didn't change
    ck_assert_ptr_eq(retVal, dest);

    // Checking if strncmp returns 0
    ck_assert_int_eq(strncmp(retVal, correctVal, 200),0);
}
END_TEST

START_TEST(test_my_strncat_with_empty_string_first)
{
    char my_str[1000];
    randStringGenerator(my_str, sizeof(my_str) - 500); //fill buffer with 500 rando chars
    char empty[1000] = "\0";

    char *retVal = my_strncat(empty, my_str, 400);

    // Checking if dest pointer didn't change
    ck_assert_ptr_eq(retVal, empty);

    // Checking if the empty is the as my_str after concat
    ck_assert_int_eq(strncmp(empty, my_str, 400), 0);
}
END_TEST

START_TEST(test_my_strncat_with_empty_string_second)
{
    char my_str[1000];
    randStringGenerator(my_str, sizeof(my_str) - 500); //fill buffer with 500 rando chars
    char empty[100] = "\0";
    size_t n = 100; //attempt to concat empty buffer

    char *retVal = my_strncat(my_str, empty, n);

    // Checking if dest pointer didn't change
    ck_assert_ptr_eq(retVal, my_str);

    // Checking if concat string = original string
    ck_assert_str_eq(retVal, my_str);
}
END_TEST

/******************************************************************************/
/**************************** my_memset tests ********************************/
/******************************************************************************/
START_TEST(test_my_memset_basic)
{
    char my_str[100] = "abcdefghij";
    char str[100] = "abcdefghij";
    char *backup = my_str;
    my_memset(my_str + 3, '*', 2);
    memset(str + 3, '*', 2);
    // Checking if my_str pointer didn't change
    ck_assert_ptr_eq(my_str, backup);

    // Checking if strncmp returns 0
    ck_assert_int_eq(strcmp(str,my_str),0);
}
END_TEST


START_TEST(test_my_memset_random_n)
{
    char my_str[200];
    char str[200];
    randStringGenerator(my_str, sizeof(my_str));
    randStringGenerator(str, sizeof(str));
    srand(time(0));
    size_t num = rand() % 198; //n < string buffer size
    if (num < 50) {
        num += 100;
    }
    char *backup = my_str;
    my_memset(my_str, 's', num);
    my_memset(my_str+num,'\0', 1);
    memset(str, 's', num);
    memset(str+num,'\0', 1);
    // Checking if my_str pointer didn't change
    ck_assert_ptr_eq(my_str, backup);

    // Checking if strncmp returns 0
    ck_assert_int_eq(strcmp(str,my_str),0);
}
END_TEST

START_TEST(test_my_memset_replace_char)
{
    char my_str[100];
    randStringGenerator(my_str, sizeof(my_str));
    srand(time(0));
    size_t num = rand() % 99; //num < string buffer size
    char *backup = my_str;
    my_memset(my_str + num, '*', 1);
    char replacedChar = my_str[num];
    // Checking if my_str pointer didn't change
    ck_assert_ptr_eq(my_str, backup);

    // Checking if the char was correctly set 
    ck_assert_int_eq(replacedChar,'*');
}
END_TEST

START_TEST(test_my_memset_replace_all)
{
    char arr[100];
    char arr1[100];
    randStringGenerator(arr, 100); //random string 1
    randStringGenerator(arr1, 100); //random string 2

    //replace all string chars with 0
    my_memset(arr, 0, 100);
    memset(arr1, 0,100);
    //check buffers are equal
    ck_assert_str_eq(arr,arr1);
    
}
END_TEST
/******************************************************************************/
/**************************** remove_first_instance tests ********************************/
/******************************************************************************/
START_TEST(test_remove_first_instance_basic)
{
    char test_str[] = "abc";
    char char_to_remove = 'b';

    //remove 'b'
    remove_first_instance(test_str, char_to_remove);
    
    //check buffers are equal
    ck_assert_str_eq(test_str,"ac");
    
}
END_TEST

START_TEST(test_remove_first_instance_random)
{
    char test_str[100];
    randStringGenerator(test_str, sizeof(test_str));

    srand(time(0));
    unsigned long randIndex = rand() % 99;
    char randChar = test_str[randIndex];

    // find the first instance of randChar
    int i = 0;
    while (i < (int)randIndex) {
        if (test_str[i] == randChar) {
            break;
        }
        i ++;
    }

    char orig_str[100];
    strcpy(orig_str, test_str);

    remove_first_instance(test_str, randChar);

    // Checking if strncmp returns 0 for first half (before removed char)
    ck_assert_int_eq(strncmp(test_str, orig_str, i),0);

    // Checking if strncmp returns 0 for second half (after removed char)
    ck_assert_int_eq(strncmp(test_str + i, orig_str + i + 1, sizeof(orig_str) - i),0);
}
END_TEST

START_TEST(test_remove_first_instance_instance_as_first_char)
{
    char test_str[100];
    randStringGenerator(test_str, sizeof(test_str));

    // select the target char as the first char
    char remove_char = test_str[0];
    
    char orig_str[100];
    strcpy(orig_str, test_str);
    
    remove_first_instance(test_str, remove_char);

    // Checking if test string removed first char
    ck_assert_str_eq(test_str, orig_str + 1);
}
END_TEST

START_TEST(test_remove_first_instance_one_char_string)
{
    char test_str[2];
    randStringGenerator(test_str, sizeof(test_str));

    char remove_char = test_str[0];
    
    remove_first_instance(test_str, remove_char);

    // Checking if test string is empty
    ck_assert_int_eq(strlen(test_str), 0);
    ck_assert_str_eq(test_str, "");
}
END_TEST

START_TEST(test_remove_first_instance_no_instances)
{
    char test_str[100];
    randStringGenerator(test_str, sizeof(test_str));

    srand(time(0));
    unsigned long randIndex = rand() % 99;
    char randChar = test_str[randIndex];

    // Replace the randChar with its ascii value + 1
    // No randChar should be in the string after this while loop
    int i = 0;
    while (i < (int) sizeof(test_str)) {
        if (test_str[i] == randChar) {
            test_str[i]++;
        }
        i ++;
    }

    char orig_str[100];
    strcpy(orig_str, test_str);
    remove_first_instance(test_str, randChar);

    // Checking if test string did not change
    ck_assert_str_eq(test_str, orig_str);
}
END_TEST

/******************************************************************************/
/**************************** replace_character_with_string tests ********************************/
/******************************************************************************/
START_TEST(test_replace_character_with_string_basic)
{
    char test_str[100] = "abc";
    char char_to_replace = 'b';
    char replace_string[] = "abc";

    //replace 'b' with 'abc'
    replace_character_with_string(test_str, char_to_replace, replace_string);
    
    //check buffers are equal
    ck_assert_str_eq(test_str,"aabcc");
    
}
END_TEST

START_TEST(test_replace_character_with_string_random)
{
    char test_str[1000];
    randStringGenerator(test_str, sizeof(test_str) - 500);

    char replace_str[100];
    randStringGenerator(replace_str, sizeof(replace_str));

    srand(time(0));
    unsigned long randIndex = rand() % (sizeof(test_str) - 500 - 1);
    char randChar = test_str[randIndex];

    // Find the index of the first randChar
    int i = 0;
    while (i < (int)randIndex) {
        if (test_str[i] == randChar) {
            break;
        }
        i ++;
    }

    char orig_str[1000];
    strcpy(orig_str, test_str);

    replace_character_with_string(test_str, randChar, replace_str);

    // Checking if strncmp returns 0 for first half (before replaced char)
    ck_assert_int_eq(strncmp(test_str, orig_str, i),0);

    // Checking if strncmp returns 0 for replace_str part of test_str
    ck_assert_int_eq(strncmp(test_str + i, replace_str, strlen(replace_str)),0);

    // Checking if strncmp returns 0 for second half (after inserted replace_str)
    ck_assert_int_eq(strncmp(test_str + i + strlen(replace_str), orig_str + i + 1, sizeof(orig_str) - i),0);
}
END_TEST

START_TEST(test_replace_character_with_string_no_character_found)
{
    char test_str[1000];
    randStringGenerator(test_str, sizeof(test_str) - 500);

    char replace_str[100];
    randStringGenerator(replace_str, sizeof(replace_str));

    srand(time(0));
    unsigned long randIndex = rand() % (sizeof(test_str) - 500 - 1);
    char randChar = test_str[randIndex];

    // Replace the randChar with its ascii value + 1
    // No randChar should be in the string after this while loop
    int i = 0;
    while (i < (int) sizeof(test_str)) {
        if (test_str[i] == randChar) {
            test_str[i]++;
        }
        i ++;
    }

    char orig_str[1000];
    strcpy(orig_str, test_str);
    replace_character_with_string(test_str, randChar, replace_str);

    // Checking if test string did not change
    ck_assert_str_eq(test_str, orig_str);
}
END_TEST

START_TEST(test_replace_character_with_string_multiple_characters)
{
    char test_str[1000];
    randStringGenerator(test_str, sizeof(test_str) - 500);

    char replace_str[100];
    randStringGenerator(replace_str, sizeof(replace_str));

    srand(time(0));
    unsigned long randIndex = rand() % (sizeof(test_str) - 500 - 100);
    char randChar = test_str[randIndex];

    // Replace the randChar with its ascii value + 49
    // No randChar should be in the string after this while loop
    int i = 0;
    while (i < (int) sizeof(test_str)) {
        if (test_str[i] == randChar) {
            test_str[i + 49] = randChar;
            break;
        }
        i ++;
    }

    char orig_str[1000];
    strcpy(orig_str, test_str);
    replace_character_with_string(test_str, randChar, replace_str);

    // Checking if strncmp returns 0 for first half (before replaced char)
    ck_assert_int_eq(strncmp(test_str, orig_str, i),0);

    // Checking if strncmp returns 0 for replace_str part of test_str
    ck_assert_int_eq(strncmp(test_str + i, replace_str, strlen(replace_str)),0);

    // Checking if strncmp returns 0 for second half (after inserted replace_str)
    ck_assert_int_eq(strncmp(test_str + i + strlen(replace_str), orig_str + i + 1, sizeof(orig_str) - i),0);
}
END_TEST

START_TEST(test_replace_character_with_string_single_char_replace_string)
{
    char test_str[1000];
    randStringGenerator(test_str, sizeof(test_str) - 500);

    char replace_str[2];
    randStringGenerator(replace_str, sizeof(replace_str));

    srand(time(0));
    unsigned long randIndex = rand() % (sizeof(test_str) - 500 - 1);
    char randChar = test_str[randIndex];

    // Find the index of the first randChar
    int i = 0;
    while (i < (int) sizeof(test_str)) {
        if (test_str[i] == randChar) {
            break;
        }
        i ++;
    }

    char orig_str[1000];
    strcpy(orig_str, test_str);
    replace_character_with_string(test_str, randChar, replace_str);

    // Checking if strncmp returns 0 for first half (before replaced char)
    ck_assert_int_eq(strncmp(test_str, orig_str, i),0);

    // Checking if strncmp returns 0 for replace_str part of test_str
    ck_assert_int_eq(strncmp(test_str + i, replace_str, strlen(replace_str)),0);

    // Checking if strncmp returns 0 for second half (after inserted replace_str)
    ck_assert_int_eq(strncmp(test_str + i + strlen(replace_str), orig_str + i + 1, sizeof(orig_str) - i),0);
}
END_TEST

START_TEST(test_replace_character_with_string_empty_replace_string)
{
    char test_str[1000];
    randStringGenerator(test_str, sizeof(test_str) - 500);

    char replace_str[] = "";

    srand(time(0));
    unsigned long randIndex = rand() % (sizeof(test_str) - 500 - 1);
    char randChar = test_str[randIndex];

    // Find the index of the first randChar
    int i = 0;
    while (i < (int) sizeof(test_str)) {
        if (test_str[i] == randChar) {
            break;
        }
        i ++;
    }

    char orig_str[1000];
    strcpy(orig_str, test_str);
    replace_character_with_string(test_str, randChar, replace_str);

    // Checking if strncmp returns 0 for first half (before replaced char)
    ck_assert_int_eq(strncmp(test_str, orig_str, i),0);

    // Checking if strncmp returns 0 for replace_str part of test_str
    ck_assert_int_eq(strncmp(test_str + i, replace_str, strlen(replace_str)),0);

    // Checking if strncmp returns 0 for second half (after inserted replace_str)
    ck_assert_int_eq(strncmp(test_str + i + strlen(replace_str), orig_str + i + 1, sizeof(orig_str) - i),0);
}
END_TEST
/******************************************************************************/
/**************************** replace_character_with_string tests ********************************/
/******************************************************************************/
START_TEST(test_remove_first_character_random)
{
    char test_str[400];
    randStringGenerator(test_str, sizeof(test_str));
    char orig_str[400];
    strcpy(orig_str, test_str);
    char *test_str_non_array = &test_str[0];
    remove_first_character(&test_str_non_array);

    ck_assert_str_eq(test_str_non_array, orig_str + 1);
}
END_TEST

START_TEST(test_remove_first_character_one_char_string)
{
    char test_str[2];
    randStringGenerator(test_str, sizeof(test_str));
    char orig_str[2];
    strcpy(orig_str, test_str);
    char *test_str_non_array = &test_str[0];
    remove_first_character(&test_str_non_array);

    ck_assert_str_eq(test_str_non_array, orig_str + 1);
}
END_TEST

/**
 * @brief Automatically ran before every test.
 */
static void setup(void)
{
}

/**
 * @brief Helper function for creating random strings. It will null-terminate like a champ.
 */
// Thanks bro --> https://stackoverflow.com/questions/15767691/whats-the-c-library-function-to-generate-random-string/15768317
static void randStringGenerator(char *buffer, int length)
{
    // C weird syntax once again
    char charset[] = "0123456789"
                     "abcdefghijklmnopqrstuvwxyz"
                     "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    // So we don't have to keep adding space for the null terminator
    length--;

    while (length-- > 0)
    {
        // The magic of double, this guy is a flexer
        size_t index = ((double)rand() / RAND_MAX) * (sizeof(charset) - 1);
        *buffer++ = charset[index];
    }

    *buffer = '\0';
}


Suite *my_string_suite(void)
{
    Suite *s = suite_create("my_string_suite");

    tcase_hack(s, setup, NULL, test_my_strlen_1);
    tcase_hack(s, setup, NULL, test_my_strlen_2);
    tcase_hack(s, setup, NULL, test_my_strlen_3);
    tcase_hack(s, setup, NULL, test_my_strlen_4);
    tcase_hack(s, setup, NULL, test_my_strlen_5);

    tcase_hack(s, setup, NULL, test_my_strncmp_equal);
    tcase_hack(s, setup, NULL, test_my_strncmp_null_terminator_in_middle_almost_same);
    tcase_hack(s, setup, NULL, test_my_strncmp_null_terminator_in_middle_different);
    tcase_hack(s, setup, NULL, test_my_strncmp_random);
    tcase_hack(s, setup, NULL, test_my_strncmp_letters_shuffled);
    tcase_hack(s, setup, NULL, test_my_strncmp_first_half_same_end_different_large_n);
    tcase_hack(s, setup, NULL, test_my_strncmp_first_half_same_end_different_small_n);
    tcase_hack(s, setup, NULL, test_my_strncmp_almost_equal);

    tcase_hack(s, setup, NULL, test_my_strncpy_basic);
    tcase_hack(s, setup, NULL, test_my_strncpy_destination_larger);
    tcase_hack(s, setup, NULL, test_my_strncpy_random_basic_256);
    tcase_hack(s, setup, NULL, test_my_strncpy_null_terminator_in_middle);
    tcase_hack(s, setup, NULL, test_my_strncpy_overwrite);
    tcase_hack(s, setup, NULL, test_my_strncpy_adv);

    tcase_hack(s, setup, NULL, test_my_strncat_basic);
    tcase_hack(s, setup, NULL, test_my_strncat_random);
    tcase_hack(s, setup, NULL, test_my_strncat_null_terminator);
    tcase_hack(s, setup, NULL, test_my_strncat_with_empty_string_first);
    tcase_hack(s, setup, NULL, test_my_strncat_with_empty_string_second);

    tcase_hack(s, setup, NULL, test_my_memset_basic);
    tcase_hack(s, setup, NULL, test_my_memset_random_n);
    tcase_hack(s, setup, NULL, test_my_memset_replace_char);
    tcase_hack(s, setup, NULL, test_my_memset_replace_all);
                
    tcase_hack(s, setup, NULL, test_remove_first_instance_basic);
    tcase_hack(s, setup, NULL, test_remove_first_instance_random);
    tcase_hack(s, setup, NULL, test_remove_first_instance_instance_as_first_char);
    tcase_hack(s, setup, NULL, test_remove_first_instance_one_char_string);
    tcase_hack(s, setup, NULL, test_remove_first_instance_no_instances);

    tcase_hack(s, setup, NULL, test_replace_character_with_string_basic);
    tcase_hack(s, setup, NULL, test_replace_character_with_string_random);
    tcase_hack(s, setup, NULL, test_replace_character_with_string_no_character_found);
    tcase_hack(s, setup, NULL, test_replace_character_with_string_multiple_characters);
    tcase_hack(s, setup, NULL, test_replace_character_with_string_single_char_replace_string);
    tcase_hack(s, setup, NULL, test_replace_character_with_string_empty_replace_string);

    tcase_hack(s, setup, NULL, test_remove_first_character_random);
    tcase_hack(s, setup, NULL, test_remove_first_character_one_char_string);
    return s;
}

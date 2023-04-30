/**
 * @file main.c
 * @brief testing and debugging functions written in tl04.c
 * @date 2023-04-xx
 */

// You may add and remove includes as necessary to help facilitate your testing
#include <stdio.h>
#include "tl04.h"

/** main
 *
 * @brief used for testing and debugging functions written in tl04.c
 *
 * @param void
 * @return 0 on success
 */
int main(void)
{
    printf("Hello, I will be printed when you run %s and %s!\n", "make tl04", "./tl04");

    // Let's test tl04.c
    printf("We can access stack_top as shown in this line, %d\n", stack_top == NULL);
    struct post_t post;
    post.question = NULL;
    int success = stack_push("What is a stack?", TIMED_LAB_4);
    printf("Success? %d\n", success);
}


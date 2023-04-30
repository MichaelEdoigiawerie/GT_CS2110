
#include <stdio.h>
#include <check.h>
#include <stddef.h>
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define RETURN_ERROR_VALUE -100

// Suites
extern Suite *tl4_suite(void);

// Define a check containing only one test case of the same name
#define tcase_singleton(suite, setup_fixture, teardown_fixture, func)   \
    {                                                                   \
        TCase *tc = tcase_create(STRINGIFY(func));                      \
        tcase_add_checked_fixture(tc, setup_fixture, teardown_fixture); \
        tcase_add_test(tc, func);                                       \
        suite_add_tcase(s, tc);                                         \
    }

/******************************************************************************/
/**************************** tl4 Header Info *********************************/
/******************************************************************************/

#define DEBUG_FLAG "-d"

#include "../tl04.h"

extern const char *questions[7];
extern void _initialize_stack(int _capacity);
extern int _stack_push(const char *question, enum category_t category);
extern int _stack_pop(struct post_t *data);
extern int _destroy_stack(void);

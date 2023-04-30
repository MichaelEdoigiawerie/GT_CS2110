#include "../list.h"

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define tcase_hack(suite, setup_fixture, teardown_fixture, func) { \
    TCase *tc = tcase_create(STRINGIFY(func)); \
    tcase_add_checked_fixture(tc, setup_fixture, teardown_fixture); \
    tcase_add_test(tc, func); \
    suite_add_tcase(s, tc); \
}

extern char *_malloc_string(char* str);
extern double *_malloc_grades(int num, double *grades);

extern LinkedList *_create_list(void);

extern void _push_front(LinkedList *list, User *new_user);
extern User *_pop_front(struct linked_list *list);

extern Node *_create_node(User *user);
extern User *_create_student_user(char *name, int num_classes, double *grades);
extern User *_create_instructor_user(char *name, double salary);
extern User *_create_user(char *name, UserType type, UserUnion data);

extern int user_equal(User *u1, User *u2);
extern int student_equal(Student s1, Student s2);
extern int instructor_equal(Instructor i1, Instructor i2);

extern void _free_user(User *user);
extern void _free_node(Node* node);
extern void _free_list(LinkedList *list);
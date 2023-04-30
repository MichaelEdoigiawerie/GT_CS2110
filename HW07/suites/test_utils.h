
#include <stdio.h>
#include <check.h>
#include <stddef.h>
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define RETURN_ERROR_VALUE -100

// Suites
extern Suite *hw7_suite(void);
extern Suite *my_string_suite(void);
extern Suite *cryptography_suite(void);
extern struct Queue oh_queue;

#define tcase_hack(suite, setup_fixture, teardown_fixture, func)        \
    {                                                                   \
        TCase *tc = tcase_create(STRINGIFY(func));                      \
        tcase_add_checked_fixture(tc, setup_fixture, teardown_fixture); \
        tcase_add_test(tc, func);                                       \
        suite_add_tcase(s, tc);                                         \
    }

/******************************************************************************/
/**************************** hw7 Header Info *********************************/
/******************************************************************************/

#define UNUSED_PARAM(x) ((void) x) // This macro is only used for turning off compiler errors initially
#define UNUSED_FUNC(x) ((void) x)  // This macro is only used for turning off compiler errors initially

// Sizes for different arrays
#define MAX_NAME_LENGTH  30
#define MAX_QUEUE_LENGTH  30

// Success and failure codes for function return
#define SUCCESS 1
#define FAILURE 0

// Students should add their structs here
enum subject{Assembly, LC3, C, Other};
enum status{Completed, InProgress};

struct Topic{
    enum subject topicName;
    float questionNumber;
};

struct StudentData{
    char name[MAX_NAME_LENGTH];
    struct Topic topic;
};

struct Student {
    int customID[MAX_NAME_LENGTH];
    int queue_number;
    struct StudentData studentData;
};

struct OfficeHoursStats{
    int no_of_people_in_queue;
    int no_of_people_visited;
    char *currentStatus;
};

struct public_key {
    int n;
    int e;
};

struct Queue {
    struct OfficeHoursStats stats;
    struct Student students[MAX_QUEUE_LENGTH];
};

// Function prototypes for my_string.c
size_t my_strlen(const char *s);
int my_strncmp(const char *s1, const char *s2, size_t n);
char *my_strncpy(char *dest, const char *src, size_t n);
char *my_strncat(char *dest, const char *src, size_t n);
void *my_memset(void *str, int c, size_t n);
void remove_first_instance(char *str, char c);
void replace_character_with_string(char *str, char c, char *replaceStr);
void remove_first_character(char **str);

// Prototype oh_queue.c functions
int push(const char *studentName, const enum subject topicName, const float questionNumber, struct public_key pub_key);
int pop(void);
int group_by_topic(struct Topic topic, struct Student *grouped[]);
int power_and_mod(int b, int e, int n);
void hash(int *ciphertext, char *plaintext, struct public_key pub_key);
int update_student(struct Topic newTopic, int customID[]);
int remove_student_by_name(char *name);
int remove_student_by_topic(struct Topic topic);
void OfficeHoursStatus(struct OfficeHoursStats* resultStats );

/**
 * CS 2110 - Fall 2022 - Homework #9
 *
 * @author Michael Edoigiawerie
 *
 * list.c: Complete the functions!
 */

/**
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!-IMPORTANT-!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * For any function that must use malloc, if malloc returns NULL, the function
 * itself should return NULL if needs to return something (or return 1 if
 * the function returns a int).
 */

// Do not add ANY additional includes!!!
#include "list.h"

/* You should NOT have any global variables. */

/* The create_node function is static because this is the only file that should
   have knowledge about the nodes backing the linked List. */
static User *create_user(char *name, UserType type, UserUnion data);
static int create_student(int num_classes, double *grades, Student *dataOut); //completed for you
static int create_instructor(double salary, Instructor *dataOut); //completed for you
static Node *create_node(char *name, UserType type, UserUnion data);
static int user_equal(const User *user1, const User *user2);

/** create_user
 *
 * Helper function that creates a User by allocating memory for it on the heap
 * and initializing with the passed in data. You MUST create a deep copy of
 * the data union. This means malloc'ing space for copies of each field in
 * UserUnion data.
 *
 * If malloc returns NULL, you should return NULL as well.
 *
 * If creating a student/instructor fails, you must free everything you've allocated
 * and return NULL
 *
 * You should call create_student() or create_instructor() in this function
 * to create a deep copy of the UserUnion depending on the UserType
 *
 * (hint: you cannot just assign UserUnion data to the data part of user)
 *
 * @param the fields of the User struct
 * @return a User, return NULL if malloc fails
 */
static User *create_user(char *name, UserType type, UserUnion data)
{
    User *newUser = (User *) malloc(sizeof(User));
    if (!newUser) return NULL;

    if (!name) newUser->name = name;
    else {
        newUser->name = (char *) malloc(strlen(name) + 1);
        if (!newUser->name) {
            free(newUser);
            return NULL;
        }
        strcpy(newUser->name, name);
    } 
    
    newUser->type = type;
    if (newUser->type == STUDENT) {
        int student = create_student(data.student.num_classes, data.student.grades, &newUser->data.student);
        if (student) {
            free(newUser);
            return NULL;
        }
    }
    if (newUser->type == INSTRUCTOR) {
        int instructor = create_instructor(data.instructor.salary, &newUser->data.instructor);
        if (instructor) {
            free(newUser);
            return NULL;
        }
    }

    return newUser;
}

/** create_student
 *
 * Helper function that creates a Student and allocates memory for the grade array on the heap.
 *
 * THIS FUNCTION IS IMPLEMENTED FOR YOU
 *
 * @param the fields of the Student struct, and an existing pointer to a student
 * @return 1 if malloc fails, 0 otherwise.
 */
static int create_student(int num_classes, double *grades, Student *dataOut)
{
    /***do not change anything in this function***/
    dataOut->num_classes = num_classes;
    dataOut->grades = NULL;
    if (grades != NULL) {
        if (!(dataOut->grades = (double *) malloc(sizeof(double)*num_classes))) return 1;
        memcpy(dataOut->grades, grades, sizeof(double)*num_classes);
    }
    return 0;
}

/** create_instructor
 *
 * Helper function that creates an Instructor.
 *
 * THIS FUNCTION IS IMPLEMENTED FOR YOU
 *
 * @param the fields of the Instructor struct, and an existing pointer to an instructor
 * @return 1 if malloc fails, 0 otherwise. (this function should always return 0 since
 * you won't need to malloc anything)
 */
static int create_instructor(double salary, Instructor *dataOut)
{
    /***do not edit anything in this function***/
    dataOut->salary = salary; //yes that's all this function does
    return 0;
}

/**
 * Helper method that completely frees a user struct
 * 
 * @param user The desired user to free
 */
static void free_user(User* user) {
    if(user->name) free(user->name);
    if (user->type == STUDENT && user->data.student.grades) {
        free(user->data.student.grades);
    }
    if (user) free(user);
}

/** create_node
  *
  * Helper function that creates a Node by allocating memory for it on the heap.
  * Be sure to set its next pointer to NULL.
  *
  * Remember that you need to malloc both the node itself and the User
  * that's contained in the struct. You might want to call create_user instead of
  * malloc'ing again.
  *
  * If malloc returns NULL for either of them, you should return NULL to indicate failure.
  * In the case that you successfully malloc one of them but fail to malloc the other one,
  * you should free the one you successfully malloc'd.
  *
  * @param the fields of the User struct
  * @return a Node
  */
static Node* create_node(char *name, UserType type, UserUnion data)
{
    Node *newNode = (Node *) malloc(sizeof(Node));
    User *newUser = create_user(name, type, data);
    if (!newNode || !newUser) {
        if (newNode) free(newNode);
        if (newUser) free_user(newUser);
        return NULL;
    }
    newNode->next = NULL;
    newNode->data = newUser;

    return newNode;
}

/** student_equal
 *
 * Helper function to compare two Student structs. You may find it ueful to call this helper
 * function from the user_equal function that you have to implement.
 *
 * @param the two Student structs to be compared
 * @return 1 if equal, 0 otherwise
 */
static int student_equal(const Student *student1, const Student *student2)
{
    if (student1->num_classes != student2->num_classes) return 0;
    if (student1->grades != student2->grades)
    {
        if (student1->grades == NULL || student2->grades == NULL) return 0;
        if (memcmp(student1->grades, student2->grades,
                student1->num_classes * sizeof(double)))
        {
            return 0;
        }
    }
    return 1;
}

/** user_equal
 * Helper function to help you compare two user structs.
 *
 * If the name, type, and union fields are all equal, you should return 1.
 * Otherwise, return 0.
 *
 * NOTE: struct members that are pointers may be null!
 * If two users both have null for a certain pointer, the pointers are
 * considered equal.
 * However, if either of the input users is NULL, you should return 0.
 *
 * You may find it useful to call the student_equal function defined above.
 *
 * Make sure you're using the right function when comparing the name.
 * Remember that you are allowed to use functions from string.h
 *
 * This is meant to be used as a helper function in 'contains'; it is not tested
 * by the autograder.
 *
 * @param the two User structs to be compared
 * @return 1 if equal, 0 otherwise
 */
static int user_equal(const User *user1, const User *user2)
{
    if (!user1 || !user2) return 0;
    if (user1->type == user2->type) {
        if (user1->name == user2->name || strcmp(user1->name, user2->name) == 0) {
            if (user1->type == INSTRUCTOR && user1->data.instructor.salary == user2->data.instructor.salary)
                return 1; 
            if (user1->type == STUDENT && student_equal(&user1->data.student, &user2->data.student)) return 1;
        }    
    }
    return 0;
}

/** create_list
 *
 * Creates a LinkedList by allocating memory for it on the heap.
 * Be sure to initialize size to zero and head to NULL.
 *
 * If malloc returns NULL, you should return NULL to indicate failure.
 *
 * @return a pointer to a new struct list or NULL on failure
 */
LinkedList *create_list(void)
{
    LinkedList *linkedList = (LinkedList *) malloc(sizeof(LinkedList));
    if (!linkedList) return NULL;
    linkedList->head = NULL;
    linkedList->size = 0;
    return linkedList;
}

/** push_front
 *
 * Adds the element at the front of the LinkedList.
 *
 * @param list a pointer to the LinkedList structure.
 * @param      the fields of the User struct
 * @return 1 if the LinkedList is NULL or if allocating the new node fails, 0 if
 * successful.
 */
int push_front(LinkedList *list, char *name, UserType type, UserUnion data)
{
    Node *newNode = create_node(name, type, data);
    if (!list || !newNode) {
        if (newNode) {
            free_user(newNode->data);
            free(newNode);
        }
        return 1;
    } 
    newNode->next = list->head;
    list->head = newNode;
    list->size = list->size + 1;
    return 0;
}

/** push_back
 *
 * Adds the element to the back of the LinkedList.
 *
 * @param list a pointer to the LinkedList structure.
 * @param      the fields of the User struct
 * @return 1 if the LinkedList is NULL or if allocating the new node fails, 0 if
 * successful.
 */
int push_back(LinkedList *list, char *name, UserType type, UserUnion data)
{
    Node *newNode = create_node(name, type, data);
    if (!list || !newNode) {
        if (newNode) {
            free_user(newNode->data);
            free(newNode);
        }
        return 1;
    }
    if (!list->head) {
        list->head = newNode;
        list->size = list->size + 1;
        return 0;
    }

    Node *curr = list->head;
    while (curr->next) curr = curr->next;
    curr->next = newNode;
    list->size = list->size + 1;
    return 0;
}

/** add_at_index
 *
 * Add the element at the specified index in the LinkedList. This index must lie in
 * the inclusive range [0,size].
 * For example, if you have no elements in the LinkedList,
 * you should be able to add to index 0 but no further.
 * If you have two elements in the LinkedList,
 * you should be able to add to index 2 but no further.
 *
 * @param list a pointer to the LinkedList structure
 * @param index 0-based, starting from the head in the inclusive range
 *              [0,size]
 * @param the fields of the User struct
 * @return 1 if the index is out of bounds or the LinkedList is NULL or malloc fails
 *         (do not add the data in this case)
 *         otherwise (on success) return 0
 */
int add_at_index(LinkedList *list, int index, char *name, UserType type, UserUnion data)
{
    Node *newNode = create_node(name, type, data);
    if (!list || !newNode || index < 0 || index > list->size) {
        free_user(newNode->data);
        free(newNode);
        return 1;
    }
    if (!list->head) {
        list->head = newNode;
        list->size = list->size + 1;
        return 0;
    } 

    if (index == 0) {
        newNode->next = list->head;
        list->head = newNode;
        list->size = list->size + 1;
        return 0;
    }

    Node *curr = list->head;
    int i = 0;
    while (i < index - 1) {
        curr = curr->next;
        i++;
    }
    Node *nextNode = curr->next;
    curr->next = newNode;
    newNode->next = nextNode;
    list->size = list->size + 1;
    return 0;
}

/** get
 *
 * Gets the data at the specified index in the LinkedList
 *
 * @param list a pointer to the LinkedList structure
 * @param index 0-based, starting from the head.
 * @param dataOut A pointer to a pointer used to return the data from the
 *        specified index in the LinkedList or NULL on failure.
 * @return 1 if dataOut is NULL or index is out of range of the LinkedList or
 *         the LinkedList is NULL, 0 (on success) otherwise
 */
int get(LinkedList *list, int index, User **dataOut)
{
    if (!dataOut || !list || index < 0 || index >= list->size) return 1;
    Node *curr = list->head;
    int i = 0;
    while (i < index) {
        curr = curr->next;
        i++;
    }
    *dataOut = curr->data;
    return 0;
}

/** contains
  *
  * Traverses the LinkedList, trying to see if the LinkedList contains some
  * data. We say the list contains some input if there exists some node with
  * equal data as the input.
  *
  * You should use 'user_equal' here to compare the data. Note that pointers are
  * allowed to be null!
  *
  * If there are multiple pieces of data in the LinkedList which are equal to
  * the "data" parameter, return the one at the lowest index.
  *
  *
  * @param list a pointer to the LinkedList structure
  * @param data The data, to see if it exists in the LinkedList
  * @param dataOut A pointer to a pointer used to return the data contained in
  *                the LinkedList or NULL on failure
  * @return int    0 if dataOut is NULL, the list is NULL, or the list
  *                does not contain data, else 1
  */
int contains(LinkedList *list, User *data, User **dataOut)
{
    if (!dataOut || !list) return 0;
    Node *curr = list->head;
    while (curr) {
        if (user_equal(curr->data, data)) {
            *dataOut = curr->data;
            return 1;
        }
        curr = curr->next;
    }
    *dataOut = NULL;
    return 0;
}

/** pop_front
  *
  * Removes the Node at the front of the LinkedList, and returns its data to the program user.
  *
  * @param list a pointer to the LinkedList.
  * @param dataOut A pointer to a pointer used to return the data in the first
  *                Node or NULL if the LinkedList is NULL or empty
  * @return 1 if dataOut is NULL (the LinkedList is NULL or empty), else (on success) 0
  */
int pop_front(LinkedList *list, User **dataOut)
{
    if (!dataOut) return 1;
    if (!list || !list->head) {
        *dataOut = NULL;
        return 1;
    }

    *dataOut = list->head->data;
    Node *ptr = list->head;
    list->head = list->head->next;
    free(ptr);
    list->size = list->size - 1;
    return 0;
}

/** pop_back
  *
  * Removes the Node at the back of the LinkedList, and returns its data to the program user.
  *
  * @param list a pointer to the LinkedList.
  * @param dataOut A pointer to a pointer used to return the data in the last
  *                Node or NULL if the LinkedList is NULL or empty
  * @return 1 if dataOut is NULL (the LinkedList is NULL or empty), else (on success) 0
  */
int pop_back(LinkedList *list, User **dataOut)
{
    if (!dataOut) return 1;
    if (!list || !list->head) {
        *dataOut = NULL;
        return 1;
    }

    if (!list->head->next) {
        *dataOut = list->head->data;
        Node *ptr = list->head;
        list->head = NULL;
        free(ptr);
        list->size = list->size - 1;
        return 0;
    }

    Node *curr = list->head;
    while (curr->next->next) curr = curr->next; 
    *dataOut = curr->next->data;
    Node *ptr = curr->next;
    curr->next = NULL;
    free(ptr);
    list->size = list->size - 1;
    return 0;
}


/** remove_at_index
 *
 * Remove the element at the specified index in the LinkedList.
 *
 * @param list a pointer to the LinkedList structure
 * @param dataOut A pointer to a pointer used to return the data in the last
 *                Node or NULL if the LinkedList is NULL or empty
 * @param index 0-based, starting from the head in the inclusive range
 *              [0,size-1]
 * @return 1 if the index is out of bounds, the LinkedList is NULL or
 *         the dataOut is NULL
 *         otherwise return 0
 */
int remove_at_index(LinkedList *list, User **dataOut, int index)
{
    if (!dataOut) return 1;
    if (!list || !list->head) {
        *dataOut = NULL;
        return 1;
    }
    if (index < 0 || index > list->size - 1) return 1;

    if (index == 0) {
        *dataOut = list->head->data;
        Node *ptr = list->head;
        list->head = list->head->next;
        free(ptr);
        list->size = list->size - 1;
        return 0;
    }

    Node *curr = list->head;
    int i = 0;
    while (i < index - 1) {
        curr = curr->next;
        i++;
    }
    Node* newNext = curr->next->next;
    *dataOut = curr->next->data;
    Node* ptr = curr->next;
    free(ptr);
    curr->next = newNext;
    list->size = list->size - 1;
    return 0;
}

/** empty_list
 *
 * Empties the LinkedList. After this is called, the LinkedList should be
 * empty. This does NOT free the LinkedList struct itself, just all nodes and
 * data within. Make sure to check that the list is not NULL before
 * using it.
 *
 * Once again, the things that need to be freed after this function are:
 * - the nodes
 * - the user structs within the nodes
 * - all pointers in the user struct (and the union within)
 *
 * However, if you're using other functions you've written (which you should be),
 * those functions might take care of some of the freeing for you.
 *
 * You may call free on a char pointer as well as a struct pointer.
 *
 * If 'list' is NULL, simply return.
 *
 * @param list a pointer to the LinkedList structure
 */
void empty_list(LinkedList *list)
{
    if (!list || !list->head) return;
    Node *curr = list->head;
    while (curr) {
        Node* nextNode = curr->next;
        free_user(curr->data);
        free(curr);
        curr = nextNode;
    }
    list->size = 0;
    list->head = NULL;
}

/** num_passing_all_classes
 *
 * Traverses the LinkedList, counting the number of Students that are passing
 * all of their classes. We say a student is passing if their grade is greater
 * than or equal to 60.
 *
 * You should make sure your code only counts students, as instructors cannot be
 * passing or failing a class!
 *
 * If a student is not taking any classes, they are NOT considered to be passing
 * all their classes.
 *
 * @param list a pointer to the LinkedList structure
 * @param dataOut A pointer to int used to return the count of students passing all
 *                of their classes
 *                or -1 if the LinkedList is NULL or empty
 * @return 1 if the LinkedList is NULL or empty, else (on success) 0
 */
int num_passing_all_classes(LinkedList *list, int *dataOut)
{
    if (!list || !list->head) {
        *dataOut = -1;
        return 1;
    }
    int count = 0;
    for (Node *curr = list->head; curr; curr = curr->next) {
        if (curr->data->type == STUDENT) {
            Student studentData = curr->data->data.student;
            int passed = 0;
            for (int i = 0; i < studentData.num_classes; i++) {
                if (*(studentData.grades + i) >= 60) passed++;
                else break;
            }
            if (passed == studentData.num_classes) count++;
        }
    }
    *dataOut = count;
    return 0;
}

/** get_average_salary
 *
 * Traverses the LinkedList, computing the average of all instructor salaries.
 *
 * You should make sure your code only looks at instructors, as students do not have salaries!
 *
 * If there are no instructors in the list, place a 0 at dataOut.
 *
 * @param list a pointer to the LinkedList structure
 * @param dataOut A pointer to double used to return the average salary of the instructors
 *                or -1 if the LinkedList is NULL or empty
 * @return 1 if the LinkedList is NULL or empty, else (on success) 0
 */
int get_average_salary(LinkedList *list, double *dataOut)
{
    if (!list || !list->head) {
        *dataOut = -1;
        return 1;
    }
    double sum = 0;
    double instructors = 0;
    for (Node *curr = list->head; curr; curr = curr->next) {
        if (curr->data->type == INSTRUCTOR) {
            Instructor iData = curr->data->data.instructor;
            sum += iData.salary;
            instructors++;
        }
    }
    *dataOut = instructors > 0 ? sum / instructors : 0;
    return 0;
}

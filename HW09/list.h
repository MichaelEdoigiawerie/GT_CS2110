/**
 * CS 2110 - Fall 2022 - Homework #9
 *
 * Do not modify this file!
 *
 * struct list.h
 */

#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <string.h>

// This is just to remove compiler warnings associated with unused variables.
// Delete calls to this as you implement functions.
#define UNUSED(x) ((void)(x))

/**************************
 ** Datatype definitions **
 **************************/

/*
 * The following structs define the LinkedList and Node for use in list.c. DO NOT MODIFY THESE STRUCT DEFINITIONS
 */

typedef struct student
{
    int num_classes; 
    double *grades; //length of grades is num_classes
} Student;

typedef struct instructor
{
    double salary;
} Instructor;

typedef enum user_type {
    STUDENT,
    INSTRUCTOR
} UserType;

typedef union {
  Student student;
  Instructor instructor;
} UserUnion;

typedef struct user
{
    char *name;
    UserType type;
    UserUnion data;
} User;


typedef struct node
{
    struct node *next;  //pointer to the next node
    User *data;          //pointer to the node's string data
} Node;


typedef struct linked_list
{
    Node *head; // Head pointer either points to a node with data or if the struct list is empty NULL
    int size;   // Size of the struct list
} LinkedList;

/***************************************************
** Prototypes for struct list library functions.  **
**                                                **
** For more details on their functionality,       **
** check struct list.c.                           **
***************************************************/

/* Creating */
LinkedList *create_list(void);

/* Adding */
int push_front(LinkedList *list, char *name, UserType type, UserUnion data);
int push_back(LinkedList *list, char *name, UserType type, UserUnion data);
int add_at_index(LinkedList *list, int index, char *name, UserType type, UserUnion data);

/* Querying */
int get(LinkedList *list, int index, User **dataOut);
int contains(LinkedList *list, User *data, User **dataOut);

/* Removing */
int pop_front(LinkedList *list, User **dataOut);
int pop_back(LinkedList *list, User **dataOut);
int remove_at_index(LinkedList *list, User **dataOut, int index);
void empty_list(LinkedList *list);

/* Analysis */
int num_passing_all_classes(LinkedList *list, int *dataOut);
int get_average_salary(LinkedList *list, double *dataOut);

#endif

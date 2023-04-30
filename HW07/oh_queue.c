// DO NOT MODIFY THE INCLUDE(S) LIST
#include <stdio.h>
#include "oh_queue.h"

struct Queue oh_queue;

/** push
 * @brief Create a new student and push him
 * onto the OH queue
 * @param studentName pointer to the student's name
 * @param topicName topic the student has a question on
 * @param questionNumber hw question number student has a question on
 * @param pub_key public key used for calculating the hash for customID
 * @return FAILURE if the queue is already at max length, SUCCESS otherwise
 */
int push(const char *studentName, const enum subject topicName, const float questionNumber, struct public_key pub_key){
    struct Student student;
    struct StudentData studentData;

    studentData.topic.topicName = topicName;
    studentData.topic.questionNumber = questionNumber;
    my_strncpy(studentData.name, studentName, MAX_NAME_LENGTH - 1);
    if (studentData.name == NULL || oh_queue.stats.no_of_people_in_queue > MAX_QUEUE_LENGTH - 1) 
        return FAILURE;

    hash(student.customID, studentData.name, pub_key);
    student.queue_number = oh_queue.stats.no_of_people_in_queue;
    student.studentData = studentData;
    
    OfficeHoursStatus(&oh_queue.stats);
    oh_queue.students[oh_queue.stats.no_of_people_in_queue++] = student;
    return SUCCESS;
}

/** pop
 * @brief Pop a student out the OH queue
 * @return FAILURE if the queue is already at empty, SUCCESS otherwise
 */
int pop(void) {
    if (oh_queue.stats.no_of_people_in_queue == 0) return FAILURE;
    for (int i = 0; i < MAX_QUEUE_LENGTH - 1; i++) {
        oh_queue.students[i] = oh_queue.students[i + 1];
    }
    oh_queue.stats.no_of_people_in_queue--;
    oh_queue.stats.no_of_people_visited++;
    OfficeHoursStatus(&oh_queue.stats);
    return SUCCESS;
}

/** group_by_topic
 * @brief Push pointers to students, who match the given topic, to
 * the given array "grouped"
 * @param topic the topic the students need to match
 * @param grouped an array of pointers to students
 * @return the number of students matched
 */
int group_by_topic(struct Topic topic, struct Student *grouped[]) { 
    int count = 0;
    if (oh_queue.stats.no_of_people_in_queue == 0) return count;
    for (int i = 0; i < MAX_QUEUE_LENGTH; i++) {
        struct Topic t = oh_queue.students[i].studentData.topic;
        if (t.topicName == topic.topicName && t.questionNumber == topic.questionNumber) {
            grouped[count] = &oh_queue.students[i];
            count++;
        }
    }
    return count;
}

/** hash
 * @brief Creates a hash based on pub_key provided
 * @param ciphertext the pointer where you will store the hashed text
 * @param plaintext the originak text you need to hash
 * @param pub_key public key used for calculating the hash
 */
void hash(int *ciphertext, char *plaintext, struct public_key pub_key) {
    int i = 0;
    while (*(plaintext + i) != '\0') {
        *(ciphertext + i) = power_and_mod(*(plaintext + i), pub_key.e, pub_key.n);
        i++;
    }
}

/** update_student
 * @brief Find the student with the given ID and update his topic
 * @param customID a pointer to the id of the student you are trying to find
 * @param newTopic the new topic that should be assigned to him
 * @return FAILURE if no student is matched, SUCCESS otherwise
 */
int update_student(struct Topic newTopic, int *customID) {
    if (oh_queue.stats.no_of_people_in_queue == 0) return FAILURE;
    for (int i = 0; i < MAX_QUEUE_LENGTH; i++) {
        int *cid = oh_queue.students[i].customID;
        for (size_t j = 0; j < my_strlen(oh_queue.students[i].studentData.name); j++) {
            if (*(cid + j) == *customID) {
                oh_queue.students[i].studentData.topic = newTopic;
                return SUCCESS;
            }
        }
    }
    return FAILURE;
}

/** remove_student_by_name
 * @brief Removes first instance of a student with the given name
 * @param name the name you are searching for
 * @return FAILURE if no student is matched, SUCCESS otherwise
 */
int remove_student_by_name(char *name){
    int found = 0;
    int index = 0;
    if (oh_queue.stats.no_of_people_in_queue == 0) return FAILURE;
    for (int i = 0; i < MAX_QUEUE_LENGTH; i++) {
        char *n = oh_queue.students[i].studentData.name;
        if (my_strncmp(n, name, MAX_NAME_LENGTH) == 0) {
            found = 1;
            index = i;
            break;
        }
    }
    if (!found) return FAILURE;
    for (int j = index; j < MAX_QUEUE_LENGTH - 1; j++) {
        oh_queue.students[j] = oh_queue.students[j + 1];
    }
    oh_queue.stats.no_of_people_in_queue--;
    oh_queue.stats.no_of_people_visited++;
    OfficeHoursStatus(&oh_queue.stats);
    return SUCCESS;
}

/** remove_student_by_topic
 * @brief Remove all instances of students with the given topic
 * @param topic the topic you are trying to remove from the queue
 * @return FAILURE if no student is matched, SUCCESS otherwise
 */
int remove_student_by_topic(struct Topic topic) {
    int count = 0;
    int len = oh_queue.stats.no_of_people_in_queue;
    char* names[MAX_QUEUE_LENGTH];
    if (oh_queue.stats.no_of_people_in_queue == 0) return FAILURE;
    for (int i = 0; i < len; i++) {
        char *n = oh_queue.students[i].studentData.name;
        struct Topic t = oh_queue.students[i].studentData.topic;
        if (t.topicName == topic.topicName && t.questionNumber == topic.questionNumber) {
            names[count] = n;
            count++;
        }
    }
    if (count == 0) return FAILURE;
    for (int i = count - 1; i >= 0; i--) {
        remove_student_by_name(names[i]);
    }
    OfficeHoursStatus(&oh_queue.stats);
    return SUCCESS;
}

/** OfficeHoursStatus
 * @brief Updates the "currentStatus" field based on
 * whether or not all students in the queue have been helped
 * @param resultStats A pointer the OfficeHoursStats variable
 * you are to update
 */
void OfficeHoursStatus(struct OfficeHoursStats* resultStats){
    if (resultStats->no_of_people_in_queue == 0) resultStats->currentStatus = "Completed";
    else resultStats->currentStatus = "InProgress";
}

/*
 * Calculates (b^e)%n without overflow
 */
int power_and_mod(int b, int e, int n) {
    long int currNum = 1;
    for (int i = 0; i < e; i++) {
        currNum *= b;
        if (currNum >= n) {
            currNum %= n;
        }
    }
    return (int) (currNum % n);
}

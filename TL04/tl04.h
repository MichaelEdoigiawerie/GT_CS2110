/**
 * \mainpage Timed Lab 4: C and Dynamic Memory Allocation
 */

#ifndef TL04_H
#define TL04_H

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>


/**
 * \brief Marks unused variables
 */
#define UNUSED(x) (void)(x)

/**
 * The return values for your functions
 * \property SUCCESS
 * \property FAILURE
 */
#define SUCCESS (0)
#define FAILURE (1)


/**
 * \brief All the categories in EdSTEM students can possibly need help on
 * \see post_t
 */
enum category_t {
    GENERAL, LECTURE, LAB, TEXTBOOK, QUIZ_1, QUIZ_2, QUIZ_3, QUIZ_4,
    TIMED_LAB_1, TIMED_LAB_2, TIMED_LAB_3, TIMED_LAB_4,
    HOMEWORK_1, HOMEWORK_2, HOMEWORK_3, HOMEWORK_4, HOMEWORK_5,
    HOMEWORK_6, HOMEWORK_7, HOMEWORK_8, HOMEWORK_9, HOMEWORK_10,
    DOCKER, LOGISTICS
};

/**
 * \brief Post to be stored on the stack
 *
 * This type represents individual posts on the Ed Discussion stack. Each
 * post has a string for their question, as well as the category that they need
 * help on. For example, a post might have the question `"How do I implement a stack?"` 
 * and need help on `TIMED_LAB_4`.
 */
struct post_t {
    char *question;              ///< Question of the post as a string
    enum category_t category; ///< Category the question is a part of
};

/**
 * \brief Variables used to track the stack.
 *
 * The two integers (capacity and numPosts) represent the current 
 * maximum capacity of posts Ed Discussion can hold and the current number 
 * of posts respectively.
 * 
 * The pointer (stack_arr) points to the bottom of the stack, or in other words, the first element in the backing array.
 *
 * Initially, capacity is set to 5, numPosts is set to 0, and stack_arr is
 * pointing the first open spot at the bottom of the stack. The stack is initially
 * empty so stack_arr is originally pointing to garbage data.
 *
 * \property extern int capacity
 * \property extern int numPosts
 * \property extern struct post_t **stack_arr
 */
extern int capacity;
extern int numPosts;
extern struct post_t **stack_arr;

/**
 * \brief Provided function to initialize the stack
*/
void initialize_stack(void);

/**
 * \brief Add posts to the top of the stack
 *
 * This function will be called by client code to add a post to the top of
 * the stack. The caller will supply the question and category of the post to add.
 *
 * This function should allocate a [post_t] on the heap, and deep-copy all the data.
 * In particular, any pointers in the [post_t] will require their own dedicated memory allocation.
 * Make sure that all members of the [post_t] are set!
 *
 * If the stack is full, double the capacity of the stack and then add the new post to the stack.
 * 
 * Finally, insert the post onto the stack with the help of the [stack_arr] pointer.
 * Refer back to the PDF/diagram for specific details about how the stack works, 
 * and consider any edge cases.
 * 
 * This function should return `SUCCESS` if the post was added successfully.
 * If it fails, it should return `FAILURE` and leave the list unchanged. It
 * should fail if and only if:
 * * `malloc` or `realloc` fails,
 * * the post's question is `NULL`, or
 * * the post's question is an empty string.
 *
 * \param[in] question The question for the post
 * \param[in] category The category for the post
 * \return Whether the post was successfully added
 */
extern int stack_push(const char *question, enum category_t category);

/**
 * \brief Pop a question from the stack
 *
 * This function will be called by client code to remove a post from the
 * top of the stack. It will return whether a post was removed successfully,
 * and the post removed in that case.
 *
 * The way this function returns the post is using the data out technique.
 * This is to get around the limitation that functions may only have one return
 * value. As such, the caller will pass in a pointer where the post
 * should be stored. Then this function will store the returned post at that
 * pointer. Independently, it returns whether it succeeded via the normal path.
 * 
 * Finally, set the pointer of the post being popped to 'NULL' and update numPosts.
 * 
 * Refer back to the PDF/diagram for specific details about how to the stack 
 * works, and consider any edge cases.
 *
 * If this function succeeds, it should return `SUCCESS` and modify `*data` to
 * be pointing to the removed post. If it fails, it should return `FAILURE`
 * and leave both the stack and `*data` unchanged. It should fail if and only if:
 * * [data] is `NULL`, or
 * * the stack is empty.
 *
 * \param[out] data Where to put the removed post
 * \return Whether a post was successfully removed
 */
extern int stack_pop(struct post_t *data);

/**
 * \brief Destroy the whole stack
 *
 * This function will be called by client code to free the whole stack. This involves
 * freeing all elements associated with the stack: every pointer in the stack, every post
 * associated with the stack, and the data associated with each post. Finally, you will set
 * numPosts to 0.
 * 
 * When this function succeeds, it should return `SUCCESS`.
 */
extern int destroy_stack(void);

#endif // #ifndef TL04_H
/**
 * @file my_string.c
 * @author Michael Edoigiawerie
 * @collaborators NAMES OF PEOPLE THAT YOU COLLABORATED WITH HERE
 * @brief Your implementation of these famous 3 string.h library functions!
 *
 * NOTE: NO ARRAY NOTATION IS ALLOWED IN THIS FILE
 *
 * @date 2023-03-15
 */

#include <stddef.h>
#include "my_string.h"
/**
 * @brief Calculate the length of a string
 *
 * @param s a constant C string
 * @return size_t the number of characters in the passed in string
 */
size_t my_strlen(const char *s)
{
    size_t len = 0;
    while (*(s + len) != '\0') len++;
    return len;
}

/**
 * @brief Compare two strings
 *
 * @param s1 First string to be compared
 * @param s2 Second string to be compared
 * @param n First (at most) n bytes to be compared
 * @return int "less than, equal to, or greater than zero if s1 (or the first n
 * bytes thereof) is found, respectively, to be less than, to match, or be
 * greater than s2"
 */
int my_strncmp(const char *s1, const char *s2, size_t n)
{
    size_t i = 0;
    while (i < n) {
        char a = *(s1 + i);
        char b = *(s2 + i);
        if (a == '\0' || b == '\0' || a < b || a > b) return a - b;
        i++;
    }
    return 0;
}

/**
 * @brief Copy a string
 *
 * @param dest The destination buffer
 * @param src The source to copy from
 * @param n maximum number of bytes to copy
 * @return char* a pointer same as dest
 */
char *my_strncpy(char *dest, const char *src, size_t n)
{
    size_t i = 0;
    while (i < n) {
        if (*(src + i) != '\0') *(dest + i) = *(src + i);
        else *(dest + i) = '\0';
        i++;
    }
    return dest;
}

/**
 * @brief Concatenates two strings and stores the result
 * in the destination string
 *
 * @param dest The destination string
 * @param src The source string
 * @param n The maximum number of bytes from src to concatenate
 * @return char* a pointer same as dest
 */
char *my_strncat(char *dest, const char *src, size_t n)
{
    size_t i = 0;
    size_t j = 0;
    while (*(dest + i) != '\0') i++;
    while (j < n) {
        if (*(src + j) != '\0') *(dest + i) = *(src + j);
        i++;
        j++;
    }
    *(dest + i) = '\0';
    return dest;
}

/**
 * @brief Copies the character c into the first n
 * bytes of memory starting at *str
 *
 * @param str The pointer to the block of memory to fill
 * @param c The character to fill in memory
 * @param n The number of bytes of memory to fill
 * @return char* a pointer same as str
 */
void *my_memset(void *str, int c, size_t n)
{
    size_t i = 0;
    char *s = str;
    while (i < n) {
        *(s + i) = (char) c;
        i++;
    }
    return s;
}

/**
 * @brief Finds the first instance of c in str
 * and removes it from str in place
 *
 * @param str The pointer to the string
 * @param c The character we are looking to delete
 */
void remove_first_instance(char *str, char c) { 
    size_t i = 0;
    int found = 0;
    while (*(str + i) != '\0') {
        if (*(str + i) == c) found = 1;
        if (found) *(str + i) = *(str + i + 1);
        i++;
    }
}

/**
 * @brief Finds the first instance of c in str
 * and replaces it with the contents of replaceStr
 *
 * @param str The pointer to the string
 * @param c The character we are looking to delete
 * @param replaceStr The pointer to the string we are replacing c with
 */
void replace_character_with_string(char *str, char c, char *replaceStr) {
    size_t len = my_strlen(replaceStr);
    size_t i = 0;
    size_t index = 0;
    if (len == 0) {
        remove_first_instance(str, c);
        return;
    } 
    while (*(str + i) != c) {
        if (*(str + i) == '\0') return;
        i++;
    }
    index = i;
    i = my_strlen(str);
    while (i > index) {
        *(str + len + i - 1) = *(str + i);
        i--;
    }
    i = index;
    while (i - index < len) {
        *(str + i) = *(replaceStr + i - index);
        i++;
    }
}

/**
 * @brief Remove the first character of str (ie. str[0]) IN ONE LINE OF CODE.
 * No loops allowed. Assume non-empty string
 * @param str A pointer to a pointer of the string
 */
void remove_first_character(char **str) {
    *str = *str + 1;
}
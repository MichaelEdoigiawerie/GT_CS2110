#ifndef MY_STRING_H
#define MY_STRING_H

#include <stddef.h>

#define UNUSED_PARAM(x) ((void) x) // This macro is only used for turning off compiler errors initially

// Function prototypes for my_string.c
size_t my_strlen(const char *s);
int my_strncmp(const char *s1, const char *s2, size_t n);
char *my_strncpy(char *dest, const char *src, size_t n);
char *my_strncat(char *dest, const char *src, size_t n);
void *my_memset(void *str, int c, size_t n);
void remove_first_instance(char *str, char c);
void replace_character_with_string(char *str, char c, char *replaceStr);
void remove_first_character(char **str);

#endif
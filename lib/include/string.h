#ifndef STRING_H
#define STRING_H

#include <stddef.h>
#include <stdint.h>

/**
 * @struct String
 */
typedef struct {
    char* chars; //< characters
    int32_t length; //< length of string
} String;

/**
 * @brief Copies the values of num bytes from src to dest
 * 
 * @param dest the location where bytes are to be copied to
 * @param src the location from which the bytes are to be copied
 * @param bytes the number of bytes to be copied
 * @return void* 
 */
void *memcpy(void *dest, const void *src, size_t num);

/**
 * @brief sets the first num of bytes of ptr with value
 * 
 * @param ptr the pointer of the block of memory to fill
 * @param value the value to be set will be converted to uint8_t
 * @param num number of bytes to be set
 * @return void* 
 */
void *memset(void *ptr, int value, size_t num);

/**
 * @brief moves block of memory
 * 
 * @param dest the destination to be moved to
 * @param src the source of bytes to be moved
 * @param num the number of bytes to be copied
 * @return void* 
 */
void *memmove(void *dest, const void *src, size_t num);

/**
 * @brief compares two blocks of memory
 * 
 * @param ptr1 pointer of block of memory
 * @param ptr2 pointer of block of memory
 * @param num number of bytes to be compared 
 * @return <0 if the first byte that does not match has a lower value in ptr1 than ptr2
 * @return 0 if both blocks match
 * @return >0 if the first byte that does not match has a higher value in ptr1 than ptr2
 */
int memcmp(const void *ptr1, const void *ptr2, size_t num);

/**
 * @brief compares two c strings
 * 
 * @param str1 string to be compared
 * @param str2 string to be compared
 * @return 0 if they are the same
 * @return <0 if the first character that does not match has a lower value in str1 than str2
 * @return >0 if the first character that does not match has a higher value in str1 than str2
 */
int cstrcmp(const char *str1, const char* str2);

int cstrncmp(const char *str1, const char* str2, size_t length);

char string_get(const String* str, size_t index);



#endif // STRING_H

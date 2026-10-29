#ifndef STDLIB_H
#define STDLIB_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void reverse(char str[], int length);

char* itoa(int num, char* str, int base);

/**
 * @brief converts a byte into a null terminated hexadecimal encoded string
 * 
 * @param byte the byte to be converted
 * @param out string which must be atleast 5 chars long
 */
void binary_to_hexstr(uint8_t byte, char* out);

#endif // STDLIB_H
 
#ifndef STDLIB_H
#define STDLIB_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


void reverse(char str[], int length)
{
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        end--;
        start++;
    }
}

char* itoa(int num, char* str, int base) {
    int i = 0;
    bool isNegative = false;
 
    /* Handle 0 explicitly, otherwise empty string is
     * printed for 0 */
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
 
    // In standard itoa(), negative numbers are handled
    // only with base 10. Otherwise numbers are
    // considered unsigned.
    if (num < 0 && base == 10) {
        isNegative = true;
        num = -num;
    }
 
    // Process individual digits
    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }
 
    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';
 
    str[i] = '\0'; // Append string terminator
 
    // Reverse the string
    reverse(str, i);
 
    return str;
}


/**
 * @brief converts a byte into a null terminated hexadecimal encoded string
 * 
 * @param byte the byte to be converted
 * @param out string which must be atleast 5 chars long
 */
void binary_to_hexstr(uint8_t byte, char* out) {
    // 8 bit value
    // for hexadecimal we need to seperate into groups of four;
    //  byte & 0x0F; least significant nibble;
    //  byte & 0xF0; most significant nibble;

    out[0] = '0';
    out[1] = 'x';


    uint8_t LSN = (byte & 0x0F); // least significant nibble
    uint8_t MSN = (byte & 0xF0) >> 4; // most significant nibble

    if((MSN) > 9 ) {
        out[2] = 'A' + (MSN - 10);
    } else {
        out[2] = '0' + MSN;
    }

    if((byte & 0x0F) > 9) {
        out[3] = 'A' +  (LSN - 10);
    } else {
        out[3] = '0' + LSN;
    }

    out[4] = '\0';
}

#endif // STDLIB_H
 
#ifndef KERNEL_TTY_H
#define KERNEL_TTY_H

#include <stddef.h>

/**
 * @brief Intializes the terminal
 */
void terminal_init(void);

/**
 * @brief display a character to the terminal
 *  
 * @param c the character to be displayed
 */
void terminal_putchar(char c);

/**
 * @brief Writes a specified chunk of data to the screen
 * 
 * @param data the data to be displayed
 * @param size the size of the chunk of data
 */
void terminal_write(const char* data, size_t size);

/**
 * @brief Writes a \0 terminated string to the screen
 * 
 * @param data the string to be displayed
 */
void terminal_writestring(const char* data);



#endif //KERNEL_TTY_H
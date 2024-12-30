#ifndef TTY_H
#define TTY_H

#include "limine.h"
#include "psf.h"
#include <stddef.h>

/**
 * @brief initialises the terminal
 * @param font the psf1 font to use
 * @param buffer the framebuffer to output to
 * @param height height of the framebuffer
 * @param widht width of the framebuffer
 */
void terminal_init(
        struct psf1_font* font,
        struct limine_framebuffer* buffer,
        size_t height, size_t width
        );


/**
 * @brief displays a character to the screen
 * @param c the character to be displayed
 */
void terminal_putchar(char c);

/**
 * @brief write the given data of the specified size to the screen
 * @param data the data to be displayed 
 * @param size the sizw of the data
 */
void terminal_write(const char* data, size_t size);

/**
 * @brief writes a \0 terminated string to the screen
 * @param data the string to be printed
 */
void terminal_writestring(const char* data);

#endif // TTY_H

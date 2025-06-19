#include "tty.h"
#include "limine.h"
#include "psf.h"
#include "vga.h"
#include <stddef.h>

static size_t terminal_row, terminal_column;
static size_t terminal_height, terminal_width;
static struct psf1_font* terminal_font;
static struct limine_framebuffer* terminal_buffer;
static int terminal_color = 0x00ff00;

static uint8_t* glyphs[256];

void terminal_init(
    struct psf1_font* font,
    struct limine_framebuffer* buffer,
    size_t height,
    size_t width
    ) {

    terminal_column = 0;
    terminal_row = 0;
    terminal_height = height;
    terminal_width = width;

    terminal_font = font;
    terminal_buffer = buffer;

    map_to_unicode_psf1(terminal_font, glyphs);
    terminal_initalised = true;
}

void terminal_putchar(char c) { 
    if(c == '\n') {
        terminal_row++;
        terminal_column = 0;
        return;
    }

    putfont(
        terminal_buffer,
        glyphs[(int)c],
        terminal_column * 8,
        terminal_row * 16,
        terminal_color
    ); 

    terminal_column++;
    if (terminal_column > terminal_width) {
        terminal_row++;
        terminal_column = 0;
    }
}

void terminal_write(const char* data, size_t size) {
    for(size_t i = 0; i < size; i++) {
        terminal_putchar(data[i]);
    }
}

void terminal_writestring(const char* data) {
    while(*data != '\0') {
        terminal_putchar(*data++);
    }
}

void terminal_println(const char* data) {
    terminal_writestring(data);
    terminal_putchar('\n');
}





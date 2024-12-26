#include "limine.h"
#include <stddef.h>
#include <stdint.h>
#include <vga.h>

void putpixel(struct limine_framebuffer* framebuffer, int x, int y, int color) {
    uint32_t index =  (y * framebuffer->pitch) + (x * (framebuffer->bpp / 8));
    volatile uint8_t* buff = (uint8_t *) framebuffer->address;
    
    buff[index] = color & 0xff;
    buff[index + 1] = (color >> 8) & 0xff;
    buff[index + 2] = (color >>  16) & 0xff;
}

void putfont(
    struct limine_framebuffer *framebuffer,
    uint8_t* glyph,
    int _x,
    int _y) {

    uint8_t* start = glyph;
    int index = 0;
    // one glyph is 16 bytes long
    while (index < 16) {
        uint8_t line = *start;

        for(size_t i = 0; i < 8; i ++) {
            if(line & 0b10000000) {
                putpixel(framebuffer, _x+i, _y+index, 0x00ff00);
            }
            line <<= 1;
        }
        index++;
        start++;
    }
}

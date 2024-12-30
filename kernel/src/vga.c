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
    int _y,
    int color) {

    uint8_t* start = glyph;
    int index = 0;
    // one glyph is 16 bytes long
    while (index < 16) {
        uint8_t line = *start;

        for(size_t i = 0; i < 8; i ++) {
            // since psf1 fonts are encoded using little endian
            // we must read the bits in this order
            // that is from left to right
            // what i tried to do before was 
            // (line & 0x01) which mirrored the characters
            if(line & 0b10000000) {
                putpixel(framebuffer, _x+i, _y+index, color);
            }
            line <<= 1;
        }
        index++;
        start++;
    }
}

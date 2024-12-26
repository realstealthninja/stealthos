#ifndef VGA_H
#define VGA_H

#include "limine.h"
#include "psf.h"

/**
 * @brief places a pixel with the specified coordinates and color
 *  
 * @param framebuffer the framebuffer in which the pixel is to be displayed
 * @param x the x coordinate (relative to top left)
 * @param y the y coordinate (relative to top left)
 * @param color color of the pixel
 */
void putpixel(struct limine_framebuffer* framebuffer, int x, int y, int color);


void putfont(
    struct limine_framebuffer* framebuffer,
    uint8_t* glyph,
    int x,
    int y);

#endif // VGA_H

#ifndef VGA_H
#define VGA_H

#include "limine.h"

/**
 * @brief places a pixel with the specified coordinates and color
 *  
 * @param framebuffer the framebuffer in which the pixel is to be displayed
 * @param x the x coordinate (relative to top left)
 * @param y the y coordinate (relative to top left)
 * @param color color of the pixel
 */
void putpixel(struct limine_framebuffer* framebuffer, int x, int y, int color);

/**
 * @brief places a glpyh with the specified coordinates and color
 * 
 * @param framebuffer the framebuffer in which the glpyh is to be placed
 * @param glyph the glyph which is to be displayed
 * @param x the x coordinate in pixels
 * @param y the y coordinate in pixels
 * @param color the color the glyph should be displayed in
 */
void putfont(
    struct limine_framebuffer* framebuffer,
    uint8_t* glyph,
    int x,
    int y,
    int color);

#endif // VGA_H

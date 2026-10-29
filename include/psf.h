#ifndef PSF_H
#define PSF_H

#include <stdint.h>

/**
 * @brief Describes a psf1 font header
 * @link https://en.wikipedia.org/wiki/PC_Screen_Font
 */
 // height == width
struct psf1_header {
    uint8_t magic[2];
    uint8_t mode;
    uint8_t glyph_size;
} __attribute__((packed));

/**
 * @brief Describes a psf2 font header
 * @link https://en.wikipedia.org/wiki/PC_Screen_Font
 */
struct psf2_header {
    uint32_t magic;
    uint32_t version;
    uint32_t header_size;
    uint32_t flags;
    uint32_t length;
    uint32_t glyph_size;
    uint32_t height;
    uint32_t width;
} __attribute__((packed));

struct psf2_font {
    struct psf2_header *header;
    void *glyph_buffer;
    uint16_t *unicode_table;
} __attribute__((packed));


struct psf1_font {
    struct psf1_header *header;
    void *glyph_buffer;
    uint16_t *unicode_table;
} __attribute__((packed));

void map_to_unicode_psf1(struct psf1_font* font, uint8_t** unicode_table);

#endif // PSF_H

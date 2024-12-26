#include <psf.h>
#include <stddef.h>

void map_to_unicode_psf1(struct psf1_font* font, uint8_t** unicode_table) {
    // check if font has unicode
    if (!(font->header->mode & 0x02)) {
        unicode_table = NULL;
        return;
    }

    // the unicode table is after the glyphs
    uint8_t*  glyphs = font->glyph_buffer;

    // each glyph is 16 bytes long
    // to map each character to a glyph
    // we iterate through the table
    // and for each character present in the table
    // we put the values into an array
    

    // we know there are 256 glyphs
    for(size_t i = 0; i < 256; i++){
        unicode_table[i] = glyphs + i * 16;
    }
}

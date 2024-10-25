#include "../includes/io.h"

unsigned int frame_buffer_index = 0;

char* frame_buffer = (char*) 0x000B8000; // memory mapped address for the frame buffer;

void frame_buffer_write_cell(
    unsigned int index,
    char character,
    unsigned char foreground,
    unsigned char background) {
    frame_buffer[index] = character;
    frame_buffer[index + 1] = ((foreground & 0x0f) << 4) | (background & 0x0f);
}

void fb_move_cursor(unsigned int pos) {
    outb(COMMAND_PORT, HIGH_BYTE_COMMAND);
    outb(DATA_PORT, ((pos >> 8) & 0x00FF));
    outb(COMMAND_PORT, LOW_BYTE_COMMAND);
    outb(DATA_PORT, pos & 0x00FF);
}

void write(char *string, unsigned int len) {
    for(unsigned int i = frame_buffer_index; i < len; i++) {
        frame_buffer_write_cell(i * 2, string[i], BLACK, WHITE);
    }
    frame_buffer_index += (len-1);
}

#include "../includes/io.h"

volatile char* frame_buffer = (char*) 0x000B8000; // memory mapped address for the frame buffer;
int x = 0, y = 0;



void frame_buffer_write_cell(
    char character,
    unsigned char foreground,
    unsigned char background) {
    if (character == '\n') {
        y++;
        x=0;
        return;
    }
    frame_buffer[y * 80 + x++] = character;
    frame_buffer[y * 80 + x++] = ((background & 0x0f) << 4) | (foreground & 0x0f);
    if (x > 80) {
        y++;
        x = 0;
    }
}

void fb_move_cursor(unsigned int pos) {
    outb(COMMAND_PORT, HIGH_BYTE_COMMAND);
    outb(DATA_PORT, ((pos >> 8) & 0x00FF));
    outb(COMMAND_PORT, LOW_BYTE_COMMAND);
    outb(DATA_PORT, pos & 0x00FF);
}

void write(char *string) {
    while(*string != '\0') {
        frame_buffer_write_cell(*string++, WHITE, BLACK);
    }
}

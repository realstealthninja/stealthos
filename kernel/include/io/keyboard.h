#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define KEYBOARD_BUFFER_SIZE 256

typedef struct {
    uint8_t code;
} key_event;

static key_event keyboard_buffer[KEYBOARD_BUFFER_SIZE];
static size_t keyboard_buffer_index = 0;

void keyboard_handler(void);


#endif // KEYBOARD_H

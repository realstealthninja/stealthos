#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define KEYBOARD_BUFFER_SIZE 256

// KEYBOARD COMMANDS

#define SET_LED 0xED
#define ECHO    0xEE
#define SCAN_CODE_COMMAND 0xF0
#define GET_SCAN_CODE 0x00
#define SET_SCAN_CODE_1 0x01
#define SET_SCAN_CODE_2 0x02
#define SET_SCAN_CODE_3 0x03


#define MAX_QUERY_QUEUE_SIZE 256

typedef struct {
    uint8_t command;
    uint8_t data;
    bool has_data;
} kbd_query;

// circular queue
struct kbd_query_queue {
    kbd_query queries[MAX_QUERY_QUEUE_SIZE];
    size_t front;
    size_t rear;
    size_t size;
    size_t occupied;
};

void kbd_query_init();
void kbd_query_enqueue(kbd_query query);
void kbd_query_dequeue();
kbd_query kbd_query_peek();

typedef struct {
    uint8_t code;
} key_event;


static key_event keyboard_buffer[KEYBOARD_BUFFER_SIZE];
static size_t keyboard_buffer_index = 0;

void keyboard_handler(void);

uint8_t keyboard_recieve_data();

void keyboard_send_query(kbd_query query);

void keyboard_send_data(uint8_t data);

void keyboard_init();

#endif // KEYBOARD_H

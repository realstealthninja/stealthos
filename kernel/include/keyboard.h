#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>
#include <stddef.h>

#define KEYBOARD_BUFFER_SIZE 256

#define KEYBOARD_ENABLE           0xf4
#define KEYBOARD_NO_OPERAND       0xff
#define KEYBOARD_DATA_PORT        0x60
#define KEYBOARD_STATUS_REGISTER  0x64
#define KEYBOARD_COMMAND_REGISTER 0x64


#define OUT_BUFFER_STATUS 0x01  // 0b1
#define IN_BUFFER_STATUS  0x03  // 0b10

// ps2 port commands
#define PS2_DISABLE_PORT1   0xAD
#define PS2_DISABLE_PORT2   0xA7
#define PS2_SELF_TEST       0xAA
#define PS2_CONTROLLER_CONFIGURATION_BYTE 0x20
// ps2 configuration byte bit masks
#define PS2_PORT1_INTERRUPT 0b00000001
#define PS2_PORT2_INTERRUPT 0b00000010
#define PS2_SYSTEM_FLAG     0b00000100
#define PS2_SHOULD_BE_ZERO  0b00001000
#define PS2_PORT1_CLOCK     0b00010000
#define PS2_PORT2_CLOCK     0b00100000
#define PS2_PORT1_TRANS     0b01000000
#define PS2_PORT2_ZERO      0b10000000

typedef struct {
    uint8_t code;
} key_event;

static key_event keyboard_buffer[KEYBOARD_BUFFER_SIZE];
static size_t keyboard_buffer_index = 0;

void keyboard_handler(void);

void keyboard_init(void);

void kbd_send_cmd(uint8_t cmd);

#endif // KEYBOARD_H

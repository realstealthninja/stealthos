#include "io/keyboard.h"
#include "tty.h"
#include "utils.h"
#include "interrupts.h"


static bool sending = false;

void keyboard_handler(void) {
    if (terminal_initalised) {
        terminal_writestring("key hit\n");
    }
    key_event scancode = {
        .code =  inb(0x60)
    };

    keyboard_buffer[keyboard_buffer_index] = scancode; 
    keyboard_buffer_index = (keyboard_buffer_index + 1) % KEYBOARD_BUFFER_SIZE;
}

void keyboard_init(void) {
    idt_install_irq_handler(0x00, keyboard_handler);
}

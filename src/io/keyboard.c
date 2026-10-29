#include "io/keyboard.h"
#include <stdint.h>
#include "interrupts/apic.h"
#include "io/serial.h"
#include "stdlib.h"
#include "tty.h"
#include "io/ps2.h"

static struct kbd_query_queue queue;

void kbd_query_init() {
    queue.size = 256;
    queue.front = 0;
    queue.rear = 0;
    
}

void kbd_query_enqueue(kbd_query query) {
    queue.occupied++;

    if (queue.occupied > queue.size) {
        // overflow
    }
    queue.rear = ++queue.rear % queue.size;
    queue.queries[queue.rear] = query;
}

void kbd_query_dequeue() {
    queue.occupied--;

    queue.front = ++queue.front % queue.size;
}

kbd_query kbd_query_peek()  {
    return queue.queries[queue.front % queue.size];
}

static bool sending = false;

void keyboard_handler(void) {
    if (terminal_initalised) {
        terminal_writestring("key hit\n");
    }

    uint8_t response = ps2_get_data();

    if (response == PS2_DEVICE_ACKNOWLEDGE) {
        kbd_query_dequeue();
    }
    
}

void keyboard_init(void) {
    keyboard_send_data(0xF0);
    uint8_t response = keyboard_recieve_data();

    if(keyboard_recieve_data() == 0xFA) {
        keyboard_send_data(0x00);
        response = keyboard_recieve_data();

    
        char hello[20];
        
        binary_to_hexstr(response, hello);
        
        terminal_println(hello);
    }

    if (response == PS2_DEVICE_ACKNOWLEDGE) {
        response = keyboard_recieve_data();
    }

    
    char hello[20];

    binary_to_hexstr(response, hello);
    
    serial_write(hello, 4);
    terminal_println(hello);
    uint32_t kbd_intr = read_ioapic(IOAPICREDTBL(0));

    for(int i = 0; i < 32; i++){
        if (kbd_intr & (1 >> i)) {
                terminal_putchar('1');
        } else {
            terminal_putchar('0');
        }
    }
    
}

void keyboard_send_data(uint8_t data) {
    if (keyboard_port.port_number == 1) {
        ps2_port1_send_data(data);
    } else {
        ps2_port2_send_data(data);
    }
}


uint8_t keyboard_recieve_data() {
    return ps2_get_data();
}

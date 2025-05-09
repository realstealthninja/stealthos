#include "keyboard.h"
#include "tty.h"
#include "utils.h"
#include "interupts.h"


void kbd_send_cmd(uint8_t cmd) {
    int8_t status = inb(KEYBOARD_STATUS_REGISTER);

    
}

int8_t kbd_read_data() {

}

void kbd_wait() {
    
}

void ps2_controller_init(void) {
    // disable both ports
    outb(KEYBOARD_DATA_PORT, PS2_DISABLE_PORT1); io_wait();
    outb(KEYBOARD_DATA_PORT, PS2_DISABLE_PORT2); io_wait();
    
    // flush the output buffer
    (void) inb(KEYBOARD_DATA_PORT);
    
    // set the configuration byte
    outb(KEYBOARD_DATA_PORT, PS2_CONTROLLER_CONFIGURATION_BYTE); io_wait();
    int8_t config_byte = inb(KEYBOARD_DATA_PORT);

    // config byte
    // 0 0 0 0 0 0 0 0
    // │ │ │ │ │ │ │ └─ 0
    // │ │ │ │ │ │ └─── port 1 translation (1 enabled, 0 disabled)
    // │ │ │ │ │ └───── port 2 Clock(1 disabled, 0 enabled)
    // │ │ │ │ └─────── port 1 Clock(1 disabled, 0 enabled)
    // │ │ │ └───────── 0
    // │ │ └─────────── System flag(1 system passed, 0 what?)
    // │ └───────────── port 2 IRQ (1 enabled, 0 disabled)
    // └─────────────── port 1 IRQ (1 enabled, 0 disabled)
    
    // disable IRQs 
    config_byte &= (0xff ^ PS2_PORT1_INTERRUPT); // 0x0
    config_byte &= (0xff ^ PS2_PORT1_INTERRUPT);
    // disable translation from scancode 2 to 1
    // by clearing bit 6
    config_byte &= (0xff ^ PS2_PORT1_TRANS);
    // enable clocks
    config_byte |= PS2_PORT1_CLOCK;
    config_byte |= PS2_PORT2_CLOCK;
    // write the config back into the ps2 controller
    outb(KEYBOARD_DATA_PORT, config_byte);

    terminal_writestring("Testing PS2 Controller");
    // check whether the ps/2 controller works
    outb(KEYBOARD_DATA_PORT, PS2_SELF_TEST); io_wait();
    int8_t test_result = inb(KEYBOARD_DATA_PORT);
    
    if (test_result != 0x55) {
       terminal_writestring("Tests failed"); 
       return
    }




}


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

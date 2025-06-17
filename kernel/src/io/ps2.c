#include "io/ps2.h"
#include "io/serial.h"
#include "utils.h"

#include <stdint.h>

static bool has_timed_out = false;

/**
 * @brief Gets the status of the controller
 * 
 * @return uint8_t 
 */
uint8_t ps2_status() {
    return inb(PS2_CONTROLLER_STATUS_PORT);
}

/**
 * @brief waits until the input buffer clears
 * 
 */
void ps2_wait_for_input() {
    int counter = 0;
    while((ps2_status() & INPUT_BUFFER_STATUS) != 0) {
        asm("pause");
        counter++;

        if (counter > 1000) {
            has_timed_out = true;
            return;
        }
    }
    return;
}

/**
 * @brief waits for the output buffer to be clear
 * 
 */
void ps2_wait_for_ouput() {
    int counter = 0;
    while(((ps2_status() & OUTPUT_BUFFER_STATUS) != 1)) {
        asm("pause");
        counter++;
        if (counter > 100) {
            has_timed_out = true;
            return;
        }
    }
    return;
}

/**
 * @brief sends a command to ps2 controller
 * 
 * @param cmd the command to be sent
 */
void ps2_send_cmd(uint8_t cmd) {
    outb(PS2_CONTROLLER_COMMAND_REGISTER, cmd);
}

/**
 * @brief send data to the ps2 controller
 * 
 * @param data data to be sent
 */
void ps2_send_data(uint8_t data) {
    ps2_wait_for_input();
    if (has_timed_out) {
        serial_write_string("Warning: ps2 waiting for output timed out");
        return;
    }
    outb(PS2_CONTROLLER_DATA_PORT, data);
}

/**
 * @brief gets data from the ps2 controller
 * 
 * @return uint8_t the data
 */
uint8_t ps2_get_data() {
    if (has_timed_out) {
        has_timed_out = false;
    }
    ps2_wait_for_ouput();
    if (has_timed_out) {
        serial_write_string("Warning: ps2 waiting for output timed out");
        return 0x00;
    }
    
    return inb(PS2_CONTROLLER_DATA_PORT);
}

void identify_ps2_device(struct ps2_port port, uint8_t first_byte) {
    switch (first_byte) {
        case 0x00:
            port.device_model = PS2_MOUSE;
            port.device_type = MOUSE;
            break;
        case 0x03:
            port.device_model = MOUSE_WITH_SCROLL_WHEEL;
            port.device_type = MOUSE;
            break;
        case 0x04:
            port.device_model = MOUSE_WITH_5_BUTTONS;
            port.device_type = MOUSE;
            break;
        case 0xAB:
            uint8_t second_byte = ps2_get_data();
            switch (second_byte) {
                case 0x83:
                case 0xC1:
                    port.device_model = MF2_KEYBOARD;
                    port.device_type = KEYBOARD;
                    break;
                case 0x84:
                    port.device_model = SHORT_KEYBOARD;
                    port.device_type = KEYBOARD;
                    break;
                case 0x85:
                    port.device_model = NCD_N97_KEYBOARD;
                    port.device_type = KEYBOARD;
                    break;
                case 0x86:
                    port.device_model = KEYBOARD_122_KEYS;
                    port.device_type = KEYBOARD;
                    break;
                case 0x90:
                    port.device_model = JAPANESE_G_KEYBOARD;
                    port.device_type = KEYBOARD;
                    break;
                case 0x91:
                    port.device_model = JAPANESE_P_KEYBOARD;
                    port.device_type = KEYBOARD;
                    break;
                case 0x92:
                    port.device_model = JAPANESE_A_KEYBOARD;
                    port.device_type = KEYBOARD;
                    break;
            }
            break;
        case 0xAC:
            uint8_t bit = ps2_get_data();
            if (bit == 0xA1) {
                port.device_model = NCD_SUN_KEYBOARD;
                port.device_type = KEYBOARD;
            } else {

            }
            break;
        default:
            port.device_model = DEVICE_NOT_IDENTIFIED;
            port.device_type = OTHER;
    }
}
    

void ps2_controller_init() {
    //TODO: check if ps2 ports exist
    // disable both the ports
    ps2_send_cmd(DISABLE_FIRST_PORT);
    ps2_send_cmd(DISABLE_SECOND_PORT);

    port_1.enabled = false;
    port_1.port_number = 1;
    port_2.enabled = false;
    port_2.port_number = 2;
    
    // discard the junk data
    (void) ps2_get_data();
    serial_write_string("discarded ps2 data");

    // read the controller byte
    ps2_send_cmd(READ_CONFIGURATION_BYTE);
    uint8_t config = ps2_get_data();
    serial_write_string("got controller config byte");

    // disable irqs and translation for port one
    config &= ~PS2_PORT1_INTERRUPT;
    config &= ~PS2_PORT1_TRANSLATION;

    // enable clock for port one
    config &= ~PS2_PORT1_CLOCK;

    // send the config back

    ps2_send_cmd(WRITE_CONFIGURATION_BYTE);
    ps2_send_data(config);

    serial_write_string("configured ps2 controller");

    // perform controller test
    ps2_send_cmd(TEST_PS2_CONTROLLER);
    if (ps2_get_data() != 0x55) {
        serial_write_string("PS2 SELF TEST FAILED!\n\0");
        return;
    }
    serial_write_string("PS2 Self test succeeded\0");
    // some controllers may reset their config after self test
    ps2_send_cmd(WRITE_CONFIGURATION_BYTE);
    ps2_send_data(config);
    
    // check if it s a dual channel ps2 controller
    ps2_send_cmd(ENABLE_SECOND_PORT);
    ps2_send_cmd(READ_CONFIGURATION_BYTE);
    
    config = ps2_get_data();
    if ((config & PS2_PORT2_CLOCK) == 0) {
        is_dual_channel = true;
        
        ps2_send_cmd(DISABLE_SECOND_PORT);
        ps2_send_cmd(READ_CONFIGURATION_BYTE);
        
        config = ps2_get_data();
        // disable port 2 interrupt
        config &= ~PS2_PORT2_INTERRUPT;
        // enable clock
        config &= ~PS2_PORT2_CLOCK;

        ps2_send_cmd(WRITE_CONFIGURATION_BYTE);
        ps2_send_data(config);
    } else {
        is_dual_channel = false;
        // no dual channel
    }

    // test port one
    ps2_send_cmd(TEST_FIRST_PORT);
    if (ps2_get_data() != 0x00) {
        serial_write_string("PORT 1 FAILED SELF TEST");
        port_1.exists = false;
    } else {
        serial_write_string("PORT 1 PASSED SELF TEST");
        port_1.exists = true;
    }

    if(is_dual_channel) {
        // test port two
        ps2_send_cmd(TEST_SECOND_PORT);
        if(ps2_get_data() != 0x00) {
            serial_write_string("PORT 2 FAILED THE TEST");
            port_2.exists = false;
        } else {
            serial_write_string("PORT 2 PASSED SELF TEST");
            port_2.exists = true;
        }
    }

    if (!port_1.exists && !port_2.exists) {
        is_ps2_enabled = false;
        return;
    }

    if (port_2.exists) {
        ps2_send_cmd(DISABLE_FIRST_PORT);
        ps2_send_cmd(READ_CONFIGURATION_BYTE);

        // enable interrupts for port 2
        config = ps2_get_data();
        config &= PS2_PORT2_INTERRUPT;

        ps2_send_cmd(WRITE_CONFIGURATION_BYTE);
        ps2_send_data(config);

        // enable the port
        ps2_send_cmd(ENABLE_SECOND_PORT);
        port_2.enabled = true;
        serial_write_string("PS2 PORT TWO ENABLED");

        // reset device
        ps2_port2_send_data(PS2_DEVICE_RESET);

        uint8_t resp =  ps2_get_data();

        if (has_timed_out) {
            port_2.device_type = DEVICE_NOT_CONNECTED;
        } else {
            port_2.device_type = OTHER;
        }

        port_2.device_model = DEVICE_NOT_IDENTIFIED;

        if (resp == PS2_DEVICE_ACKNOWLEDGE || resp == 0xAA) {
            resp = ps2_get_data();
            if (resp == 0xFA || resp == PS2_DEVICE_ACKNOWLEDGE) {
                port_2.device_type = OTHER;
                port_2.device_model = DEVICE_NOT_IDENTIFIED;
            }
        } else if (resp == 0xFC) {
            port_2.device_type = DEVICE_NOT_CONNECTED;
            port_2.device_model = DEVICE_NOT_IDENTIFIED;
        }

        if (port_2.device_model == DEVICE_NOT_IDENTIFIED && port_2.device_type != DEVICE_NOT_CONNECTED) {
            // identify the device
            ps2_port2_send_data(PS2_DEVICE_DISABLE_SCANNING);
            uint8_t response = ps2_get_data();
            if (response != PS2_DEVICE_ACKNOWLEDGE) {
                if (response == PS2_DEVICE_RESEND) {
                    ps2_port2_send_data(PS2_DEVICE_DISABLE_SCANNING);
                    response = ps2_get_data();
                }
            }
            
            ps2_port2_send_data(PS2_DEVICE_IDENTIFY);
            response = ps2_get_data();
            if (response != PS2_DEVICE_ACKNOWLEDGE) {
                if (response == PS2_DEVICE_RESEND) {
                    ps2_port2_send_data(PS2_DEVICE_DISABLE_SCANNING);
                    response = ps2_get_data();
                }
            }
            uint8_t first_byte = ps2_get_data();
            if (!has_timed_out) {
                identify_ps2_device(port_2, first_byte);
            } else {
                port_2.device_model = AT_KEYBOARD;
                port_2.device_type = KEYBOARD;
                
            }

            ps2_port2_send_data(PS2_DEVICE_ENABLE_SCANNING);
            if (port_2.device_model != DEVICE_NOT_IDENTIFIED && port_2.device_type != OTHER) {
                serial_write_string("port two identified");
            }
        }
        
        ps2_send_cmd(ENABLE_FIRST_PORT);
    }

    if (port_1.exists) {
        ps2_send_cmd(READ_CONFIGURATION_BYTE);
        
        // enable interrupts for port 1
        config = ps2_get_data();
        config &= PS2_PORT1_INTERRUPT;

        ps2_send_cmd(WRITE_CONFIGURATION_BYTE);
        ps2_send_data(config);

        // enable the port
        ps2_send_cmd(ENABLE_FIRST_PORT);
        port_1.enabled = true;
        serial_write_string("PS2 PORT ONE ENABLED");
        

        // reset device
        ps2_port1_send_data(PS2_DEVICE_RESET);
        uint8_t byte =  ps2_get_data();

        if (has_timed_out) {
            port_1.device_type = DEVICE_NOT_CONNECTED;
        } else {
            port_1.device_type = OTHER;
        }

        port_1.device_model = DEVICE_NOT_IDENTIFIED;

        if (byte == PS2_DEVICE_ACKNOWLEDGE || byte == 0xAA) {
            byte = ps2_get_data();
            if (byte == 0xFA || byte == PS2_DEVICE_ACKNOWLEDGE) {
                port_1.device_type = OTHER;
                port_1.device_model = DEVICE_NOT_IDENTIFIED;
            }
        } else if (byte == 0xFC) {
            port_1.device_type = DEVICE_NOT_CONNECTED;
            port_1.device_model = DEVICE_NOT_IDENTIFIED;
        }

        if (port_1.device_model == DEVICE_NOT_IDENTIFIED && port_1.device_type != DEVICE_NOT_CONNECTED) {
            // identify the device
            ps2_port1_send_data(PS2_DEVICE_DISABLE_SCANNING);
            uint8_t response = ps2_get_data();
            if (response != PS2_DEVICE_ACKNOWLEDGE) {
                if (response == PS2_DEVICE_RESEND) {
                    ps2_port1_send_data(PS2_DEVICE_DISABLE_SCANNING);
                    response = ps2_get_data();
                }
            }
            
            ps2_port1_send_data(PS2_DEVICE_IDENTIFY);
            response = ps2_get_data();
            if (response != PS2_DEVICE_ACKNOWLEDGE) {
                if (response == PS2_DEVICE_RESEND) {
                    ps2_port1_send_data(PS2_DEVICE_DISABLE_SCANNING);
                    response = ps2_get_data();
                }
            }
            uint8_t first_byte = ps2_get_data();
            if (!has_timed_out) {
                identify_ps2_device(port_1, first_byte);
            } else {
                port_1.device_model = AT_KEYBOARD;
                port_1.device_type = KEYBOARD;
            }

            ps2_port1_send_data(PS2_DEVICE_ENABLE_SCANNING);

            if (port_1.device_model != DEVICE_NOT_IDENTIFIED) {
                serial_write_string("port 1 identified");
            }
        }

    }

    is_ps2_enabled = true;
}

void ps2_port1_send_data(uint8_t data) {
    ps2_send_data(data);
}

void ps2_port2_send_data(uint8_t data) {
    if (!port_2.exists) {
        return;
    } 
    ps2_send_cmd(WRITE_BYTE_TO_SECOND_PORT_INPUT_BUFFER);
    ps2_send_data(data);
}


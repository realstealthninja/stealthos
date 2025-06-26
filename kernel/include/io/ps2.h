#ifndef PS2_H
#define PS2_H

// PS2 CONTROLLER PORTS
#include <stdbool.h>
#include <stdint.h>
#define PS2_CONTROLLER_DATA_PORT   0x60
#define PS2_CONTROLLER_STATUS_PORT 0x64
#define PS2_CONTROLLER_COMMAND_REGISTER 0x64

// PS2 CONTROLLER COMMANDS MUST BE SENT ON THE 0x64 PORT

#define READ_FIRST_BYTE_FROM_INTERNAL_RAM 0x20
#define READ_CONFIGURATION_BYTE 0x20

#define WRITE_FIRST_BYTE_TO_INTERNAL_RAM  0x60
#define WRITE_CONFIGURATION_BYTE 0x60

#define DISABLE_SECOND_PORT 0xA7
#define ENABLE_SECOND_PORT  0xA8

#define TEST_SECOND_PORT    0xA9

#define TEST_PS2_CONTROLLER 0xAA

#define TEST_FIRST_PORT     0xAB

#define DISABLE_FIRST_PORT  0xAD
#define ENABLE_FIRST_PORT   0xAE

#define READ_CONTROLLER_INPUT_PORT  0xC0
#define READ_CONTROLLER_OUTPUT_PORT 0xD0

#define WRITE_BYTE_TO_CONTROLLER_PORT_OUTPUT 0xD1
#define WRITE_BYTE_TO_FIRST_PORT_OUTPUT_BUFFER    0xD2
#define WRITE_BYTE_TO_SECOND_PORT_OUTPUT_BUFFER   0xD3
#define WRITE_BYTE_TO_SECOND_PORT_INPUT_BUFFER    0xD4

// STATUS BIT MASKS
#define OUTPUT_BUFFER_STATUS (1 << 0)
#define INPUT_BUFFER_STATUS  (1 << 1)
#define SYSTEM_FLAG          (1 << 2)
#define COMMAND_OR_DATA      (1 << 3)
#define TIME_OUT_ERROR       (1 << 6)
#define PARITY_ERROR         (1 << 7)

// CONFIG BIT MASKS

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
    
#define PS2_PORT1_INTERRUPT    (1 << 0)
#define PS2_PORT2_INTERRUPT    (1 << 1)
#define SYSTEM_FLAG            (1 << 2)
#define SHOULD_BE_ZERO         (1 << 3)
#define PS2_PORT1_CLOCK        (1 << 4)
#define PS2_PORT2_CLOCK        (1 << 5)
#define PS2_PORT1_TRANSLATION  (1 << 6)
#define SHOULD_BE_ZERO_7       (1 << 7)

// device commands
#define PS2_DEVICE_IDENTIFY         0xF2
#define PS2_DEVICE_ENABLE_SCANNING  0xF4
#define PS2_DEVICE_DISABLE_SCANNING 0xF5
#define PS2_DEVICE_ACKNOWLEDGE      0xFA
#define PS2_DEVICE_RESET            0xFF
#define PS2_DEVICE_RESEND           0xFE



static bool is_ps2_enabled = false; ///< true if ps2 is enabled and initalized
static bool is_dual_channel = false; ///< true if ps2 port is a dual channel port


enum ps2_device_type {
    DEVICE_NOT_CONNECTED,
    KEYBOARD,
    MOUSE,
    OTHER
};

enum ps2_device_model {
    DEVICE_NOT_IDENTIFIED,
    AT_KEYBOARD,
    PS2_MOUSE,
    MOUSE_WITH_SCROLL_WHEEL,
    MOUSE_WITH_5_BUTTONS,
    MF2_KEYBOARD,
    SHORT_KEYBOARD,
    NCD_N97_KEYBOARD,
    KEYBOARD_122_KEYS,
    JAPANESE_G_KEYBOARD,
    JAPANESE_P_KEYBOARD,
    JAPANESE_A_KEYBOARD,
    NCD_SUN_KEYBOARD
};

/**
 * @brief describes a ps2 port and what is connected to it
 * 
 */
struct ps2_port {
    int port_number;
    enum ps2_device_model device_model;
    enum ps2_device_type device_type;
    bool enabled;
    bool exists;
};

static struct ps2_port port_1; ///< physical port 1
static struct ps2_port port_2; ///< physical port 2

static struct ps2_port keyboard_port; ///< logical keyboard port
static struct ps2_port mouse_port; ///< logical mouse port

/**
 * @brief initialises the ps2 controller
 */
void ps2_controller_init();

/**
 * @brief sends data to the first port
 * 
 * @param data the data to be sent
 */
void ps2_port1_send_data(uint8_t data);

/**
 * @brief sends data to the second port
 * 
 * @param data the data to be sent
 */
void ps2_port2_send_data(uint8_t data);


uint8_t ps2_get_data();

#endif // PS2_H
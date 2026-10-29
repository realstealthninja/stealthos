#include <io/serial.h>
#include <stdint.h>

#include "utils.h"

void serial_configure_baud_rate(uint16_t com, uint16_t divisor) {
    outb(SERIAL_LINE_COMMAND_PORT(com), SERIAL_LINE_ENABLE_DLAB);

    outb(SERIAL_DATA_PORT(com), (divisor >> 8) & 0xff);

    outb(SERIAL_DATA_PORT(com), divisor & 0xff);
}

void serial_configure_line(uint16_t com) {
    outb(SERIAL_LINE_COMMAND_PORT(com), 0x03);
}

void serial_configure_buffer(uint16_t com) {
    outb(SERIAL_LINE_COMMAND_PORT(com), 0xC7);
}

void serial_configure_modem(uint16_t com) {
    outb(SERIAL_LINE_COMMAND_PORT(com), 0x03);
}

bool serial_is_transmit_fifo_empty(uint16_t com) {
    return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x20;
}

void serial_init() {
    uint16_t com = SERIAL_COM1_PORT;
    serial_configure_baud_rate(com, 1);
    serial_configure_line(com);
    serial_configure_buffer(com);
    serial_configure_modem(com);
}

void serial_putc(char c) {
    uint16_t com = SERIAL_COM1_PORT;
    while (!serial_is_transmit_fifo_empty(com)) {
        io_wait();
    }

    outb(com, c);
}

void serial_write(char* data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        serial_putc(data[i]);
    }
}

void serial_write_string(char* data) {
    while (*data != '\0') {
        serial_putc(*data++);
    }
    serial_putc('\n');
}

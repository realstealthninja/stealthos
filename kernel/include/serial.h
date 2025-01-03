#ifndef SERIAL_H
#define SERIAL_H

#include "tty.h"
#include <stdbool.h>
#include <stdint.h>

#define SERIAL_COM1_PORT                0x3F8

#define SERIAL_DATA_PORT(base)          (base)
#define SERIAL_FIFO_COMMAND_PORT(base)  (base + 2)
#define SERIAL_LINE_COMMAND_PORT(base)  (base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
#define SERIAL_LINE_STATUS_PORT(base)   (base + 5)


#define SERIAL_LINE_ENABLE_DLAB         0x80


void serial_configure_baud_rate(uint16_t com, uint16_t divisor);

void serial_configure_line(uint16_t com);

void serial_configure_buffer(uint16_t com);

void serial_configure_modem(uint16_t com);

bool serial_is_transmit_fifo_empty(uint16_t com);

void serial_init(void);

void serial_putc(char c);

void serial_write(char* data, size_t len);

void serial_write_string(char* data);

#endif // SERIAL_H

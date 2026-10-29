/**
 * @file
 * @brief serial.h
 */

#ifndef SERIAL_H
#define SERIAL_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#define SERIAL_COM1_PORT                0x3F8

#define SERIAL_DATA_PORT(base)          (base)
#define SERIAL_FIFO_COMMAND_PORT(base)  (base + 2)
#define SERIAL_LINE_COMMAND_PORT(base)  (base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
#define SERIAL_LINE_STATUS_PORT(base)   (base + 5)


#define SERIAL_LINE_ENABLE_DLAB         0x80


/**
 * @brief configures baud rate of the serial
 *
 * @param com the port to be used
 * @param divisor 11,500 is the default baud rate
 */
void serial_configure_baud_rate(uint16_t com, uint16_t divisor);


/**
 * @brief configures the line
 *
 * @param com the port to be configured
 */
void serial_configure_line(uint16_t com);


/**
 * @brief configures the buffer
 *
 * @param com the port to be configured
 */
void serial_configure_buffer(uint16_t com);


/**
 * @brief configures the modem
 *
 * @param com the port to be configured
 */
void serial_configure_modem(uint16_t com);

/**
 * @brief checks if everything has transimtted
 *
 * @param com the port toe be checked
 * @return true if transmit is empty 
 * @return false if transmit is not empty
 */
bool serial_is_transmit_fifo_empty(uint16_t com);

/**
 * @brief initialized the serial port 0x3F8
 */
void serial_init(void);


/**
 * @brief puts a character to serial
 *
 * @param c character to be sent
 */
void serial_putc(char c);

/**
 * @brief sends the given data of specified length to serial
 *
 * @param data the data to be sent
 * @param len the length of the data
 */
void serial_write(char* data, size_t len);


/**
 * @brief sends a string terminated with \0
 *
 * @param data the string to be sent
 */
void serial_write_string(char* data);

#endif // SERIAL_H

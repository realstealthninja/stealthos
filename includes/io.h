#ifndef STEALTH_IO_H
#define STEALTH_IO_H

/* The I/O ports */
#define COMMAND_PORT         0x3D4
#define DATA_PORT            0x3D5

/* The I/O port commands */
#define HIGH_BYTE_COMMAND    14
#define LOW_BYTE_COMMAND     15

/* Colors of text */
#define BLACK 0x00
#define BLUE 0x01
#define WHITE 0xff



/**
 * @brief sends data to a specified port
 * 
 * @param port the port to send the data to 
 * @param data the data to be sent to 
 */
void outb(unsigned short port, unsigned char data);


/**
 * @brief Writes to the frame buffer characters
 * 
 * @param index of the cell to be written to
 * @param character to be written onto the cell
 * @param foreground color of the cell
 * @param background color of the cell
 * @note index should be updated by 2
 */
void frame_buffer_write_cell(
    unsigned int index,
    char character,
    unsigned char foreground,
    unsigned char background
);

/**
 * @brief moves the cursor of the frame buffer to the given positon
 * 
 * @param pos the new position of the cursor
 */
void fb_move_cursor(unsigned int pos);

/**
 * @brief Writes a given string to the display
 * 
 * @param string the string to be displayed
 * @param len length of the string
 */
void write(char *string, unsigned int len);




#endif // STEALTH_IO_H
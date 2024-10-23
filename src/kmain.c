#define BLACK 0x00
#define BLUE 0x01
#define WHITE 0xff


char* frame_buffer = (char*) 0x000B8000; // memory mapped address for the frame buffer;

/**
 * @brief Writes to the frame buffer characters
 * 
 * @param index of the cell to be written to
 * @param character to be written onto the cell
 * @param foreground color of the cell
 * @param background color of the cell
 */
void frame_buffer_write_cell(
    unsigned int index,
    char character,
    unsigned char foreground,
    unsigned char background) {
    frame_buffer[index] = character;
    frame_buffer[index + 1] = ((foreground & 0x0f) << 4) | (background & 0x0f);
}


int sum_of_three(int a, int b, int c) {
    frame_buffer_write_cell(10, 'H', BLUE, BLACK);

    return a+b+c;
}
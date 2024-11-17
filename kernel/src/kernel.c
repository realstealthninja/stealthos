#include <tty.h>


/**
 * @brief Entry point of the os.
 * 
 * @return int 
 */
int kmain() {
    terminal_init();

    terminal_write("Hello, world!", 13);
    return 0;
}
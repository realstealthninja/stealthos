#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

static inline void outb(
    uint16_t port, uint8_t val
    ) {
    asm volatile(
        "outb %b0, %w1"
        : 
        : "a"(val), "Nd"(port) : "memory"
    );
}

static inline uint8_t inb(
    uint16_t port 
    ) {
    uint8_t ret_val;

    asm volatile(
        "inb %w1, %b0"
        : "=a" (ret_val)
        : "Nd" (port)
        : "memory"
    );

    return ret_val;
}

static inline void io_wait(void) {
    outb(0x80, 0);
}

#endif // UTILS_H

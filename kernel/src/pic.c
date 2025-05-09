#include "pic.h"
#include "utils.h"

void PIC_sendEOI(uint8_t irq) {
    if(irq >= 8) {
        outb(PIC2_COMMAND, PIC_EOI);
    } else {
        outb(PIC1_COMMAND, PIC_EOI);
    }
}

void PIC_remap(int offset1, int offset2) {
    outb(PIC1_COMMAND,ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);

    outb(PIC1_COMMAND, offset1);
    io_wait();
    outb(PIC2_COMMAND, offset2);
    io_wait();

    outb(PIC1_DATA, 4);
    io_wait();
    outb(PIC2_DATA, 2);

    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC1_DATA, ICW4_8086);
    io_wait();

    outb(PIC1_DATA, 0);
    outb(PIC1_DATA, 0);
}

void PIC_init() {
    PIC_remap(0x20, 0x28);
}

void PIC_disable(void) {
    outb(PIC1_DATA, 0xff);
    outb(PIC2_DATA, 0xff);
}


void IRQ_set_mask(uint8_t IRQline) {
    uint16_t port;
    uint8_t value;

    if(IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = inb(port) | (1 << IRQline);
    outb(port, value);        
}

void IRQ_clear_mask(uint8_t IRQline) {
    uint16_t port;
    uint8_t value;

    if(IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = inb(port) & ~(1 << IRQline);
    outb(port, value);        
}

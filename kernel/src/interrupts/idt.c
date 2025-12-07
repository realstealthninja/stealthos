#include <stdint.h>
#include <stdbool.h>
#include "io/serial.h"

#include "interrupts/idt.h" 

__attribute__((aligned(0x10)))
struct interrupt_descriptor_entry_t IDT[IDT_MAX_DESCRIPTORS];

struct interrupt_descriptor_table_register_t idtr;

bool vectors[IDT_MAX_DESCRIPTORS];

extern void* isr_stub_table[];
uint64_t irq_handlers[IDT_MAX_DESCRIPTORS];


void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags, uint8_t ist) {
    struct interrupt_descriptor_entry_t* descriptor = &IDT[vector];

    descriptor->isr_low             = (uint64_t) isr & 0xffff;
    descriptor->segment_selector    = 0x28; 
    descriptor->IST                 = ist;
    descriptor->type_attributes     = flags;
    descriptor->isr_mid             = ((uint64_t) isr >> 16) & 0xffff;
    descriptor->isr_high            = ((uint64_t) isr >> 32) & 0xffffffff;
    descriptor->reserved            = 0;
}


void idt_install_irq_handler(int8_t vector, void *handler) {
    irq_handlers[vector] = (uint64_t)handler;
    idt_set_descriptor(vector, isr_stub_table[vector], IDT_DESCRIPTOR_EXTERNAL, 0); 
}

/**
 * @brief Initalizes the Interrupt descriptor table
 * @returns void
 */
void idt_init() {
    asm volatile ("cli");
    idtr.base   = (uint64_t)IDT;
    idtr.limit  = 0xfff;

    for (uint8_t vector = 0; vector < 32; vector++) {
        idt_set_descriptor(
                vector,
                isr_stub_table[vector],
                IDT_DESCRIPTOR_EXCEPTION, 0
        );
        vectors[vector] = true;
    }

    asm volatile ("lidt %0" : : "m"(idtr));
    asm volatile ("sti");
}

struct cpu_status_t* interrupt_dispatcher(struct cpu_status_t* context) {
    switch (context->vector) {
        case 0: 
            serial_write_string("Divide by zero\n\0");
            break;
        case 8:
            serial_write_string("Double fault... panic\n\0");
            break;
        case 13:
            serial_write_string("General protection fault.... panic\n\0");
            break;
        case 14:
            serial_write_string("Page fault... panic\n\0");
            break;
        default:
            serial_write_string("Unexpected interrupt...\n\0");
            break;
    }
    return context;
}



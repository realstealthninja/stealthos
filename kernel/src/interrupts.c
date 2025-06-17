#include <stdint.h>
#include "interrupts.h" 
#include "limine.h"
#include "stdbool.h"



__attribute__((aligned(0x10)))
static struct interrupt_descriptor_entry_t IDT[256];


__attribute__((used, section(".limine_requests")))
static volatile struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0
};


static struct interrupt_descriptor_table_register_t idtr;

static bool vectors[IDT_MAX_DESCRIPTORS];

uint64_t irq_handlers[IDT_MAX_DESCRIPTORS];


void exception_handler() {
    asm volatile ("cli");
    asm volatile ("hlt"); 
}


void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags, uint8_t ist) {
    struct interrupt_descriptor_entry_t* descriptor = &IDT[vector];
    struct limine_hhdm_response *hhdm = hhdm_request.response;

    descriptor->isr_low             = (uint64_t) isr & 0xffff;
    descriptor->segment_selector    = hhdm->offset; 
    descriptor->IST                 = ist;
    descriptor->type_attributes     = flags;
    descriptor->isr_mid             = ((uint64_t) isr >> 16) & 0xffff;
    descriptor->isr_high            = ((uint64_t) isr >> 32) & 0xfffffff;
    descriptor->reserved            = 0;
}


void idt_install_irq_handler(int8_t vector, void *handler) {
    irq_handlers[vector] = (uint64_t)handler;
    idt_set_descriptor(vector, isr_stub_table[vector], IDT_DESCRIPTOR_EXTERNAL, 2); 
}

/**
 * @brief Initalizes the Interrupt descriptor table
 * @returns void
 */
void idt_init() {
    idtr.base   = (uintptr_t)& IDT[0];
    idtr.limit  = (uint16_t) sizeof(
            struct interrupt_descriptor_entry_t
            ) * IDT_MAX_DESCRIPTORS -1;

    for (uint8_t vector = 0; vector < 32; vector++) {
        idt_set_descriptor(vector, isr_stub_table[vector], IDT_DESCRIPTOR_EXCEPTION, 1);
        vectors[vector] = true;
    }

    asm volatile ("lidt %0" : : "m"(idtr));
    asm volatile ("sti");
}


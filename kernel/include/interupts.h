#ifndef INTERUPTS_H
#define INTERUPTS_H

#include <stdint.h>

#define IDT_MAX_DESCRIPTORS 			256
#define IDT_CPU_EXCEPTIONS_COUNT        32

#define IDT_DESCRIPTOR_X16_INTERRUPT	0x06
#define IDT_DESCRIPTOR_X16_TRAP 		0x07
#define IDT_DESCRIPTOR_X32_TASK 		0x05
#define IDT_DESCRIPTOR_X32_INTERRUPT  	0x0E
#define IDT_DESCRIPTOR_X32_TRAP			0x0F
#define IDT_DESCRIPTOR_RING1  			0x40
#define IDT_DESCRIPTOR_RING2  			0x20
#define IDT_DESCRIPTOR_RING3  			0x60
#define IDT_DESCRIPTOR_PRESENT			0x80

#define IDT_DESCRIPTOR_EXCEPTION  (IDT_DESCRIPTOR_X32_INTERRUPT | IDT_DESCRIPTOR_PRESENT)
#define IDT_DESCRIPTOR_EXTERNAL   (IDT_DESCRIPTOR_X32_INTERRUPT | IDT_DESCRIPTOR_PRESENT)
#define IDT_DESCRIPTOR_CALL       (IDT_DESCRIPTOR_X32_INTERRUPT | IDT_DESCRIPTOR_PRESENT \
                                  | IDT_DESCRIPTOR_RING3)

struct interrupt_descriptor_entry_t {
    uint16_t isr_low;           // the lowest 16 bits of the Interrupt Service Routine
    uint16_t segment_selector;
    uint8_t  IST;
    uint8_t  type_attributes;
    uint16_t isr_mid;
    uint32_t isr_high;
    uint32_t reserved;
} __attribute__((packed));


struct interrupt_descriptor_table_register_t {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));


__attribute__((noreturn))
void exception_handler(void);

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags, uint8_t ist);
void idt_install_irq_handler(int8_t vector, void* handler);

extern void* isr_stub_table[];

void idt_init(void);

#endif // INTERUPTS_H

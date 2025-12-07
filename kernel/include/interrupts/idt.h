#ifndef IDT_H
#define IDT_H

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

struct cpu_status_t {
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rbp;
    uint64_t rdi;
    uint64_t rsi;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;

    uint64_t vector;
    uint64_t error_code;

    uint64_t iret_rip;
    uint64_t iret_cs;
    uint64_t iret_flags;
    uint64_t iret_rsp;
    uint64_t iret_ss;
};


struct cpu_status_t* interrupt_dispatcher(struct cpu_status_t* context);


void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags, uint8_t ist);
void idt_install_irq_handler(int8_t vector, void* handler);

void idt_init(void);



#endif // IDT_H

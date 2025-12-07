#include "interrupts/idt.h"
#include "interrupts/apic.h"
#include "interrupts/interrupts.h"
#include "interrupts/pic.h"


void setup_interrupts() {
    /**
     *  Intialises and populates the IDT
     *  Initalises and rempas the pic
     *  Disables the pic
     *  initalises and enables the apic
     */
    idt_init();
    PIC_init();
    PIC_disable();
    apic_init();
}

#include "memory/gdt.h"
#include <stdint.h>

uint64_t create_gdt_descriptor(uint32_t base, uint32_t limit, uint16_t flag) {
    uint64_t descriptor;
    descriptor =  limit & 0xF0000;
    descriptor |= (flag << 8) & 0xF0FF00;
    descriptor |= (base >> 16) & 0xFF;
    descriptor |= base & 0xFF000000;


    descriptor <<= 32;

    descriptor |= base << 16;
    descriptor |= limit & 0xffff;

    return descriptor;
}

void gdt_init() {
    gdt_entries[0] = create_gdt_descriptor(0, 0, 0);                   // NULL DESCRIPTOR
                                                     
    gdt_entries[1] = create_gdt_descriptor(0, 0xfffff, GDT_CODE_PL0);  // KERNEL CODE
    gdt_entries[2] = create_gdt_descriptor(0, 0xfffff, GDT_DATA_PL0);  // KERENEL DATA

    gdt_entries[3] = create_gdt_descriptor(0, 0xfffff, GDT_CODE_PL3);  // USER CODE
    gdt_entries[4] = create_gdt_descriptor(0, 0xfffff, GDT_DATA_PL3);  // USER DATA

    // gdt_entries[5] reserved for TSS
    // gdt_entries[6] reserved for TSS


    gdtr.limit = GDT_LENGTH * sizeof(uint64_t) - 1;
    gdtr.address = (uint64_t) gdt_entries;

    asm("lgdt %0" : : "m"(gdtr)); // load gdt
    reload_seg();
}

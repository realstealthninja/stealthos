#ifndef APIC_H
#define APIC_H

#include <stdbool.h>

#define IA32_APIC_BASE_MSR 0x1B
#define IA32_APIC_BASE_MSR_BSP 0x100 // Processor is a BSP
#define IA32_APIC_BASE_MSR_ENABLE 0x800

bool has_apic(void);

void enable_apic();
#endif // APIC_H

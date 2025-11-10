#include "apic.h"

#include <stdbool.h>
#include <cpuid.h>
#include <stdint.h>
#include "utils.h"

#define CPUID_FEAT_EDX_ACPI 1 << 22

bool has_apic() {
    uint32_t eax, unused, edx;        
    __get_cpuid(1, &eax, &unused, &unused, &edx);
    return edx & CPUID_FEAT_EDX_ACPI;
}


void cpu_set_apic_address(uintptr_t apic) {
    uint32_t edx = 0;
    uint32_t eax = (apic & 0xfffff0000) | IA32_APIC_BASE_MSR_ENABLE;
    cpu_set_msr(IA32_APIC_BASE_MSR, eax, edx);
    
}

uintptr_t cpu_get_apic_address() {
    uint32_t eax, edx;
    cpu_get_msr(IA32_APIC_BASE_MSR, &eax, &edx);

    return (eax & 0xffffff000);
}


void enable_apic() {
    cpu_set_apic_address(cpu_get_apic_address());
    outb(0xF0, inb(0xF0) | 0x100);
}


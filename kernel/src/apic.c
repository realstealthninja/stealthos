#include "apic.h"

#include <stdbool.h>
#include <cpuid.h>
#include <stdint.h>
#include "hhdm.h"
#include "power/acpi.h"
#include "power/madt.h"
#include "utils.h"

#define CPUID_FEAT_EDX_ACPI 1 << 22

static void* ioapic_adddress;

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

/**
 * @brief finds the base address of the ioapic
 */
void find_ioapic() {
    struct MADT madt; 
    void* madt_addr = find_SDT("APIC", 4);
    parse_MADT(madt_addr, &madt);
    ioapic_adddress = (void*)to_virtual_addr(madt.io_apic->io_apic_address);
}


void enable_apic() {
    cpu_set_apic_address(cpu_get_apic_address());
    write_register(0xF0, read_register(0xF0) | 0x100);
}

void init_apic() {
    enable_apic();
    find_ioapic();
}

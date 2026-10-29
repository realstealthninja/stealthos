#include "interrupts/apic.h"

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

uint32_t read_ioapic(uint32_t reg) {
    uint32_t volatile *ioapic = (uint32_t volatile*) ioapic_adddress;
    ioapic[0] = (reg & 0xff);
    return ioapic[4];
}

void write_ioapic(uint32_t reg, uint32_t value) {
    uint32_t volatile *ioapic = (uint32_t volatile *) ioapic_adddress;
    ioapic[0] = (reg & 0xff);
    ioapic[4] = value;
}


void enable_apic() {
    cpu_set_apic_address(cpu_get_apic_address());
    write_register(cpu_get_apic_address() + APIC_TPR, 0x00);
    write_register(cpu_get_apic_address() + APIC_SVR, read_register(cpu_get_apic_address() + APIC_SVR));
}


void apic_init() {
    enable_apic();
    find_ioapic();
}

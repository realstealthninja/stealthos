#ifndef APIC_H
#define APIC_H

#include <stdbool.h>

#define IA32_APIC_BASE_MSR 0x1B
#define IA32_APIC_BASE_MSR_BSP 0x100 // Processor is a BSP
#define IA32_APIC_BASE_MSR_ENABLE 0x800


#define APIC_SVR 0xf0
#define APIC_TPR 0x80

/**
 * @brief Checks whether APIC exists
 */
bool has_apic(void);

/**
 * @brief Enables APIC
 */
void enable_apic();

/**
 * @brief Initalises and enables APIC
 */
void apic_init();

#endif // APIC_H

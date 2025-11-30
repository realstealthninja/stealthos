#ifndef APIC_H
#define APIC_H

#include <stdbool.h>

#define IA32_APIC_BASE_MSR 0x1B
#define IA32_APIC_BASE_MSR_BSP 0x100 // Processor is a BSP
#define IA32_APIC_BASE_MSR_ENABLE 0x800

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
void init_apic();

#endif // APIC_H

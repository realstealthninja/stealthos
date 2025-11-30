#ifndef MADT_H
#define MADT_H

#include <stdint.h>
#include "tables.h"

/**
 * @struct MADT_Entry_Header
 * @brief Represents the common header of all MADT entries (interrupt devices)
 */
struct MADT_Entry_Header {
    uint8_t type;
    uint8_t length;
} __attribute__((packed));


/**
 * @struct Entry_0
 * @brief Represents the Processor Local APIC entry
 */
struct Entry_0 {
    struct MADT_Entry_Header header;
    uint8_t proc_id;
    uint8_t apic_id;
    uint32_t flags;
} __attribute__((packed));

/**
 * @struct Entry_1
 * @brief Represents the I/O APIC entry
 */
struct Entry_1 {
    struct MADT_Entry_Header header;
    uint8_t io_apic_id;
    uint8_t reserved;
    uint32_t io_apic_address;
    uint32_t global_sys_interrupt_base;
} __attribute__((packed));

/**
 * @struct Entry_2
 * @brief Represents the I/O APIC Interrupt Source entry
 */
struct Entry_2 {
    struct MADT_Entry_Header header;
    uint8_t bus_source;
    uint8_t irq_source;
    uint32_t global_sys_interrupt;
    uint16_t flags;
} __attribute__((packed));

//TODO: Add the rest of the entries

/**
 * @brief Represents the Multiple APIC Descriptor Table
 * @struct MADT
 */
struct MADT {
    struct ACPISDTHeader_t header;
    uint32_t local_apic_address;
    uint32_t flags;
    struct Entry_0* local_apic;
    struct Entry_1* io_apic;
    struct Entry_2* io_apic_source_override;
};


/**
 * @brief Parses a MADT.
 * @param[in] address The address of the MADT table
 * @param[out] madt The madt table to be filled
 */
void parse_MADT(void* address, struct MADT* madt);

#endif // MADT_H

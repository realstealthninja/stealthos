#ifndef ACPI_H
#define ACPI_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "tables.h"

/**
 * @struct RSDP_t
 * @brief Root system descriptor pointer
 */
struct RSDP_t {
    char signature[8];
    uint8_t checksum;
    char OEMID[6];
    uint8_t revision;
    uint32_t rsdt_address;
} __attribute__((packed));

/**
 * @struct XSDP_t
 * @brief extended system descriptor pointer
 */
struct XSDP_t {
    char signature[8];
    uint8_t checksum;
    char OEMID[6];
    uint8_t revision;
    uint32_t rsdt_address; // depricated

    uint32_t length;
    uint64_t xsdt_address;
    uint8_t extended_checksum;
    uint8_t reserved[3];
} __attribute__((packed));

/**
 * @struct XSDT_t
 * @brief Extended system descriptor table
 */
struct XSDT_t {
    struct ACPISDTHeader_t header;
    uint64_t SDTs[];
} __attribute__((packed));


/**
 * @struct RSDT_t
 * @brief Root System Descriptor Table
 */
struct RSDT_t {
    struct ACPISDTHeader_t header;
    uint32_t SDTs[];
} __attribute__((packed));


/**
 * @struct SDT
 * @brief Represents the System Descriptor table
 */
struct SDT {
    union {
        struct XSDT_t *xsdt;
        struct RSDT_t *rsdt;
    };
    size_t table_count;
    bool is_system_descriptor_extended;
};

static struct SDT sdt;

/**
 * @brief Finds rdsp or xdsp and parses the rdst or xdst respectively
 */
void acpi_init();

/**
 * @brief finds The address of a specified system descriptor table
 * @param signature The signature of the specified SDT
 * @param length The length of the signature string
 */
void* find_SDT(const char* signature, size_t length);

#endif // ACPI_H

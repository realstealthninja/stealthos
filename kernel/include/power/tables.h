#ifndef ACPI_TABLES
#define ACPI_TABLES

#include <stdint.h>

/**
 * @struct ACPISDTHeader_t
 * @brief Represents the common header of all acpi SDTs
 */
struct ACPISDTHeader_t {
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char OEMID[6];
    char OEMTableID[8];
    uint32_t OEMRevision;
    uint32_t creatorID;
    uint32_t creatorRevision;
} __attribute__((packed));


#endif //ACPI_TABLES

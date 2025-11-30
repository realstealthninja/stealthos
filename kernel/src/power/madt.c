#include "power/madt.h"
#include "hhdm.h"
#include "power/tables.h"


void parse_MADT(void *address, struct MADT *madt) {
    char* addr = (char*) address;

    madt->header = *((struct ACPISDTHeader_t*) addr);
    addr += sizeof(struct ACPISDTHeader_t);
    madt->local_apic_address = to_virtual_addr(*((uint32_t*) addr));
    addr += sizeof(uint32_t);
    madt->flags = *((uint32_t*) addr);
    addr += sizeof(uint32_t);

    while(addr < ((char*) address + madt->header.length)) {
        struct MADT_Entry_Header* header = (struct MADT_Entry_Header*) addr;
        switch (header->type) {
            case 0:
                madt->local_apic = (struct Entry_0*) addr;
                addr += sizeof(struct Entry_0);
                break;

            case 1:
                madt->io_apic = (struct Entry_1*) addr;
                addr += sizeof(struct Entry_1);
                break;

            case 2:
                madt->io_apic_source_override = (struct Entry_2*) addr;
                addr += sizeof(struct Entry_2);
                break;

            default:
                addr += header->length;
        }
    }
};

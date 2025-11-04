#include <limine.h>
#include <stddef.h>
#include <string.h>

#include "io/serial.h"
#include "power/acpi.h"
// ask for the modules 
__attribute__((used, section(".limine_requests")))
static volatile struct limine_rsdp_request rdsp_request = {
    .id = LIMINE_RSDP_REQUEST,
    .revision = 0,
};


struct RSDP_t* rsdp;

void acpi_init() {
    struct limine_rsdp_response* resp = rdsp_request.response;
    if (resp == NULL) {
        serial_write_string("RDSP NOT FOUND\0");
    }

    rsdp = (struct RSDP_t*) resp->address;


    if (rsdp->revision < 2) {
        serial_write_string("BAD xsdp\0");
        sdt.rsdt = (struct RSDT_t*) (uint64_t) rsdp->rsdt_address; 
        table_count = (sdt.rsdt->header.length - sizeof(struct ACPISDTHeader_t)) / 8;
        is_system_descriptor_extended = false;
    } else {
        struct XSDP_t* xsdp = (struct XSDP_t*) resp->address;
        sdt.xsdt = (struct XSDT_t*) xsdp->xsdt_address;
        table_count = (sdt.rsdt->header.length - sizeof(struct ACPISDTHeader_t)) / 8;
        is_system_descriptor_extended = true;
    }
}


void* find_SDT(const char* signature, size_t length) {
    if(is_system_descriptor_extended) {
        struct XSDT_t* root = sdt.xsdt;
        for(size_t i = 0; i < table_count; i++) {
            struct ACPISDTHeader_t* header = (struct ACPISDTHeader_t*) root->SDTs[i];
            if (cstrncmp(header->signature, signature, length) == 0) {
                return (void*) header;
            }
        }

    } else {
        struct RSDT_t* root = sdt.rsdt;
        for(size_t i = 0; i < table_count; i++) {
            struct ACPISDTHeader_t* header = (struct ACPISDTHeader_t*) (uint64_t)root->SDTs[i]; 
            if (cstrncmp(header->signature, signature, length) == 0) {
                return (void*) header;
            }
        } 
    }

    return NULL;
}

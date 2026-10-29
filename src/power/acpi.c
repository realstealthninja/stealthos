#include <limine.h>
#include <stddef.h>
#include <string.h>

#include "hhdm.h"
#include "io/serial.h"
#include "stdlib.h"
#include "power/acpi.h"

// ask for the modules 
__attribute__((used, section(".limine_requests")))
static volatile struct limine_rsdp_request rdsp_request = {
    .id = LIMINE_RSDP_REQUEST,
    .revision = 0,
};

void acpi_init() {
    struct limine_rsdp_response* resp = rdsp_request.response;
    struct RSDP_t* rsdp;
    struct XSDP_t* xsdp;

    if (resp == NULL) {
        serial_write_string("RDSP NOT FOUND\0");
    }

    rsdp = (struct RSDP_t*) resp->address;

    if (rsdp->revision < 2) {
        sdt.rsdt = (struct RSDT_t*) to_virtual_addr(rsdp->rsdt_address); 
        sdt.table_count = (sdt.rsdt->header.length - sizeof(struct ACPISDTHeader_t)) / 8;
        sdt.is_system_descriptor_extended = false;
    } else {
        xsdp = (struct XSDP_t*) resp->address;
        sdt.xsdt = (struct XSDT_t*) to_virtual_addr(xsdp->xsdt_address);
        sdt.table_count = (sdt.xsdt->header.length - sizeof(struct ACPISDTHeader_t)) / 8;
        sdt.is_system_descriptor_extended = true;
    }
}

struct SDT* get_sdt() {
    return &sdt;
}


void* find_SDT(const char* signature, size_t length) {
    struct ACPISDTHeader_t* header;  

    for(size_t i = 0; i < sdt.table_count; i++) {
        if (sdt.is_system_descriptor_extended) {
            header = (struct ACPISDTHeader_t*) to_virtual_addr(sdt.xsdt->SDTs[i]);
        } else {
            header = (struct ACPISDTHeader_t*) to_virtual_addr(sdt.rsdt->SDTs[i]);
        }

        if (cstrncmp(header->signature, signature, length) == 0) {
            return (void*) header;
        }
    }

    return NULL;
}

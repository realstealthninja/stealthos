#include <limine.h>
#include <stddef.h>
#include "io/serial.h"
#include "power/apci.h"
// ask for the modules 
__attribute__((used, section(".limine_requests")))
static volatile struct limine_rsdp_request rdsp_request = {
    .id = LIMINE_RSDP_REQUEST,
    .revision = 0,
};

struct XSDP_t* xsdp;

void acpi_init() {
    struct limine_rsdp_response* resp = rdsp_request.response;
    if (resp == NULL) {
        serial_write_string("RDSP NOT FOUND\0");
    }

    xsdp = (struct XSDP_t*) resp->address;


    if (xsdp->revision < 2) {
        serial_write_string("BAD xsdp\0");
    }

    xsdt = (struct XSDT_t*) xsdp->xsdt_address;
    xsdt_table_count = (xsdt->header.length - sizeof(struct ACPISDTHeader_t)) / 8;
}

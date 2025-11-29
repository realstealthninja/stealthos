#include "hhdm.h"
#include <stdint.h>
#include "limine.h"

__attribute__((used, section(".limine_requests")))
static volatile struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0,
};



uint64_t hhdm_get_offset() {
    if (hhdm_offset == 0) {
        hhdm_offset = hhdm_request.response->offset;
    }
    return hhdm_offset;
}

uint64_t to_virtual_addr(uint64_t address) {
    return address + hhdm_get_offset();
}

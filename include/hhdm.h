#ifndef HHDM
#define HHDM

#include <stdint.h>

static uint64_t hhdm_offset = 0;

uint64_t hhdm_get_offset();
uint64_t to_virtual_addr(uint64_t address);
#endif //HHDM

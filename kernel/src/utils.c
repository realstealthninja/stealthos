#include "utils.h"


void cpu_set_msr(uint32_t msr, uint32_t low, uint32_t high) {
    asm volatile("wrmsr": : "a"(low), "d"(high), "c"(msr));
}

void cpu_get_msr(uint32_t msr, uint32_t* low, uint32_t* high) {
    asm volatile("rdmsr":  "=a"(*low), "=d"(*high): "c" (msr));
}


uint64_t read_register(uint32_t reg) {
    uint32_t low;
    uint32_t high;
    cpu_get_msr(reg, &low, &high);

    return ((uint64_t)high << 32) | low;
}

void write_register(uint32_t reg, uint64_t value) {
    cpu_set_msr(reg, value & 0xffffffff00000000,value >> 32);
}
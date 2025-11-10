#include "utils.h"


void cpu_set_msr(uint32_t msr, uint32_t low, uint32_t high) {
    asm volatile("wrmsr": : "a"(low), "d"(high), "c"(msr));
}

void cpu_get_msr(uint32_t msr, uint32_t* low, uint32_t* high) {
    asm volatile("rdmsr":  "=a"(*low), "=d"(*high): "c" (msr));
}
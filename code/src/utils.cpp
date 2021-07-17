/* 2021 Nikolai Chizhov */

#include "utils.hpp"

namespace Utils {
namespace Sync {

uint32_t __ldrex(volatile uint32_t *addr) {
    uint32_t res;
    __asm__ volatile("ldrex %0, [%1]"
                     : "=r"(res)
                     : "r"(addr));
    return res;
}

uint32_t __strex(uint32_t val, volatile uint32_t *addr) {
    uint32_t res;
    __asm__ volatile("strex %0, %2, [%1]"
                     : "=&r"(res)
                     : "r"(addr), "r"(val));
    return res;
}

void __clrex(void) {
    __asm__ volatile ("clrex");
}

} /* namespace Sync */
} /* namespace Utils */
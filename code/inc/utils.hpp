/* 2021 Nikolai Chizhov */

#pragma once

#include <cstdint>

namespace Utils {
    /* dependent bool value for static asserts */
    template <bool value>
	inline constexpr bool dependentBool = value;
}

namespace Utils {
namespace Sync {

    uint32_t __ldrex(volatile uint32_t *addr);
    uint32_t __strex(uint32_t val, volatile uint32_t *addr);
    void __clrex(void);

    template <typename T>
    class Atomic {
    public:
        static void Set(T addr, T mask, T val, T offset) {
            T prevVal;
            T newVal;
            do {
                prevVal = *reinterpret_cast<volatile T *>(addr);
                newVal = prevVal;
                newVal &= ~(mask << (offset));
                newVal |= (val << (offset));
            } while (
                !TryToWrite(reinterpret_cast<volatile T *>(addr),
                            prevVal,
                            newVal)
            );
        }

    private:
        static bool TryToWrite(volatile T *ptr, T oldVal, T newVal) {
            if (__ldrex(ptr) == static_cast<uint32_t>(oldVal)) {
                return (0 == __strex(static_cast<uint32_t>(newVal),
                                     static_cast<volatile uint32_t *>(ptr))
                );
            }
            __clrex();
            return false;
        }
    };

} /* namespace Sync */
} /* namespace Utils */
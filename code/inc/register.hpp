/* 2021 Nikolai Chizhov */

#pragma once

#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <initializer_list>
#include <limits>

/* Access modes for registers */
struct RegisterMode {
    /* No access (you cannot call methods, but you can read public fields) */
    struct None
    {};
    /* Read-only:
    * If you try to modify it (set, reset, etc.) you will get the compilation error
    */
    struct Read
    {};
    /* Write-only:
    * If you try to read it (get) you will get the compilation error
    */
    struct Write
    {};
    /* Allmighty:
    * No restrinctions
    */
    struct RW : public Write, public Read
    {};
};

/* Type of the register depends on its size */
template <uint32_t size>
struct RegisterDataType
{};

template <>
struct RegisterDataType<16> {
    using Type = uint16_t;
};

template <>
struct RegisterDataType<32> {
    using Type = uint32_t;
};


/* Register
 *
 * For example, GPIOA->ODR is a register
 *
 * address      - address of the hardware resigter
 * size         - size, bits
 * AccessMode   - mode, declared above
 */
template <uintptr_t address, size_t size, typename AccessMode>
class Register {
public:
    /* declared here to be available outside */
    static constexpr uintptr_t Address = address;
    using Type = typename RegisterDataType<size>::Type;

    inline static Type Get() {
        CheckMode<RegisterMode::Read>();

        return *reinterpret_cast<Type *>(address);
    }

    inline static void Set(Type value) {
        CheckMode<RegisterMode::Write>();

        *reinterpret_cast<Type *>(address) = value;
    }

    inline static void Toggle(Type value) {
        CheckMode<RegisterMode::Write>();

        *reinterpret_cast<Type *>(address) ^= value ;
    }

private:
    /* Check the mode, instead of SFINAE */
    template <typename T>
    static constexpr void CheckMode() {
        static_assert(std::is_base_of_v<T, AccessMode>);
    }
};

/* RegisterField
 *
 * For example, GPIOA->ODR[0] is a register field
 *
 * Reg          - register
 * offset       - offset from the register address
 * size         - size, bits
 * AccessMode   - RegisterMode mode, declared above
 */
template <typename Reg, size_t offset, size_t size, typename AccessMode>
class RegisterField {
public:
    /* declared here to be visible outside */
    using RegType = typename Reg::Type;
    using Register = Reg;
    static constexpr RegType Offset = offset;
    static constexpr RegType Size = size;
    static constexpr RegType Mask = (size < sizeof(RegType) * 8U) ?
                                    ((1U << size) - 1U) << offset :
                                    std::numeric_limits<RegType>::max();

    using Access = AccessMode;

    inline static RegType Get() {
        CheckMode<RegisterMode::Read>();

        return ((*reinterpret_cast<RegType *>(Reg::Address)) & Mask) >> offset;
    }

    static void Set(RegType value) {
        CheckMode<RegisterMode::Write>();

        RegType newValue = *reinterpret_cast<RegType *>(Reg::Address);
        newValue &= ~Mask;
        newValue |= (value << offset);

        *reinterpret_cast<RegType *>(Reg::Address) = newValue;
    }

private:
    /* Check the mode, instead of SFINAE */
    template <typename T>
    static inline constexpr void CheckMode() {
        static_assert(std::is_base_of_v<T, AccessMode>);
    }
};

/* Field value
 *
 * For example, ODR->High and ODR->Low are field values
 *
 * Field    - corresponding RegisterField
 * Base     - base type
 * value    - its value (0x01 for ODR->High)
 */
template <typename Field, typename Base, typename Field::Register::Type value>
struct FieldValue {
    using Type = typename Field::Register::Type;
    using BaseType = Base;
    using Access = typename Field::Access;
    constexpr static Type Mask = static_cast<Type>(1U << Field::Size) - 1U;
    constexpr static Type Value = value;
    constexpr static Type Offset = Field::Offset;

    static void Set() {
        CheckMode<RegisterMode::Write>();

        Type newRegValue;
        newRegValue = *reinterpret_cast<Type *>(Field::Register::Address);
        newRegValue &= ~Mask;
        newRegValue |= (value << Field::Offset);

        *reinterpret_cast<Type *>(Field::Register::Address) = newRegValue;
    }

    inline static bool IsSet() {
        CheckMode<RegisterMode::Read>();

        Type regValue = *reinterpret_cast<Type*>(Field::Register::Address);
        return (regValue & Mask) == (value << Field::Offset);
    }

private:
    /* Check the mode, instead of SFINAE */
    template <typename T>
    static inline constexpr void CheckMode() {
        static_assert(std::is_base_of_v<T, typename Field::Access>);
    }
};

/* Register Field Set
 *
 * An alternative way for working with a hardware resiter.
 * Allows you to set sevetal RegisterField values simultaneously.
 *
 * address            - address of the hardware resigter
 * size               - size, bits
 * AccessMode         - mode, declared above
 * FieldValueBaseType - base type
 * Args               - FieldValues
 */
template <uint32_t address, size_t size, typename AccessMode,
          typename FieldValueBaseType, typename... Args>
class RegisterFieldSet {
public:
    using Type = typename RegisterDataType<size>::Type;

    static void Set() {
        CheckMode<RegisterMode::Write>();

        Type regValue = *reinterpret_cast<Type *>(address);
        regValue &= ~GetMask();
        regValue |= GetValue();
        *reinterpret_cast<Type *>(address) = regValue;
    }

    static bool IsSet() {
        CheckMode<RegisterMode::Read>();

        Type regValue = *reinterpret_cast<Type *>(address);
        return ((regValue & GetMask()) == GetValue());
    }

private:
    template <typename T>
    static inline constexpr void CheckMode() {
        static_assert(std::is_base_of_v<T, AccessMode>);
    }

private:
    template <typename T>
    static inline constexpr void CheckBaseType() {
        static_assert(
            std::is_same<FieldValueBaseType, typename T::BaseType>::value,
            "It looks like you're trying to mix different types"
        );
    }

    template <typename FiedlVal>
    static constexpr Type GetItemMask() {
        CheckBaseType<FiedlVal>();

        return FiedlVal::Mask << FiedlVal::Offset;;
    }

    template <typename FiedlVal>
    static constexpr Type GetItemValue() {
        CheckBaseType<FiedlVal>();

        return FiedlVal::Value << FiedlVal::Offset;
    }

    static constexpr Type GetMask() {
        Type result = 0UL;
        const auto values = {GetItemMask<Args>()...};
        for (const auto v : values) {
            result |= v;
        }
        return result;
    }

    static constexpr auto GetValue() {
        Type result = 0UL;
        const auto values = {GetItemValue<Args>()...};
        for (const auto v : values) {
            result |= v;
        }
        return result;
    }
};
